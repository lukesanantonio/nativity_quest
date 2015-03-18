/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>
#include <vector>
#include <random>

#include "../common/vec.h"
namespace game { namespace decl
{
  struct Item_Impl
  {
    std::string id;
    std::string str;
    Vec<int> sprite_pos;
  };

  using Item = std::shared_ptr<const Item_Impl>;

  struct Items
  {
    explicit Items(std::string const& filename);

    Item get_item(std::string const& str) const noexcept;

    inline std::string get_spritesheet() const noexcept
    { return spritesheet_; }
    inline Vec<int> get_sprite_extents() const noexcept
    { return sprite_extents_; }

  private:
    std::string spritesheet_;
    Vec<int> sprite_extents_;

    std::vector<Item> items_;

    mutable std::random_device r_device_;
  };

  namespace no
  {
    const Item item;
  }
} }
