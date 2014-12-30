/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "navigate.h"
#include "../common/volume.h"

#define PI 3.14159

namespace game
{
  Navigate_State::Navigate_State(Game& game, std::string sprite_json,
                                 std::string players_json,
                                 std::string map_json,
                                 std::string items_decl_json,
                                 std::string enemies_decl_json,
                                 std::string hud_json) noexcept
                                 : Game_State(game),
                                   sprites(sprite_json),
                                   players(players_json),
                                   map(sprites, map_json, items_decl_json,
                                       enemies_decl_json),
                                   player(0),
                                   hud{parse_json(hud_json)}
  {
    for(int player_index; player_index < map.players.size(); ++player_index)
    {
      auto& player = map.players[player_index];
      player.inventory[5] = map.items.get_item(players.get_item(player_index));
      player.sprite_frame = player_index;
      respawn(player);
    }

    game_.presenter.sprites(&sprites);
  }

  void Navigate_State::handle_event(SDL_Event const& event) noexcept {}
  void Navigate_State::step() noexcept {}

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

    // Render the player.
    auto player_scr_coord = (active_player.pos - viewport_src.pos) * map.scale;

    // Determine the character orientation, then the required rotation in
    // addition to that.

    auto move = Vec<double>{0.0, 0.0};
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

    game_.view.render(game_.graphics);
  }
}
