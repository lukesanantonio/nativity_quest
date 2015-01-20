/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>

#include "../game/Game.h"

#include "../ui/View.h"

#include "SDL.h"

namespace game
{
  struct Menu_State : public Game_State
  {
    Menu_State(Game& game);

    ui::Shared_View menu_ui;

    void handle_event(SDL_Event const&) noexcept override;
    void step() noexcept override;
    void render() const noexcept override;

    void on_enter() noexcept override;
    void on_exit() noexcept override;
  };
}

