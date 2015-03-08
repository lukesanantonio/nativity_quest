/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "win.h"
#include "navigate.h"

#define HUD_JSON "ui/win"
namespace game
{
  Win_State::Win_State(Game& game, std::string player_name) noexcept
                       : Game_State(game, false), hud(ui::load(game, HUD_JSON))
  {
    hud->find_child_r<ui::Label>("msg")->str_args(std::move(player_name));

    auto but = hud->find_child_r("menu_return_button");
    but->add_event_trigger<ui::Mouse_Click>([this](auto const&)
    {
      // Pop this state and presumable the navigation state. This could
      // probably be a bit more reliable and explicit but it works always
      // because no // other state pops this one above anything other than a
      // navigation state // anyway.
      pop_state(game_);
      pop_state(game_);
    }, true);
  }
  void Win_State::on_enter() noexcept {}
  void Win_State::render() const noexcept
  {
    hud->render();
  }
}
