/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "intro.h"
#include "../ui/ui.h"
#include "../Enemy_Instance.h"
#define COMBAT_INTRO_HUD_JSON
namespace game
{
  struct Combat_Intro : public Intro_State
  {
    Combat_Intro(Game& g, Navigate_State& ns, Enemy_Instance& e) noexcept;

    inline void on_enter() noexcept {}
    inline void on_exit() noexcept {}

  private:
    void push_state() noexcept;

    void done_() noexcept override;

    Enemy_Instance& enemy_;
  };
}
