/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include "asset.h"
namespace game { namespace assets
{
  template <class T> inline std::shared_ptr<T>
  find(assets::Vector assets, std::string name) noexcept
  {
    using std::begin; using std::end;
    auto asset_find = std::find_if(begin(assets), end(assets),
    [&](auto const& ptr)
    {
      return ptr->ext == T::ext && ptr->name == name;
    });

    if(asset_find == end(assets)) return nullptr;

    return std::dynamic_pointer_cast<T>(*asset_find);
  }
} }
