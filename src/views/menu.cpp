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
                         view{g}
  {
    presenter.present(view);
  }

  Menu_Data::Menu_Data(Menu_Data&& md) noexcept
                       : model{std::move(md.model)},
                         presenter{std::move(md.presenter)},
                         view{std::move(md.view)}
  {
    presenter.model(model);
  }

  void Menu_Data::handle_event(State& s, SDL_Event const&) noexcept {}
  void Menu_Data::step(State& s) noexcept {}
  void Menu_Data::render(State& s, Graphics_Desc&,
                         Sprite_Container&) const noexcept
  {
    view.render();
  }
}
