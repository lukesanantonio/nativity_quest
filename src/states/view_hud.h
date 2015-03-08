/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
#include "../ui/ui.h"
#include "../common/animation.h"
namespace game
{
  struct View_Hud_State : public Game_State
  {
    View_Hud_State(Game&, std::string) noexcept;

    void handle_event(SDL_Event const&) noexcept;
    void step() noexcept;
    void render() const noexcept;

    inline void on_enter() noexcept {}
    inline void on_exit() noexcept {}

    ui::Shared_View hud;
    Animation anim;

    Color background = {0x00, 0x00, 0x00};
  };
}
