/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "navigate.h"
#include "../decl/items.h"
namespace game
{
  struct Discard_State : public Navigate_Sub_State
  {
    Discard_State(Game& g, Navigate_State& ns, decl::Item item) noexcept;

    void handle_event(SDL_Event const&) noexcept;
    void step() noexcept;
    void render() const noexcept;

    void on_enter() noexcept override;
    void on_exit() noexcept override;

    decl::Item item;
    ui::Model hud;
  };
}
