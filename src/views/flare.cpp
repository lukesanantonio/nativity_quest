/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "flare.h"
namespace game
{
  void Flare_State::step() noexcept
  {
    alpha = std::min(alpha + 5, 0xff);
    if(alpha == 0xff)
    {
      pop_state(game_);
    }
  }
  void Flare_State::render() const noexcept
  {
    SDL_SetRenderDrawColor(game_.graphics.renderer, 0xff, 0xff, 0xff, alpha);
    SDL_RenderClear(game_.graphics.renderer);
  }
}
