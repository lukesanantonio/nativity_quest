/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "navigate.h"
#include "../common/volume.h"

#include "../ui/ui.h"

#include "combat_intro.h"
#include "move.h"
#include "inventory.h"
#include "uncrate.h"
#include "player_switch.h"
#include "player_intro.h"

#if 0

#include "win.h"

#endif

#define PI 3.14159

#define PLAYERS_JSON "assets/decl/players.json"
#define MAP_JSON "assets/map.json"
#define ITEMS_JSON "assets/decl/items.json"
#define ENEMIES_JSON "assets/decl/enemies.json"
#define HUD_JSON "ui/navigate"

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

    auto player_name = hud->find_child_r<ui::Label>("player_name");

    player_name->str("Player #" + std::to_string(player));;
    hud->layout(game_.graphics.size());
  }
  Navigate_State::Navigate_State(Game& game) noexcept
                                 : Game_State(game, true),
                                   players(PLAYERS_JSON),
                                   map(game, MAP_JSON, ITEMS_JSON,
                                       ENEMIES_JSON),
                                   player(0),
                                   hud{ui::load(game, HUD_JSON)},
                                   effects{&map.items}
  {
    hud->layout(game.graphics.size());

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

    hud->find_child_r("next_player")->add_event_trigger<ui::Mouse_Click>(
    [this](auto const&)
    {
      this->next_player();
    }, nullptr);

    hud->find_child_r("use_inventory")->add_event_trigger<ui::Mouse_Click>(
    [this](auto const&)
    {
      push_state(game_, std::make_shared<Inventory_View_State>(game_, *this));
    }, nullptr);

    update_cur_zone();
  }

  void Navigate_State::handle_event(SDL_Event const& event) noexcept
  {
    if(!hud->dispatch_event(event))
    {
      if(event.type == SDL_MOUSEBUTTONDOWN)
      {
        if(event.button.button == SDL_BUTTON_LEFT)
        {
          // Get the mouse in screen coordinates.
          auto mouse = Vec<int>{event.button.x, event.button.y};

          // Use the map scale and current top-left corner to calculate the
          // map coordinates.
          auto map_coord = static_cast<Vec<double> >(mouse) / map.scale;
          map_coord += map_corner;

          // Calculate our delta movement.
          auto delta = map_coord - map.players[player].pos;

          auto move = std::make_shared<Movement_State>(game_, *this, delta);
          push_state(game_, move);
        }
      }
    }
  }
  void Navigate_State::step() noexcept
  {
    using std::begin; using std::end;

    // Remove any dead enemies.
    {
      auto new_end = std::remove_if(begin(map.enemies), end(map.enemies),
      [this](auto const& enemy)
      {
        return enemy.entity_data.cur_life == 0;
      });

      map.enemies.erase(new_end, end(map.enemies));
    }

    // Respawn ourselves if necessary.
    if(active_player().entity_data.cur_life == 0)
    {
      active_player().pos = this->active_player().spawn_pt;
      reset_life(this->active_player().entity_data);

      // Switch player
      next_player();
    }

    // Check for any enemies in our view
    {
      auto enemy_find = std::find_if(begin(map.enemies), end(map.enemies),
      [this](auto const& enemy)
      {
        if(enemy.not_fighting) return false;

        // Check if they are a reasonable distance.
        auto len = length(this->active_player().pos - Vec<double>{enemy.pos});
        return len < this->active_player().view_radius;
      });

      if(enemy_find != end(map.enemies))
      {
        // Go into combat.
        push_state(game_,
                   std::make_shared<Combat_Intro>(game_, *this, *enemy_find));
        return;
      }
    }

    // If we cleared the area of enemies, check for chests.
    {
      auto chest_find = std::find_if(begin(map.chests), end(map.chests),
      [this](auto const& chest)
      {
        // If the chest isn't visible, don't allow it to be opened/found.
        if(!chest.visible) return false;

        // Check if it's a suitable distance.
        auto len = length(this->active_player().pos - Vec<double>{chest.pos});
        return len < this->active_player().view_radius / 10;
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
#if 0
        auto win =std::make_shared<Win_State>(game_, players.get_name(player));
        push_state(game_, std::move(win));
#endif
      }
    }
  }
  void Navigate_State::on_enter() noexcept
  {
    if(first)
    {
      auto intro =
        std::make_shared<Player_Intro>(game_, *this, players.get_name(player));
      push_state(game_, std::move(intro));
      first = false;
    }

    // Update the zone just in case.
    update_cur_zone();

    hud->visible(true);
    hud->handle_events(true);
  }
  void Navigate_State::on_exit() noexcept
  {
    hud->visible(false);
    hud->handle_events(false);
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
      auto layout = ui::as<ui::Side_Layout>(hud);

      // Find the zone label and change it's string to that of the current
      // zone.
      auto zone_label = hud->find_child<ui::Label>("zone_label");
      zone_label->str_name(cur_zone->str);

      // We coulldd just layout the parent, but this is I guess safer for
      // possibly any unexpected future changes.
      hud->layout(game_.graphics.size());
    }
  }

  void Navigate_State::render() const noexcept
  {
    auto& active_player = map.players[player];

    // Render the map.
    auto viewport_src = view_pt<int>(game_.graphics.size(), map.size(),
                                     active_player.pos, map.scale);
    if(moving_corner)
    {
      viewport_src.pos = map_corner;
    }

    map_corner = viewport_src.pos;

    auto map_sprite = get_asset<assets::Image_Asset>(game_, "map");

    auto viewport_src_rect = to_sdl_rect(viewport_src);
    SDL_RenderCopy(game_.graphics.renderer,
                   map_sprite->image.texture(game_.graphics.renderer),
                   &viewport_src_rect, NULL);

    // Render the map overlay
    auto map_overlay = get_asset<assets::Image_Asset>(game_, "map_overlay");
    SDL_RenderCopy(game_.graphics.renderer,
                   map_overlay->image.texture(game_.graphics.renderer),
                   &viewport_src_rect, NULL);


    // Render the player.
    for(int pl_id = 0; pl_id < map.players.size(); ++pl_id)
    {
      if(pl_id != player) render_player(map.players[pl_id],
                                        viewport_src,
                                        false);
    }

    render_player(active_player, viewport_src, true);

    // Render any chests. TODO Get rid of this sprite reference!
    auto chest_sprite = get_asset<assets::Image_Asset>(game_, "anim/chest/0");

    for(auto const& chest : map.chests)
    {
      // Only render active chests, others have been opened and shouldn't be
      // shown, TODO make an opened chest sprite.
      if(!chest.visible) continue;

      auto chest_extent = Vec<int>{chest_sprite->image.surface->w,
                                   chest_sprite->image.surface->h};

      SDL_Rect chest_dest;
      chest_dest.x = chest.pos.x - viewport_src.pos.x - chest_extent.x / 2;
      chest_dest.y = chest.pos.y - viewport_src.pos.y - chest_extent.y / 2;

      // Scale to the viewport
      chest_dest.x *= map.scale;
      chest_dest.y *= map.scale;

      chest_dest.w = chest_sprite->image.surface->w * map.scale * .8;
      chest_dest.h = chest_sprite->image.surface->h * map.scale * .8;
      SDL_RenderCopy(game_.graphics.renderer,
                     chest_sprite->image.texture(game_.graphics.renderer),
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
    if(active_player.fog.surface.get())
    {
      SDL_RenderCopy(game_.graphics.renderer,
                     active_player.fog.texture(game_.graphics.renderer),
                     &viewport_src_rect, NULL);
    }

    // Render the Hud.
    hud->render();
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

    auto char_info = players.get_orientation_info(orient);

    SDL_Rect char_src;
    char_src.x = char_info.src.pos.x;
    char_src.y = char_info.src.pos.y;
    char_src.w = char_info.src.width;
    char_src.h = char_info.src.height;

    SDL_Rect char_dest;
    char_dest.x = player_scr_coord.x;
    char_dest.y = player_scr_coord.y;

    auto char_scale = players.get_sprite_scale();

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

    auto char_sprite = get_asset<assets::Image_Asset>(game_,
                                           players.get_sprite(p.sprite_frame));

    SDL_RenderCopyEx(game_.graphics.renderer,
                     char_sprite->image.texture(game_.graphics.renderer),
                     &char_src, &char_dest,
                     angle / PI * 180, &char_center, SDL_FLIP_NONE);
  }
}
