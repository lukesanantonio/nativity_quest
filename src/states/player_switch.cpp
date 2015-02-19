/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "player_switch.h"
#include "../common/volume.h"

#include "../game/Game.h"
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
      auto player_str = navigate.players.get_name(navigate.player);
      auto intro = std::make_shared<Player_Intro>(game_, navigate, player_str);
      replace_state(game_, std::move(intro));
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
