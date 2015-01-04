/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "navigate.h"
namespace game
{
  struct Player_Intro : public Navigate_Sub_State
  {
    Player_Intro(Game& game, Navigate_State& ns,
                 std::string player_name) noexcept;

    void handle_event(SDL_Event const&) noexcept;
    void step() noexcept;
    void render() const noexcept;

    inline void on_enter() noexcept {}
    void on_exit() noexcept;

    mutable ui::Model hud;
    std::string player_name;
    int cur_step = 0;
    int max_step;
  };
}
