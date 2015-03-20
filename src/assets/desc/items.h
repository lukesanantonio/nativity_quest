/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>
#include <vector>
#include <random>

#include "../describe.h"

#include "../../common/vec.h"
namespace game { namespace assets
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
    using asset_t = Json_Asset;

    std::shared_ptr<Image_Asset> spritesheet;
    Vec<int> sprite_extents;

    Item get_item(std::string const& str) const noexcept;
    std::vector<Item> items;
  };

  namespace no
  {
    const Item item;
  }

  void describe(Asset_Ptr<Items> const&, Items&, assets::Vector&) noexcept;
} }
