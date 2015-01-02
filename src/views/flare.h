/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
namespace game
{
  struct Flare_State : public Game_State
  {
    Flare_State(Game& g) noexcept : Game_State(g, false) {}

    inline void handle_event(SDL_Event const&) noexcept override {}

    void step() noexcept override;
    void render() const noexcept override;

    inline void on_enter() noexcept override {}
    inline void on_exit() noexcept override {}

    int alpha = 0;
  };
}
