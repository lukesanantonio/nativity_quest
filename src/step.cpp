/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "step.h"

#include "State.h"
#include "Graphics_Desc.h"

namespace game
{
  void handle_event(State&, SDL_Event const&) noexcept
  {
  }
  void step(State&) noexcept
  {
  }
  void render(State& s, Graphics_Desc& g) noexcept
  {
    switch(s.type)
    {
      case View::Player_Turn:
      {
        break;
      }
      default:
      {
        SDL_SetRenderDrawColor(g.renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g.renderer);
        SDL_RenderPresent(g.renderer);
        break;
      }
    }
  }
}
