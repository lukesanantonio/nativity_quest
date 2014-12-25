/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "navigate.h"
#include "../common/volume.h"
namespace game
{
  void Navigate_State::handle_event(SDL_Event const&) noexcept
  {
  }
  void Navigate_State::step() noexcept
  {
  }
  void Navigate_State::render() const noexcept
  {
    auto& active_player = map.players[player];

    auto viewport_src = view_pt<int>(game_.graphics.size(), map.size(),
                                     active_player.pos, map.scale);

    auto map_sprite = sprites.get_sprite(map.map_sprite);

    auto viewport_src_rect = to_sdl_rect(viewport_src);
    SDL_RenderCopy(game_.graphics.renderer,
                   map_sprite->texture(game_.graphics.renderer),
                   &viewport_src_rect, NULL);
  }
}
