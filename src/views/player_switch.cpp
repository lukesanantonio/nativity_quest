/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "player_switch.h"
#include "../common/volume.h"

#include "player_intro.h"
namespace game
{
  Player_Switch_State::Player_Switch_State(Game& g,
                                           Navigate_State& ns) noexcept
                                           : Navigate_Sub_State(g, ns),
                                             where(ns.map_corner) {}
  void Player_Switch_State::step() noexcept
  {
    auto active_player_src =
      view_pt<int>(game_.graphics.size(), navigate.map.size(),
                   navigate.active_player().pos, navigate.map.scale);

    auto delta = active_player_src.pos - where;
    auto norm_delta = normalize<double>(delta);
    where += norm_delta * std::min(length(delta), 5.0);
    navigate.map_corner = where;

    if(navigate.map_corner == active_player_src.pos)
    {
      auto player_str = "Player " + std::to_string(navigate.player + 1);
      replace_state(game_, std::make_shared<Player_Intro>(game_, player_str));
    }
  }

  void Player_Switch_State::on_enter() noexcept
  {
    navigate.moving_corner = true;
  }
  void Player_Switch_State::on_exit() noexcept
  {
    navigate.moving_corner = false;
  }
}
