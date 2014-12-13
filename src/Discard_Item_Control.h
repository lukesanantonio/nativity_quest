/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"
#include "Graphics_Desc.h"
#include "Label_View.h"
#include "Player.h"
namespace game
{
  struct Discard_Item_Control
  {
    Discard_Item_Control(Player& p) noexcept : player(p) {}

    void handle_event(SDL_Event const& event) noexcept;
    void layout(Label_View<Discard_Item_Control>& view) noexcept;
    void render(Graphics_Desc& g,
                Label_View<Discard_Item_Control> const& view) const noexcept;

    Player& player;

    int selected = 0;
    bool done = false;
  };
}
