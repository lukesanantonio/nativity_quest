/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"
#include "Graphics_Desc.h"
#include "Label_View.h"
#include "Player.h"

#include "decl/enemies.h"

#include "Inventory_View_Control.h"
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

  enum class Combat_Anim_State
  {
    Player_Life,
    Enemy_Life,
    None
  };

  struct Combat_Control
  {
    Combat_Control(Player& player, Enemy_Instance& enemy) noexcept;

    void handle_event(SDL_Event const& event) noexcept;
    void layout(Label_View<Combat_Control>& view,
                Graphics_Desc& g) noexcept;
    void pre_render(Graphics_Desc& g,
                    Label_View<Combat_Control> const& view,
                    Sprite_Container& sprites) const noexcept {}
    void render(Graphics_Desc& g,
                Label_View<Combat_Control> const& view,
                Sprite_Container& sprites) const noexcept;

    int selected = 0;

    Player& player;
    Enemy_Instance& enemy;

    Fight_State state = Fight_State::Player_Turn;

    void step() noexcept;

    mutable Combat_Anim_State anim = Combat_Anim_State::None;
    mutable int last_damage;
    mutable int anim_step;

    bool in_inventory = false;
    mutable Label_View<Inventory_View_Control> inventory_view;
  };
}
