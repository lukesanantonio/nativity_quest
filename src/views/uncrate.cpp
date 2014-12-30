/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "uncrate.h"
#include "../decl/items.h"
namespace game
{
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
  void Uncrate_State::step() noexcept
  {
    // TODO Get these constants from json or something.
    constexpr auto max_frame = 4;
    constexpr auto frames_between = 45;
    constexpr auto frames_end = 90;

    if(anim_frame == max_frame)
    {
      if(++intermediate_counter == frames_end)
      {
        chest.visible = false;

        Player& player = navigate.map.players[navigate.player];
        auto inventory = find_inventory_space(player.inventory);
        if(inventory != std::end(player.inventory))
        {
          // Inventory not full!
          *inventory = chest.item;
        }
        else
        {
#if 0
          auto& player = turn.map.players[turn.player];
          Discard_Item_Data discard_data{player, turn.items};

          discard_data.label_view.control().new_item = data.chest.item;
          discard_data.after_state = Waiting_Data{};

          for(int i = 0; i < player.inventory.size(); ++i)
          {
            discard_data.label_view.add_label(player.inventory[i]->str);
          }

          discard_data.label_view.add_label("Discard");

          for(auto& label : discard_data.label_view.labels())
          {
            label.text_height(25);
            label.color({0x00, 0x00, 0x00, 0xff});
          }
#endif
        }

        pop_state(game_);
      }
    }
    else if(++intermediate_counter == frames_between)
    {
      ++anim_frame;
      intermediate_counter = 0;
    }
    else
    {
      ++intermediate_counter;
    }
  }
  void Uncrate_State::render() const noexcept
  {
    auto& sprites = navigate.sprites;

    // Render the large crate in the center of the screen.
    auto fr = anim_frame < 4 ? anim_frame : 3;
    decl::Sprite chest_spr = sprites.get_sprite(navigate.map.chest_sprite, fr);

    // Center the large chest.
    SDL_Rect chest_dest;
    chest_dest.h = game_.graphics.size().y / 2;
    chest_dest.w = chest_dest.h;
    chest_dest.x = game_.graphics.size().x / 2 - chest_dest.w / 2;
    chest_dest.y = game_.graphics.size().y / 2 - chest_dest.h / 2;
    SDL_RenderCopy(game_.graphics.renderer,
                   chest_spr->texture(game_.graphics.renderer),
                   NULL, &chest_dest);

    if(anim_frame == 4)
    {
      // Render the item atop everything just about as big.
      auto& items = navigate.map.items;
      decl::Sprite item_sprites = sprites.get_sprite(items.get_spritesheet());

      SDL_Rect item_src;
      item_src.w = items.get_sprite_extents().x;
      item_src.h = items.get_sprite_extents().y;

      decl::Item uncovered_item = chest.item;
      item_src.x = uncovered_item->sprite_pos.x * item_src.w;
      item_src.y = uncovered_item->sprite_pos.y * item_src.h;

      SDL_RenderCopy(game_.graphics.renderer,
                     item_sprites->texture(game_.graphics.renderer),
                     &item_src, &chest_dest);
    }
  }
}
