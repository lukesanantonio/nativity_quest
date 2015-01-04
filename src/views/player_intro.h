/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
namespace game
{
  struct Player_Intro : public Game_State
  {
    Player_Intro(Game& game, std::string player_name) noexcept;

    void handle_event(SDL_Event const&) noexcept;
    void step() noexcept;
    void render() const noexcept;

    inline void on_enter() noexcept {}
    inline void on_exit() noexcept {}

    mutable ui::Model hud;
    int cur_step = 0;
    int max_step;
  };
}
