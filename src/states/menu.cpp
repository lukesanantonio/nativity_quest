/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "menu.h"
#include "../common/json.h"
#include "navigate.h"

#define MENU_JSON "assets/menu.json"

namespace game
{
  Menu_State::Menu_State(Game& game)
                         : model{parse_json(MENU_JSON)}, Game_State(game, true)
  {
    game.presenter.use_handler("on_start", [this](Vec<int>)
    {
      push_state(game_, std::make_shared<Navigate_State>(game_));
    });
    game.presenter.use_handler("on_quit", [this](Vec<int>)
    {
      pop_state(game_);
    });
  }

  void Menu_State::handle_event(SDL_Event const& event) noexcept {}
  void Menu_State::step() noexcept {}
  void Menu_State::render() const noexcept
  {
    game_.view.render(game_.graphics);
  }

  void Menu_State::on_enter() noexcept
  {
    game_.view.reset();
    game_.presenter.present(model, game_.view, game_.graphics.size());

    game_.presenter.handle_events(true);
  }
  void Menu_State::on_exit() noexcept
  {
    game_.presenter.handle_events(false);
  }
}
