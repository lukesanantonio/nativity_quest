/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "menu.h"
#include "../common/json.h"
//#include "navigate.h"

#include "../ui/load.h"

#define MENU_JSON "assets/menu.json"

namespace game
{
  Menu_State::Menu_State(Game& game)
                         : menu_ui{ui::load(game, MENU_JSON)},
                           Game_State(game, true)
  {
#if 0
    game.presenter.use_handler("on_start", [this](Vec<int>)
    {
      push_state(game_, std::make_shared<Navigate_State>(game_));
    });
    game.presenter.use_handler("on_quit", [this](Vec<int>)
    {
      pop_state(game_);
    });
#endif
    menu_ui->layout(game.graphics.size());
  }

  void Menu_State::handle_event(SDL_Event const& event) noexcept
  {
    menu_ui->dispatch_event(event);
  }
  void Menu_State::step() noexcept {}
  void Menu_State::render() const noexcept
  {
    menu_ui->render();
  }

  void Menu_State::on_enter() noexcept
  {
  }
  void Menu_State::on_exit() noexcept
  {
  }
}
