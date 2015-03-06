/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
#include "../common/animation.h"
namespace game
{
  struct Flare_State : public Game_State
  {
    Flare_State(Game& g) noexcept;

    inline void handle_event(SDL_Event const&) noexcept override {}

    void step() noexcept override;
    void render() const noexcept override;

    inline void on_enter() noexcept override {}
    inline void on_exit() noexcept override {}

    Animation anim;

    static constexpr int factor = 5;
    int alpha = 0;
  };
}
