/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "navigate.h"
namespace game
{
  struct Player_Switch_State : public Navigate_Sub_State
  {
    Player_Switch_State(Game& g, Navigate_State& ns) noexcept;

    inline void handle_event(SDL_Event const&) noexcept override {}
    void step() noexcept override;
    inline void render() const noexcept override {}

    void on_enter() noexcept override;
    void on_exit() noexcept override;

    Vec<double> where;

    int cur_step = 0;
  };
}
