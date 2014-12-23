/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "menu.h"
#include "../util/json.h"
namespace game
{
  Menu_Data::Menu_Data(Game& game, std::string const& menu_json)
                       : model{parse_json(menu_json)}, presenter{},
                         view{game.font}, Game_State(game)
  {
    presenter.present(model, view, game.graphics.size());
  }

  void Menu_Data::handle_event(SDL_Event const&) noexcept {}
  void Menu_Data::step() noexcept {}
  void Menu_Data::render() const noexcept
  {
    view.render(game_.graphics);
  }
}
