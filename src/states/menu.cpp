/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "menu.h"
#include "../common/json.h"
//#include "navigate.h"

#include "../ui/ui.h"

#define MENU_JSON "assets/menu.json"

namespace game
{
  Menu_State::Menu_State(Game& game)
                         : menu_ui{ui::load(game, MENU_JSON)},
                           Game_State(game, true)
  {

    auto layout = ui::as<ui::Linear_Layout>(menu_ui);
    layout->find_child("start")->add_event_trigger<ui::Mouse_Click>(
    [&](Vec<int>)
    {
      // start
    });
    layout->find_child("quit")->add_event_trigger<ui::Mouse_Click>(
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
