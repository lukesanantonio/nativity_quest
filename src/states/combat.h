/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "navigate.h"
#include "../Enemy_Instance.h"

#include "../ui/ui.h"

#include <random>

#include "flare.h"
#include "../Entity_Health_Mediator.h"
namespace game
{
  enum class Fight_State
  {
    Enemy_Won,
    Player_Won,
    Player_Turn,
    Enemy_Turn,
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

    ui::Shared_View hud;
    ui::Shared_View combat_grid;
    ui::Shared_View inventory_grid;

    int selected;
    bool clicked = false;

    Enemy_Instance& enemy;

    Fight_State fight_state;
    Fight_State from_anim;

    std::random_device random_device;

    inline Player& active_player() noexcept;
    inline Player const& active_player() const noexcept;

    void switch_to_inventory() noexcept;
    void switch_to_combat() noexcept;

    Labels_State labels_state;

    int cur_step = 0;
    int max_step = 0;
    int last_damage = 0;

    Entity_Health_Mediator player_health_mediator;
    Entity_Health_Mediator enemy_health_mediator;

    inline bool bars_animating() const noexcept;
  };

  inline Player& Combat_State::active_player() noexcept
  {
    return navigate.map.players[navigate.player];
  }
  inline Player const& Combat_State::active_player() const noexcept
  {
    return navigate.map.players[navigate.player];
  }

  inline bool Combat_State::bars_animating() const noexcept
  {
    return player_health_mediator.animating() ||
           enemy_health_mediator.animating();
  }

  void set_bar_to_life(ui::Bar& bar, Entity_Data& et) noexcept;

  void set_bar_animating(ui::Bar& bar, Entity_Data& et, int delta_life,
                         int step, int max_step) noexcept;
}
