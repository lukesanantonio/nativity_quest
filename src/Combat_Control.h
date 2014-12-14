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
namespace game
{
  struct Combat_Control
  {
    Combat_Control(Enemy_Instance& enemy) noexcept : enemy(enemy) {}

    void handle_event(SDL_Event const& event) noexcept;
    void layout(Label_View<Combat_Control>& view,
                Graphics_Desc& g) noexcept;
    void render(Graphics_Desc& g,
                Label_View<Combat_Control> const& view,
                Sprite_Container& sprites) const noexcept;

    int selected = 0;
    bool run = false;

    Enemy_Instance& enemy;
  };
}

