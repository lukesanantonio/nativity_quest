/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "inventory.h"
#include "../assets/desc/items.h"
namespace game
{
  struct Discard_State : public Inventory_View_State
  {
    Discard_State(Game& g, Navigate_State& ns, assets::Item item) noexcept;

    void on_inventory_label_select() noexcept override;
    void on_label_view_done() noexcept;
    void on_extra_label_select() noexcept override;

    assets::Item extra_item;
  };
}
