/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "player_intro.h"
namespace game
{
  Player_Intro::Player_Intro(Game& game, Navigate_State& ns,
                             std::string pn) noexcept
                             : Navigate_Sub_State(game, ns),
                               player_name(pn),
                               max_step(80)
  {
    using namespace std::literals;

    auto text = ui::Text{player_name + "'s turn!", "white",
                         "black"s};
    auto alignment = ui::Alignment{ui::Horizontal_Alignment::Center,
                                   ui::Vertical_Alignment::Center};
    hud.elements.push_back({text, {}, alignment});
  }
  void Player_Intro::handle_event(SDL_Event const& event) noexcept
  {
    if((event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYDOWN) &&
       max_step / 2 < cur_step)
    {
      pop_state(game_);
    }
  }
  void Player_Intro::step() noexcept
  {
    if(cur_step++ == max_step) pop_state(game_);
  }
  void Player_Intro::render() const noexcept
  {
    game_.view.reset();
    game_.presenter.present(hud, game_.view, game_.graphics.size());
    game_.view.render(game_.graphics);
  }
  void Player_Intro::on_exit() noexcept
  {
    auto& text = boost::get<ui::Text>(navigate.hud.elements[4].element);
    text.str = "Player: " + player_name;
    navigate.ui_dirty = true;
  }
}
