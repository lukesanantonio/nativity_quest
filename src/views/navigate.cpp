/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "navigate.h"
#include "../common/volume.h"

#include "move.h"
#include "uncrate.h"
#include "inventory.h"
#include "combat.h"
#include "player_switch.h"
#include "win.h"

#define PI 3.14159

#define NAVIGATE_SPRITES "assets/navigation_sprites.json"
#define PLAYERS_JSON "assets/player.json"
#define MAP_JSON "assets/map.json"
#define ITEMS_JSON "assets/items.json"
#define ENEMIES_JSON "assets/enemies.json"
#define HUD_JSON "assets/navigate_hud.json"

namespace game
{
  void Navigate_State::next_player() noexcept
  {
    // Count down an enemy's wait to start fighting again.
    for(auto& e_instance : map.enemies)
    {
      if(e_instance.not_fighting) --e_instance.not_fighting;
    }

    // Reset a player's movement.
    map.players[player].moved = 0.0;

    // Change the active player.
    if(++player == map.players.size()) player = 0;

    // Push the next-player-animation state.
    push_state(game_, std::make_shared<Player_Switch_State>(game_, *this));
  }
  Navigate_State::Navigate_State(Game& game) noexcept
                                 : Game_State(game, true),
                                   sprites(NAVIGATE_SPRITES),
                                   players(PLAYERS_JSON),
                                   map(sprites, MAP_JSON, ITEMS_JSON,
                                       ENEMIES_JSON),
                                   player(0),
                                   hud{parse_json(HUD_JSON)},
                                   effects{&map.items}
  {
    for(int player_index = 0; player_index < map.players.size();
        ++player_index)
    {
      auto& player = map.players[player_index];
      player.inventory[5] = map.items.get_item(players.get_item(player_index));
      player.sprite_frame = player_index;
      respawn(player);

      reset_fog(player, map.size());
      unfog(player, effects);
    }

    game_.presenter.sprites(&sprites);

    game_.presenter.use_handler("on_next_player",
    [this](auto const&)
    {
      next_player();
    });
    game_.presenter.use_handler("on_inventory_view",
    [this](auto const&)
    {
      push_state(game_, std::make_shared<Inventory_View_State>(game_, *this));
    });

    update_cur_zone();
  }

  void Navigate_State::handle_event(SDL_Event const& event) noexcept
  {
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
      if(event.button.button == SDL_BUTTON_LEFT)
      {
        // Get the mouse in screen coordinates.
        auto mouse = Vec<int>{event.button.x, event.button.y};

        // Use the map scale and current top-left corner to calculate the
        // map coordinates.
        auto map_coord = (mouse / map.scale) + map_corner;

        // Calculate our delta movement.
        auto delta = map_coord - map.players[player].pos;

        auto move = std::make_shared<Movement_State>(game_, *this, delta);
        push_state(game_, move);
      }
    }
  }
  void Navigate_State::step() noexcept
  {
    auto& active_player = map.players[player];

    using std::begin; using std::end;

    // Remove any dead enemies.
    {
      auto new_end = std::remove_if(begin(map.enemies), end(map.enemies),
      [](auto const& enemy)
      {
        return enemy.entity_data.cur_life == 0;
      });

      map.enemies.erase(new_end, end(map.enemies));
    }

    // Respawn ourselves if necessary.
    if(active_player.entity_data.cur_life == 0)
    {
      active_player.pos = active_player.spawn_pt;
      reset_life(active_player.entity_data);

      // Switch player
      next_player();
    }

    // Check for any enemies in our view
    {
      auto enemy_find = std::find_if(begin(map.enemies), end(map.enemies),
      [&active_player](auto const& enemy)
      {
        if(enemy.not_fighting) return false;

        // Check if they are a reasonable distance.
        auto len = length(active_player.pos - Vec<double>{enemy.pos});
        return len < active_player.view_radius;
      });

      if(enemy_find != end(map.enemies))
      {
        // Go into combat.
        push_state(game_,
                   std::make_shared<Combat_State>(game_, *this, *enemy_find));
        return;
      }
    }

    // If we cleared the area of enemies, check for chests.
    {
      auto chest_find = std::find_if(begin(map.chests), end(map.chests),
      [&active_player](auto const& chest)
      {
        // If the chest isn't visible, don't allow it to be opened/found.
        if(!chest.visible) return false;

        // Check if it's a suitable distance.
        auto len = length(active_player.pos - Vec<double>{chest.pos});
        return len < active_player.view_radius;
      });

      if(chest_find != end(map.chests))
      {
        push_state(game_,
                   std::make_shared<Uncrate_State>(game_, *this, *chest_find));
        return;
      }
    }

    // Can it be? Have we reached Bethlehem?
    if(cur_zone != decl::no::zone)
    {
      if(cur_zone->important)
      {
        auto string = "Player " + std::to_string(player);
        push_state(game_, std::make_shared<Win_State>(game_, string));
      }
    }
  }
  void Navigate_State::on_enter() noexcept
  {
    ui_dirty = true;

    // Update the zone just in case.
    update_cur_zone();

    game_.presenter.handle_events(true);
  }
  void Navigate_State::on_exit() noexcept
  {
    game_.presenter.handle_events(false);
  }

  void Navigate_State::update_cur_zone() noexcept
  {
    auto& active_player = map.players[player];
    auto new_zone = map.zones.get_zone(active_player.pos);

    // If the zone is both new and valid.
    if(new_zone != cur_zone && new_zone != decl::no::zone)
    {
      cur_zone = new_zone;

      // Update the ui.
      auto& text = boost::get<ui::Text>(hud.elements[1].element);
      text.str = cur_zone->str;

      ui_dirty = true;
    }
  }

  void Navigate_State::render() const noexcept
  {
    if(ui_dirty)
    {
      game_.view.reset();
      game_.presenter.present(hud, game_.view, game_.graphics.size());
      ui_dirty = false;
    }

    auto& active_player = map.players[player];

    // Render the map.
    auto viewport_src = view_pt<int>(game_.graphics.size(), map.size(),
                                     active_player.pos, map.scale);
    if(moving_corner)
    {
      viewport_src.pos = map_corner;
    }

    map_corner = viewport_src.pos;

    auto map_sprite = sprites.get_sprite(map.map_sprite);

    auto viewport_src_rect = to_sdl_rect(viewport_src);
    SDL_RenderCopy(game_.graphics.renderer,
                   map_sprite->texture(game_.graphics.renderer),
                   &viewport_src_rect, NULL);

    // Render the map overlay
    auto map_overlay_sprite = sprites.get_sprite(map.map_overlay_sprite);
    SDL_RenderCopy(game_.graphics.renderer,
                   map_overlay_sprite->texture(game_.graphics.renderer),
                   &viewport_src_rect, NULL);


    // Render the player.
    for(int pl_id = 0; pl_id < map.players.size(); ++pl_id)
    {
      if(pl_id != player) render_player(map.players[pl_id],
                                        viewport_src,
                                        false);
    }

    render_player(active_player, viewport_src, true);

    // Render any chests.
    auto chest_sprite = sprites.get_sprite(map.chest_sprite);

    for(auto const& chest : map.chests)
    {
      // Only render active chests, others have been opened and shouldn't be
      // shown, TODO make an opened chest sprite.
      if(!chest.visible) continue;

      auto chest_extent =
              Vec<int>{chest_sprite->surface()->w, chest_sprite->surface()->h};

      SDL_Rect chest_dest;
      chest_dest.x = chest.pos.x - viewport_src.pos.x - chest_extent.x / 2;
      chest_dest.y = chest.pos.y - viewport_src.pos.y - chest_extent.y / 2;

      // Scale to the viewport
      chest_dest.x *= map.scale;
      chest_dest.y *= map.scale;

      chest_dest.w = chest_sprite->surface()->w * map.scale * .8;
      chest_dest.h = chest_sprite->surface()->h * map.scale * .8;
      SDL_RenderCopy(game_.graphics.renderer,
                     chest_sprite->texture(game_.graphics.renderer),
                     NULL, &chest_dest);
    }

    for(auto const& enemy : map.enemies) {
      auto enemy_extents = Vec<int>{15, 15};

      // Render fighting enemies red and non-fighting enemies yellow.
      SDL_Rect enemy_dest;
      enemy_dest.x = enemy.pos.x - viewport_src.pos.x - enemy_extents.x / 2;
      enemy_dest.y = enemy.pos.y - viewport_src.pos.y - enemy_extents.y / 2;

      // Scale to the viewport
      enemy_dest.x *= map.scale;
      enemy_dest.y *= map.scale;

      enemy_dest.w = enemy_extents.x;
      enemy_dest.h = enemy_extents.y;

      if(enemy.not_fighting == 0)
      {
        SDL_SetRenderDrawColor(game_.graphics.renderer,
                               0xff, 0x00, 0x00, 0xff);
      }
      else
      {
        SDL_SetRenderDrawColor(game_.graphics.renderer,
                               0xff, 0xff, 0x00, 0xff);
      }

      SDL_RenderFillRect(game_.graphics.renderer, &enemy_dest);

      --enemy_dest.x;
      --enemy_dest.y;
      enemy_dest.w += 2;
      enemy_dest.h += 2;

      SDL_SetRenderDrawColor(game_.graphics.renderer, 0x00, 0x00, 0x00, 0xff);
      SDL_RenderDrawRect(game_.graphics.renderer, &enemy_dest);
    }

    // Render the fog of war.
    if(active_player.fog.surface())
    {
      SDL_RenderCopy(game_.graphics.renderer,
                     active_player.fog.texture(game_.graphics.renderer),
                     &viewport_src_rect, NULL);
    }

    game_.view.render(game_.graphics);
  }
  void Navigate_State::render_player(Player const& p,
                                Volume<int> const& viewport_src,
                                bool use_orient) const noexcept
  {
    auto player_scr_coord = (p.pos - viewport_src.pos) * map.scale;

    // Determine the character orientation, then the required rotation in
    // addition to that.
    auto move_unit = normalize(move);

    if(!use_orient)
    {
      move_unit = {0, 0};
    }

    auto angle = std::atan2(move_unit.y, move_unit.x);

    auto segment_len = (2 * PI) / 4;

    auto orient = short{0};

    if(segment_len + segment_len / 2 >= angle &&
       angle >= segment_len - segment_len / 2)
    {
      orient = 0;
      angle = angle - segment_len;
    }
    else if(segment_len / 2 >= angle && angle >= -segment_len / 2)
    {
      orient = 1;
      // angle = angle
    }
    else if(-segment_len - segment_len / 2 <= angle &&
            angle <= -segment_len + segment_len / 2)
    {
      orient = 2;
      angle = angle - -segment_len;
    }
    else if(angle >= 2 * segment_len - segment_len / 2 ||
            2 * -segment_len + segment_len / 2 >= angle)
    {
      orient = 3;
      if(angle > 0)
        angle = angle - segment_len * 2;
      else
        angle = angle - -segment_len * 2;
    }

    auto char_info = players.get_sprite_source(orient);
    auto char_scale = players.get_sprite_scale();

    SDL_Rect char_src;
    char_src.x = char_info.src.pos.x;
    char_src.y = char_info.src.pos.y;
    char_src.w = char_info.src.width;
    char_src.h = char_info.src.height;

    SDL_Rect char_dest;
    char_dest.x = player_scr_coord.x;
    char_dest.y = player_scr_coord.y;

    // The center point of the sprite should be the player position.
    char_dest.x -= char_info.center.x * map.scale * char_scale;
    char_dest.y -= char_info.center.y * map.scale * char_scale;

    // The width is scaled first by the map scale, then by the sprite-specific
    // scale.
    char_dest.w = char_info.src.width * map.scale * char_scale;
    char_dest.h = char_info.src.height * map.scale * char_scale;

    SDL_Point char_center;
    char_center.x = char_info.center.x * map.scale * char_scale;
    char_center.y = char_info.center.y * map.scale * char_scale;

    auto char_sprite = sprites.get_sprite(players.get_sprite(),
                                          p.sprite_frame);

    SDL_RenderCopyEx(game_.graphics.renderer,
                     char_sprite->texture(game_.graphics.renderer),
                     &char_src, &char_dest,
                     angle / PI * 180, &char_center, SDL_FLIP_NONE);
  }
}
