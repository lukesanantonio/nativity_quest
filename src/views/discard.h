/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "inventory.h"
#include "../decl/items.h"
namespace game
{
  struct Discard_State : public Inventory_View_State
  {
    Discard_State(Game& g, Navigate_State& ns, decl::Item item) noexcept;

    void on_label_view_done() noexcept override;
    void on_extra_label_select() noexcept override;

    decl::Item extra_item;
  };
}
