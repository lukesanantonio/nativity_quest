/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "step.h"

#include "State.h"
#include "Graphics_Desc.h"
#include "Sprite_Container.h"

#include "render.h"

namespace game
{
  void handle_event(State& s, SDL_Event const& event) noexcept
  {
    switch(s.type)
    {
      case View::Turn:
      {
        handle_event_state(s, boost::get<Turn_Data>(s.state_data), event);
        break;
      }
    }
  }
  void step(State& s) noexcept
  {
    switch(s.type)
    {
      case View::Turn:
      {
        step_state(s, boost::get<Turn_Data>(s.state_data));
        break;
      }
    }
  }

  void render(State& s, Graphics_Desc& g, Sprite_Container& a) noexcept
  {
    switch(s.type)
    {
      case View::Turn:
      {
        render_state(s, g, a, boost::get<Turn_Data>(s.state_data));
        break;
      }
    }
    SDL_RenderPresent(g.renderer);
  }
}
