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
    Discard_Item_Control(Player& p, Item_Parser const& items) noexcept
                         : player(p), items(items) {}


    void handle_event(SDL_Event const& event) noexcept;
    void layout(Label_View<Discard_Item_Control>& view,
                Graphics_Desc& g) noexcept;

    void pre_render(Graphics_Desc&, Label_View<Discard_Item_Control> const&,
                    Sprite_Container&) const noexcept {}

    void render(Graphics_Desc& g,
                Label_View<Discard_Item_Control> const& view,
                Sprite_Container& sprites) const noexcept;

    Player& player;
    Item_Parser const& items;

    Volume<int> item_volume;
    Item new_item;

    int selected = 0;
    bool done = false;
  };
}
