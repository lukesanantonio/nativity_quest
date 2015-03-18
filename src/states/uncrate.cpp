/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "uncrate.h"
#include "../decl/items.h"
#include "discard.h"
#include "../common/log.h"

#define HUD_JSON "ui/uncrate"
#define ITEMS_IMAGE "images/items"

namespace game
{
  constexpr int anim_size = 4;
  constexpr char const* const anim_sprites[] =
  {
    "anim/chest/0",
    "anim/chest/1",
    "anim/chest/2",
    "anim/chest/3",
  };

  template <typename C>
  auto find_inventory_space(C& container) noexcept -> decltype(auto)
  {
    using std::begin; using std::end;
    auto no_item_find = std::find_if(begin(container), end(container),
    [](auto const& i)
    {
      return i == decl::no::item;
    });

    return no_item_find;
  }

  Uncrate_State::Uncrate_State(Game& game, Navigate_State& ns,
                               Chest& chest) noexcept
                               : Navigate_Sub_State(game, ns), chest(chest),
                                 hud{ui::load(game, HUD_JSON)},
                                 anim{45, 5}
  {
    using namespace std::placeholders;
    anim.set_segment_fn(std::bind(&Uncrate_State::on_anim_segment, this, _1));
  }

  void Uncrate_State::handle_event(SDL_Event const& event) noexcept
  {
    hud->dispatch_event(event);
  }

  void Uncrate_State::step() noexcept
  {
    anim.step();

    if(anim.done() && user_continue)
    {
      // Disable the chest still on the ground.
      chest.visible = false;

      // Now change the users inventory.
      Player& player = navigate.map.players[navigate.player];
      auto inventory = find_inventory_space(player.inventory);
      if(inventory != std::end(player.inventory))
      {
        // Inventory not full!
        *inventory = chest.item;

        // Leave.
        pop_state(game_);
      }
      else
      {
        replace_state(game_, std::make_shared<Discard_State>(game_, navigate,
                      chest.item));
      }
    }
  }
  void Uncrate_State::render() const noexcept
  {
    hud->render();
  }
  void Uncrate_State::on_exit() noexcept
  {
    // In case the item made the player have more view radius (ie a torch).
    unfog(navigate.active_player(), navigate.effects);
  }

  void Uncrate_State::on_anim_segment(int segment) noexcept
  {
    // Set the chest animation.
    auto sprite = hud->find_child_r<ui::Sprite>("box_sprite");
    auto label = hud->find_child_r<ui::Label>("found_msg");
    if(segment < 4)
    {
      // New animation frame.

      // Set the sprite to the correct frame from the chest-opening animation.
      auto asset =
             get_asset<assets::Image_Asset>(game_, anim_sprites[segment]);
      sprite->src(asset);

      // Don't reveal the item yet.
      label->str_args(std::string{"..."});
    }
    else if(segment == 4)
    {
      sprite->scale(sprite->scale() * .25);

      // Load the items image into the sprite.
      sprite->src(get_asset<assets::Image_Asset>(game_, ITEMS_IMAGE));

      // Then set the src rect. TODO this is copied from inventory.cpp
      // find a way to share this code.
      auto sprite_extents = navigate.map.items.get_sprite_extents();
      auto src_rect = vol_from_extents(sprite_extents);
      src_rect.pos.x = chest.item->sprite_pos.x * src_rect.width;
      src_rect.pos.y = chest.item->sprite_pos.y * src_rect.height;
      sprite->set_src_rect(src_rect);

      // TODO Make this automatic somehow!
      label->str_args(t(chest.item->str));

      // Make the continue button work.
      hud->find_child_r("continue_button")->add_event_trigger<ui::Mouse_Click>(
      [&](auto const&)
      {
        user_continue = true;
      }, nullptr, true);
    }

    hud->layout(game_.graphics.size());
  }
}
