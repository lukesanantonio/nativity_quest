/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "menu.h"
#include "../common/json.h"
#include "navigate.h"

#include "../ui/ui.h"

#define MENU_JSON "assets/menu.json"

namespace game
{
  Menu_State::Menu_State(Game& game)
                         : Game_State(game, true),
                           menu_ui{ui::load(game, MENU_JSON)}
  {

    menu_ui->find_child_r("start")->add_event_trigger<ui::Mouse_Click>(
    [&](Vec<int>)
    {
      // start
      push_state<Navigate_State>(game_);
    });
    menu_ui->find_child_r("quit")->add_event_trigger<ui::Mouse_Click>(
    [&](Vec<int>)
    {
      // quit
      pop_state(game_);
    });

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
