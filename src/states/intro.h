/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../ui/views/label.h"
#include "navigate.h"
namespace game
{
  struct Intro_State : public Navigate_Sub_State
  {
    Intro_State(Game& game, Navigate_State& ns, ui::Label&& l,
                int max_step = 80) noexcept;

    void handle_event(SDL_Event const&) noexcept;
    void step() noexcept;
    void render() const noexcept;

    mutable ui::Label label;
    int cur_step = 0;
    int max_step;
  private:
    virtual void done_() noexcept;
  };
}
