/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>
namespace game
{
  struct Item_Impl;

  using Item = std::shared_ptr<const Item_Impl>;
  Item get_item(std::string const& item_name) noexcept;

  namespace no
  {
    const Item item;
  }
}
