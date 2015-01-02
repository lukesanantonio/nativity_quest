/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "navigate.h"
#include "../Enemy_Instance.h"

#include "../ui/Model.h"
namespace game
{
  struct Combat_State : public Navigate_Sub_State
  {
    Combat_State(Game& g, Navigate_State& ns, Enemy_Instance& e) noexcept;

    void handle_event(SDL_Event const&) noexcept override;
    void step() noexcept override;
    void render() const noexcept override;

    void on_enter() noexcept override;
    void on_exit() noexcept override;

    ui::Model hud;

    Enemy_Instance& enemy;

    inline Player& active_player() noexcept;
    inline Player const& active_player() const noexcept;
  };

  inline Player& Combat_State::active_player() noexcept
  {
    return navigate.map.players[navigate.player];
  }
  inline Player const& Combat_State::active_player() const noexcept
  {
    return navigate.map.players[navigate.player];
  }
}
