/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "inventory.h"
namespace game
{
  Inventory_View_State::Inventory_View_State(Game& g,
                                             Navigate_State& ns) noexcept
                                             : Navigate_Sub_State(g, ns)
  {
    auto label_view = ui::Label_View{};

    auto& player = navigate.map.players[navigate.player];
    auto& inventory = player.inventory;

    for(auto item : inventory)
    {
      if(item)
      {
        label_view.labels.push_back(item->str);
      }
      else
      {
        label_view.labels.push_back("No item");
      }
    }

    label_view.labels.push_back("Back");
    label_view.selected = 0;
    label_view.col = "black";

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

    alignment.horizontal = ui::Horizontal_Alignment::Left;
    alignment.vertical = ui::Vertical_Alignment::Bottom;

    auto bar = ui::Bar{player.entity_data.max_life,
                       player.entity_data.cur_life,
                       "black"};
    hud.elements.push_back({bar, {5, 0, 0, 5}, alignment});
  }

  void Inventory_View_State::handle_event(SDL_Event const&) noexcept {}
  void Inventory_View_State::step() noexcept
  {
    game_.view.reset();
    game_.presenter.present(hud, game_.view, game_.graphics.size());

    // Get the currently selected menu item in the label view.
    auto& label_view = boost::get<ui::Label_View>(hud.elements[0].element);

    // Find that corrosponding item from the user's inventory.
    auto sel = label_view.selected;
    if(sel < 6)
    {
      on_inventory_label_select();
    }
    else
    {
      on_extra_label_select();
    }

    if(label_view.done)
    {
      on_label_view_done();
    }
  }
  void Inventory_View_State::set_sprite_src(decl::Item item) noexcept
  {
    if(!item)
    {
      hud.elements[1].is_visible = false;
      return;
    }

    auto& sprite_element = boost::get<ui::Sprite>(hud.elements[1].element);

    // Modify the image displayed using that item.
    auto& vol = sprite_element.vol.value();
    vol.pos.x = item->sprite_pos.x * vol.width;
    vol.pos.y = item->sprite_pos.y * vol.height;

    hud.elements[1].is_visible = true;
  }
  void Inventory_View_State::on_label_view_done() noexcept
  {
    auto& label_view = boost::get<ui::Label_View>(hud.elements[0].element);
    auto sel = label_view.selected;

    if(sel != 6) label_view.done = false;
    else pop_state(game_);
  }
  void Inventory_View_State::on_inventory_label_select() noexcept
  {
    auto sel = boost::get<ui::Label_View>(hud.elements[0].element).selected;
    set_sprite_src(navigate.map.players[navigate.player].inventory[sel]);
  }
  void Inventory_View_State::on_extra_label_select() noexcept
  {
    set_sprite_src(decl::no::item);
  }

  void Inventory_View_State::render() const noexcept
  {
    game_.view.render(game_.graphics);
  }

  void Inventory_View_State::on_enter() noexcept
  {
    game_.presenter.handle_events(true);
  }
  void Inventory_View_State::on_exit() noexcept
  {
    game_.presenter.handle_events(false);
  }
}