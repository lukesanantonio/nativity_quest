/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "inventory.h"
//#include "combat.h"

#define INVENTORY_VIEW_HUD "ui/inventory"

namespace game
{
  constexpr int ids_size = 7;
  constexpr char const* const ids[] =
  {
    "item0",
    "item1",
    "item2",
    "item3",
    "item4",
    "item5",
    "back"
  };

  Inventory_View_State::
  Inventory_View_State(Game& g, Navigate_State& ns) noexcept
                       : Navigate_Sub_State(g, ns),
                         hud(ui::load(g, INVENTORY_VIEW_HUD))
  {
    // Update all the labels with the users inventory.
    update_labels();

    // Lay out the ui.
    hud->layout(game_.graphics.size());

    auto spritesheet =
             get_asset<assets::Image_Asset>(g, ns.map.items.get_spritesheet());
    auto hud_sprite = hud->find_child_r<ui::Sprite>("item_sprite");

    // Set the sprite up correctly. Set it to hidden initialially.
    hud_sprite->src(spritesheet);

    auto set_selected = [this](int index, auto const&)
    {
      selected = index;
    };

    // Have each label tell us when it's selected. The call to bind binds it's
    // index to it so it nows what to advertise to us when the time comes.
    selected = 6;
    for(std::size_t i = 0; i < ids_size; ++i)
    {
      auto item = hud->find_child_r(ids[i]);
      item->add_event_trigger<ui::Mouse_Hover>(
        std::bind(set_selected, i, std::placeholders::_1), true);

      item->add_event_trigger<ui::Mouse_Click>(
      [&](auto const&)
      {
        clicked = true;
      }, true);
    }

    mediator_.entity_data(&ns.active_player().entity_data);
    mediator_.bar(hud->find_child_r<ui::Bar>("player_health").get());
  }

  void Inventory_View_State::handle_event(SDL_Event const& e) noexcept
  {
    hud->dispatch_event(e);
  }
  void Inventory_View_State::step() noexcept
  {
    // Keep the health bar up to date.
    mediator_.step();

    // Set the border around the selected item, removing borders around
    // unselected label views.
    for(int i = 0; i < ids_size; ++i)
    {
      auto child = hud->find_child_r(ids[i]);
      if(selected == i)
      {
        child->set_border(ui::View_Volume::Parent, {0x00, 0x00, 0x00});
      }
      else
      {
        child->remove_border(ui::View_Volume::Parent);
      }
    }

#if 0
    if(anim)
    {
      if(cur_step++ == max_step)
      {
        anim = false;
      }

      set_bar_animating(bar, navigate.active_player().entity_data, delta,
                        cur_step, max_step);
    }
    else
    {
      set_bar_to_life(bar, navigate.active_player().entity_data);
    }
#endif

    // Get the currently selected menu item in the label view.
    // Find that corrosponding item from the user's inventory.
    if(selected < 6)
    {
      on_inventory_label_select();
    }
    else
    {
      on_extra_label_select();
    }
  }
  void Inventory_View_State::set_sprite_src(decl::Item item) noexcept
  {
    auto sprite = hud->find_child_r<ui::Sprite>("item_sprite");
    if(item)
    {
      sprite->visible(true);
    }
    else
    {
      sprite->visible(false);
      return;
    }

    // Just change the src rect and relayout.
    auto sprite_extents = navigate.map.items.get_sprite_extents();
    auto src_rect = vol_from_extents(sprite_extents);
    src_rect.pos.x = item->sprite_pos.x * src_rect.width;
    src_rect.pos.y = item->sprite_pos.y * src_rect.height;
    sprite->set_src_rect(src_rect);

    hud->layout(game_.graphics.size());
  }
  void Inventory_View_State::on_inventory_label_select() noexcept
  {
    auto& player = navigate.map.players[navigate.player];
    auto item = player.inventory[selected];
    set_sprite_src(item);

    auto& effects = navigate.effects;

    // Use the item if possible.
    if(clicked && effects.used_in_navigation(item))
    {
      auto et = player.entity_data;
      if(effects.apply_effect(player, item))
      {
        // Remove the item from the users inventory.
        player.inventory[selected] = decl::no::item;
        // Update the hud.
        update_labels();

        // Animate the life (if it has been changed).
        if(player.entity_data.cur_life != et.cur_life)
        {
          // Animate this change
          anim = true;
          delta = et.cur_life - player.entity_data.cur_life;
          cur_step = 0;
          max_step = std::abs(delta) * 15;
        }
      }
    }

    clicked = false;
  }
  void Inventory_View_State::on_extra_label_select() noexcept
  {
    set_sprite_src(decl::no::item);
    if(clicked) pop_state(game_);
  }

  void Inventory_View_State::render() const noexcept
  {
    hud->render();
  }

  void Inventory_View_State::update_labels() noexcept
  {
    auto& invent = navigate.active_player().inventory;

    for(int i = 0; i < static_cast<int>(invent.size()); ++i)
    {
      using namespace std::literals;
      auto ui_id = "item"s + std::to_string(i);

      auto item = invent[i];

      auto item_str = item ? item->str : "No item";
      hud->find_child_r<ui::Label>(ui_id)->str(item_str);
    }
  }
}
