/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
#include "navigate.h"
namespace game
{
  struct Movement_State : public Navigate_Sub_State
  {
    Movement_State(Game& game, Navigate_State& ns, Vec<int> delta) noexcept
                   : Navigate_Sub_State(game, ns), delta(delta) {}

    Vec<double> delta;

    inline void handle_event(SDL_Event const&) noexcept override {}
    void step() noexcept override;
    inline void render() const noexcept override {}

    inline void on_enter() noexcept override {}
    void on_exit() noexcept override;
  };
}
