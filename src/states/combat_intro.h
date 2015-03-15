/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "navigate.h"
#include "../ui/ui.h"
#include "../Enemy_Instance.h"
#define COMBAT_INTRO_HUD_JSON
namespace game
{
  struct Combat_Intro : public Navigate_Sub_State
  {
    Combat_Intro(Game& g, Navigate_State& ns, Enemy_Instance& e) noexcept;

    void handle_event(SDL_Event const&) noexcept override;
    void step() noexcept override;
    void render() const noexcept override;

    inline void on_enter() noexcept {}
    void on_exit() noexcept {}

  private:
    void push_state() noexcept;

    mutable ui::Label label_;
    Enemy_Instance& enemy_;
    int cur_step = 0;
    int max_step;
  };
}
