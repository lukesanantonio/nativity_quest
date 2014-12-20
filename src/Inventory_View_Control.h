/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"
#include "Label_View.h"
#include "Graphics_Desc.h"
namespace game
{
  struct Player;

  struct Inventory_View_Control
  {
    void handle_event(SDL_Event const& event) noexcept;
    void layout(Label_View<Inventory_View_Control>& view,
                Graphics_Desc& g) noexcept;
    void pre_render(Graphics_Desc&, Label_View<Inventory_View_Control> const&,
                    Sprite_Container&) const noexcept {}
    void render(Graphics_Desc&, Label_View<Inventory_View_Control> const&,
                Sprite_Container&) const noexcept;

    int selected = 0;
    bool enter = false;

    Player* health_player = nullptr;
  };
}
