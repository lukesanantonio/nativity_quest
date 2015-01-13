/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
#include "../Map.h"
#include "../common/vec.h"
#include "../decl/sprites.h"
#include "../decl/players.h"
#include "../decl/effects.h"
namespace game
{
  struct Navigate_State : public Game_State
  {
    Navigate_State(Game& game) noexcept;

    decl::Sprites sprites;
    decl::Players players;

    mutable ui::Model hud;
    mutable bool ui_dirty = true;

    decl::Zone cur_zone;

    Map map;

    player_id player;

    // map coordinates.
    mutable Vec<int> map_corner;
    bool moving_corner = false;

    Vec<double> move = {0,0};

    decl::Effects effects;

    void handle_event(SDL_Event const&) noexcept override;
    void step() noexcept override;
    void render() const noexcept override;

    void on_enter() noexcept override;
    void on_exit() noexcept override;

    void update_cur_zone() noexcept;

    void next_player() noexcept;

    inline Player& active_player() noexcept;
    inline Player const& active_player() const noexcept;

    bool first = true;

  private:
    void render_player(Player const& p,
                       Volume<int> const& viewport_src,
                       bool) const noexcept;
  };

  inline Player& Navigate_State::active_player() noexcept
  {
    return map.players[player];
  }
  inline Player const& Navigate_State::active_player() const noexcept
  {
    return map.players[player];
  }

  struct Navigate_Sub_State : public Game_State
  {
    Navigate_Sub_State(Game& g, Navigate_State& ns) noexcept
                       : Game_State(g, false), navigate(ns) {}

    Navigate_State& navigate;
  };
}
