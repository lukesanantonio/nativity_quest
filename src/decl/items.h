/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>
#include <vector>

#include "../common/vec.h"
namespace game
{
  struct Item_Impl
  {
    std::string str;
    Vec<int> pos;
  };

  using Item = std::shared_ptr<const Item_Impl>;

  struct Item_Parser
  {
    explicit Item_Parser(std::string const& filename);

    Item get_item(std::string const& str) const noexcept;

    inline std::string get_spritesheet() const noexcept;
    inline Vec<int> get_sprite_extents() const noexcept;

  private:
    std::string spritesheet_;

    Vec<int> sprite_extents_;

    std::vector<Item> items_;
  };

  inline std::string Item_Parser::get_spritesheet() const noexcept
  {
    return this->spritesheet_;
  }
  inline Vec<int> Item_Parser::get_sprite_extents() const noexcept
  {
    return this->sprite_extents_;
  }

  namespace no
  {
    const Item item;
  }
}
