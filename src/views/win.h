/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
#include "../ui/Model.h"
namespace game
{
  struct Win_State : public Game_State
  {
    Win_State(Game& game, std::string player_name) noexcept;

    inline void handle_event(SDL_Event const&) noexcept {}
    inline void step() noexcept {}
    void render() const noexcept;

    void on_enter() noexcept;
    inline void on_exit() noexcept {}

    ui::Model hud;
  };
}
