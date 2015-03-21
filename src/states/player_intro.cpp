/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "player_intro.h"
#include "../ui/ui.h"
namespace game
{
  namespace
  {
    ui::Label make_l(Game& g, std::string pn) noexcept
    {
      auto l = ui::Label{g.graphics, g.font};;
      l.str_name("player_turn_intro");
      l.str_args(pn);
      return l;
    }
  }

  Player_Intro::Player_Intro(Game& g, Navigate_State& ns,
                             std::string pn) noexcept
                             : Intro_State(g, ns, make_l(g, pn)),
                               pn_(pn) { }
  void Player_Intro::on_enter() noexcept { }
  void Player_Intro::on_exit() noexcept
  {
    // When the intro is finished, set the player name in the navigation hud.
    auto navigate_hud = ui::as<ui::Side_Layout>(navigate.hud);
    auto text = navigate_hud->find_child<ui::Label>("player_name");
    text->str_name("player_name");
    text->str_args(pn_);

    navigate.hud->layout(game_.graphics.size());
  }
}
