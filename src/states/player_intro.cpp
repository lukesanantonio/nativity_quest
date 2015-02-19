/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "player_intro.h"
#include "../ui/ui.h"
namespace game
{
  Player_Intro::Player_Intro(Game& game, Navigate_State& ns,
                             std::string pn) noexcept
                             : Navigate_Sub_State(game, ns),
                               label_(game.graphics, game.font),
                               player_name(pn),
                               max_step(80)
  {
    label_.str(player_name + "'s turn!");
    label_.layout(game.graphics.size());
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
    label_.render();
  }
  void Player_Intro::on_exit() noexcept
  {
    auto navigate_hud = ui::as<ui::Side_Layout>(navigate.hud);
    auto text = navigate_hud->find_child<ui::Label>("player_name");
    text->str("Player: " + player_name);
  }
}
