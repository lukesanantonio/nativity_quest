/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "discard.h"
namespace game
{
  Discard_State::Discard_State(Game& g, Navigate_State& ns,
                               decl::Item item) noexcept
                               : Navigate_Sub_State(g, ns), item(item)
  {
    auto label_view = ui::Label_View{};

    auto& player = navigate.map.players[navigate.player];
    auto& inventory = player.inventory;

    label_view.labels.push_back(inventory[0]->str);
    label_view.labels.push_back(inventory[1]->str);
    label_view.labels.push_back(inventory[2]->str);
    label_view.labels.push_back(inventory[3]->str);
    label_view.labels.push_back(inventory[4]->str);
    label_view.labels.push_back(inventory[5]->str);
    label_view.labels.push_back("Discard");

    label_view.selected = 6;

    hud.elements.push_back({label_view, {}, ui::Side::Bottom});

    auto item_sprite = ui::Sprite{};
    item_sprite.src = navigate.map.items.get_spritesheet();
    item_sprite.scale = 1.0;

    item_sprite.vol = Volume<int>{{0, 0},
                                  navigate.map.items.get_sprite_extents().x,
                                  navigate.map.items.get_sprite_extents().y};

    auto alignment = ui::Alignment{};
    alignment.horizontal = ui::Horizontal_Alignment::Center;
    alignment.vertical = ui::Vertical_Alignment::Center;

    hud.elements.push_back({item_sprite, {}, alignment});
  }

  void Discard_State::handle_event(SDL_Event const&) noexcept
  {
  }
  void Discard_State::step() noexcept
  {
    game_.view.reset();
    game_.presenter.present(hud, game_.view, game_.graphics.size());

    // Get the currently selected menu item in the label view.
    auto& label_view = boost::get<ui::Label_View>(hud.elements[0].element);
    auto sel = label_view.selected;

    // Find that corrosponding item from the user's inventory.
    decl::Item sel_item;
    if(sel != 6)
    {
      sel_item = navigate.map.players[navigate.player].inventory[sel];
    }
    else
    {
      sel_item = item;
    }

    // Now find the sprite that we need to modify.
    auto& sprite_element = boost::get<ui::Sprite>(hud.elements[1].element);

    // Modify the image displayed using that item.
    auto& vol = sprite_element.vol.value();
    vol.pos.x = sel_item->sprite_pos.x * vol.width;
    vol.pos.y = sel_item->sprite_pos.y * vol.height;

    if(label_view.done)
    {
      if(sel != 6)
      {
        navigate.map.players[navigate.player].inventory[sel] = item;
      }
      pop_state(game_);
    }
  }
  void Discard_State::render() const noexcept
  {
    game_.view.render(game_.graphics);
  }

  void Discard_State::on_enter() noexcept
  {
    game_.presenter.handle_events(true);
  }
  void Discard_State::on_exit() noexcept
  {
    game_.presenter.handle_events(false);
  }
}
