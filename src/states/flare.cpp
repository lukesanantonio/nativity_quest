/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "flare.h"
namespace game
{
  Flare_State::Flare_State(Game& g) noexcept
                           : Game_State(g, false), anim(1, 0xff / factor)
  {
    anim.set_segment_fn([&](int segment)
    {
      this->alpha = std::min(segment * factor, 0xff);
    });
  }
  void Flare_State::step() noexcept
  {
    if(anim.done())
    {
      pop_state(game_);
    }
    else
    {
      anim.step();
    }
  }
  void Flare_State::render() const noexcept
  {
    // TODO Implement some sort of query to the animation object that will
    // allow us to chuck our storage of alpha and just get it from the anim
    // object when we need it.
    SDL_SetRenderDrawColor(game_.graphics.renderer, 0xff, 0xff, 0xff, alpha);
    SDL_RenderClear(game_.graphics.renderer);
  }
}
