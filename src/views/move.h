/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
namespace game
{
  struct Navigate_State;

  struct Movement_State : public Game_State
  {
    Movement_State(Game& game, Navigate_State& ns,
                   Vec<int> delta) noexcept
                   : Game_State(game), ns(ns), delta(delta) {}

    Navigate_State& ns;
    Vec<double> delta;

    inline void handle_event(SDL_Event const&) noexcept override {}
    void step() noexcept override;
    inline void render() const noexcept override {}

    inline void on_enter() noexcept override {}
    inline void on_exit() noexcept override {}

    inline bool is_toplevel() const noexcept override { return false; }
  };
}
