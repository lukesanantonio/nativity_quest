/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "discard.h"
namespace game
{
  Discard_State::Discard_State(Game& g, Navigate_State& ns,
                               assets::Item item) noexcept
                               : Inventory_View_State(g, ns), extra_item(item)
  {
    auto label = hud->find_child_r<ui::Label>("back");

    label->str_name("discard_item");
  }

  void Discard_State::on_inventory_label_select() noexcept
  {
    set_sprite_src(navigate.map.players[navigate.player].inventory[selected]);
    if(clicked) on_label_view_done();
  }

  void Discard_State::on_label_view_done() noexcept
  {
    if(selected < 6)
    {
      if(selected < 5)
      {
        // Replace the item in the user's inventory.
        auto& item = navigate.map.players[navigate.player].inventory[selected];
        item = extra_item;
      }
      else
      {
        // Cannot replace the last item in the player's inventory.
        clicked = false;
        return;
      }
    }
    pop_state(game_);
  }
  void Discard_State::on_extra_label_select() noexcept
  {
    set_sprite_src(extra_item);
    if(clicked) on_label_view_done();
  }
}
