/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>
#include <vector>

#include "pong/common/vector.h"
namespace game
{
  struct Item_Impl
  {
    std::string str;
    pong::math::vector<int> sprite_position;
  };

  using Item = std::shared_ptr<const Item_Impl>;

  struct Item_Parser
  {
    Item_Parser(std::string const& filename);

    std::string get_spritesheet_filename() const noexcept;
    Item get_item(std::string const& str) const noexcept;

  private:
    std::string spritesheet_;
    std::vector<Item> items_;
  };

  namespace no
  {
    const Item item;
  }
}
