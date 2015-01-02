/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
#include "navigate.h"
namespace game
{
  struct Uncrate_State : public Navigate_Sub_State
  {
    Uncrate_State(Game& game, Navigate_State& ns, Chest& chest) noexcept
                  : Navigate_Sub_State(game, ns), chest(chest) {}

    inline void handle_event(SDL_Event const&) noexcept override {}
    void step() noexcept override;
    void render() const noexcept override;

    inline void on_enter() noexcept override {}
    void on_exit() noexcept override;

    Chest& chest;

    int anim_frame = 0;
    int intermediate_counter = 0;
  };
}

