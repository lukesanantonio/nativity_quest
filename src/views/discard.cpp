/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "discard.h"
namespace game
{
  Discard_State::Discard_State(Game& g, Navigate_State& ns,
                               decl::Item item) noexcept
                               : Inventory_View_State(g, ns), extra_item(item)
  {
    auto& label_view = boost::get<ui::Label_View>(hud.elements[0].element);

    label_view.labels[6] = "Discard";
    label_view.selected = 6;

    hud.elements.erase(hud.elements.end() - 1);
  }

  void Discard_State::on_inventory_label_select() noexcept
  {
    auto sel = boost::get<ui::Label_View>(hud.elements[0].element).selected;
    set_sprite_src(navigate.map.players[navigate.player].inventory[sel]);
  }

  void Discard_State::on_label_view_done() noexcept
  {
    auto sel = boost::get<ui::Label_View>(hud.elements[0].element).selected;

    if(sel != 6)
    {
      navigate.map.players[navigate.player].inventory[sel] = extra_item;
    }
    pop_state(game_);
  }
  void Discard_State::on_extra_label_select() noexcept
  {
    set_sprite_src(extra_item);
  }
}
