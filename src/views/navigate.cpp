/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "navigate.h"
#include "../common/volume.h"

#include "move.h"
#include "uncrate.h"

#define PI 3.14159

namespace game
{
  Navigate_State::Navigate_State(Game& game, std::string sprite_json,
                                 std::string players_json,
                                 std::string map_json,
                                 std::string items_decl_json,
                                 std::string enemies_decl_json,
                                 std::string hud_json) noexcept
                                 : Game_State(game, true),
                                   sprites(sprite_json),
                                   players(players_json),
                                   map(sprites, map_json, items_decl_json,
                                       enemies_decl_json),
                                   player(0),
                                   hud{parse_json(hud_json)}
  {
    for(int player_index = 0; player_index < map.players.size();
        ++player_index)
    {
      auto& player = map.players[player_index];
      player.inventory[5] = map.items.get_item(players.get_item(player_index));
      player.sprite_frame = player_index;
      respawn(player);
    }

    game_.presenter.sprites(&sprites);

    game_.presenter.use_handler("on_next_player",
    [this](auto const&)
    {
      map.players[player].moved = 0.0;
      if(++player == map.players.size()) player = 0;
    });
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
      }
    }

    // Can it be? Have we reached Bethlehem?
  }
  void Navigate_State::on_enter() noexcept
  {
    game_.view.reset();
    game_.presenter.present(hud, game_.view, game_.graphics.size());

    game_.presenter.handle_events(true);
  }
  void Navigate_State::on_exit() noexcept
  {
    game_.presenter.handle_events(false);
  }

  void Navigate_State::render() const noexcept
  {
    auto& active_player = map.players[player];

    // Render the map.
    auto viewport_src = view_pt<int>(game_.graphics.size(), map.size(),
                                     active_player.pos, map.scale);

    auto map_sprite = sprites.get_sprite(map.map_sprite);

    auto viewport_src_rect = to_sdl_rect(viewport_src);
    SDL_RenderCopy(game_.graphics.renderer,
                   map_sprite->texture(game_.graphics.renderer),
                   &viewport_src_rect, NULL);

    map_corner = viewport_src.pos;

    // Render the player.
    auto player_scr_coord = (active_player.pos - viewport_src.pos) * map.scale;

    // Determine the character orientation, then the required rotation in
    // addition to that.
    auto move_unit = normalize(move);
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
                                          active_player.sprite_frame);

    SDL_RenderCopyEx(game_.graphics.renderer,
                     char_sprite->texture(game_.graphics.renderer),
                     &char_src, &char_dest,
                     angle / PI * 180, &char_center, SDL_FLIP_NONE);

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

    for(auto const& enemy : map.enemies)
    {
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

    game_.view.render(game_.graphics);
  }
}
