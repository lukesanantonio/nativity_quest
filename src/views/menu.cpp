/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "menu.h"
#include "../util/json.h"
namespace game
{
  Menu_Data::Menu_Data(Graphics_Desc& g, std::string const& menu_json)
                       : model{parse_json(menu_json)},
                         presenter{model},
                         view{g} {}

  Menu_Data::Menu_Data(Menu_Data&& md) noexcept
                       : model{std::move(md.model)},
                         presenter{std::move(md.presenter)},
                         view{std::move(md.view)}
  {
    presenter.model(model);
  }

  void handle_event_state(State& s, Menu_Data&, SDL_Event const&) noexcept {}
  void step_state(State& s, Menu_Data&) noexcept {}
  void render_state(State& s, Graphics_Desc&,
                    Sprite_Container&, Menu_Data& menu) noexcept
  {
    menu.presenter.present(menu.view);
    menu.view.render();
  }
}
