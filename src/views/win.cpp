/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "win.h"

#include "navigate.h"
#include "../common/json.h"

#define HUD_JSON "assets/win_hud.json"
namespace game
{
  Win_State::Win_State(Game& game, std::string player_name) noexcept
                       : Game_State(game, false), hud(parse_json(HUD_JSON))
  {
    boost::get<ui::Text>(hud.elements[0].element).str = player_name + " won!";

    game_.presenter.use_handler("on_menu_return", [this](auto const&)
    {
      pop_state(game_);
      pop_state(game_);
    });

    game_.view.reset();
    game_.presenter.present(hud, game_.view, game.graphics.size());
  }
  void Win_State::on_enter() noexcept
  {
    game_.presenter.handle_events(true);
  }
  void Win_State::render() const noexcept
  {
    game_.view.render(game_.graphics);
  }
}
