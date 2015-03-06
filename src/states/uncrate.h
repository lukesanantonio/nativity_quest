/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
#include "../common/animation.h"
#include "navigate.h"
#include "../ui/ui.h"
namespace game
{
  struct Uncrate_State : public Navigate_Sub_State
  {
    Uncrate_State(Game& game, Navigate_State& ns, Chest& chest) noexcept;

    void handle_event(SDL_Event const&) noexcept override;
    void step() noexcept override;
    void render() const noexcept override;

    inline void on_enter() noexcept override {}
    void on_exit() noexcept override;

    Chest& chest;

    ui::Shared_View hud;

    Animation anim;
    bool user_continue = false;

  private:
    void on_anim_segment(int segment) noexcept;
  };
}

