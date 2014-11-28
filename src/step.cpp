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
      case View::Menu:
      {
        handle_event_state(boost::get<Menu_Data>(s.state_data), event);
        break;
      }
      case View::Turn:
      {
        handle_event_state(boost::get<Turn_Data>(s.state_data), event);
        break;
      }
      case View::Combat:
      {
        handle_event_state(boost::get<Combat_Data>(s.state_data), event);
        break;
      }
      case View::Trade:
      {
        handle_event_state(boost::get<Trade_Data>(s.state_data), event);
        break;
      }
    }
  }
  void step(State& s) noexcept
  {
    switch(s.type)
    {
      case View::Menu:
      {
        step_state(boost::get<Menu_Data>(s.state_data));
        break;
      }
      case View::Turn:
      {
        step_state(boost::get<Turn_Data>(s.state_data));
        break;
      }
      case View::Combat:
      {
        step_state(boost::get<Combat_Data>(s.state_data));
        break;
      }
      case View::Trade:
      {
        step_state(boost::get<Trade_Data>(s.state_data));
        break;
      }
    }
  }

  void render(State& s, Graphics_Desc& g, Sprite_Container& a) noexcept
  {
    switch(s.type)
    {
      case View::Menu:
      {
        render_state(g, a, boost::get<Menu_Data>(s.state_data));
        break;
      }
      case View::Turn:
      {
        render_state(g, a, boost::get<Turn_Data>(s.state_data));
        break;
      }
      case View::Combat:
      {
        render_state(g, a, boost::get<Combat_Data>(s.state_data));
        break;
      }
      case View::Trade:
      {
        render_state(g, a, boost::get<Trade_Data>(s.state_data));
        break;
      }
    }
    SDL_RenderPresent(g.renderer);
  }
}
