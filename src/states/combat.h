/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "navigate.h"
#include "../Enemy_Instance.h"

#include "../ui/Model.h"

#include <random>

#include "flare.h"
namespace game
{
  enum class Fight_State
  {
    Enemy_Won,
    Player_Won,
    Player_Turn,
    Enemy_Turn,
    Enemy_Animating,
    Player_Animating,
    Running
  };

  enum class Labels_State
  {
    Inventory, Combat
  };

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

    Fight_State fight_state;
    Fight_State from_anim;

    std::random_device random_device;

    inline Player& active_player() noexcept;
    inline Player const& active_player() const noexcept;

    void switch_to_inventory_view() noexcept;
    void switch_to_combat_menu() noexcept;

    Labels_State labels_state;

    int cur_step = 0;
    int max_step = 0;
    int last_damage = 0;
  };

  inline Player& Combat_State::active_player() noexcept
  {
    return navigate.map.players[navigate.player];
  }
  inline Player const& Combat_State::active_player() const noexcept
  {
    return navigate.map.players[navigate.player];
  }

  void set_bar_to_life(ui::Bar& bar, Entity_Data& et) noexcept;

  void set_bar_animating(ui::Bar& bar, Entity_Data& et, int delta_life,
                         int step, int max_step) noexcept;
}