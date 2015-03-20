/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../describe.h"
#include <unordered_map>
namespace game { namespace assets
{
  struct Lang
  {
    using asset_t = Json_Asset;

    std::unordered_map<std::string, std::string> dict;
  };

  void describe(Asset_Ptr<Lang> const&, Lang&, assets::Vector&) noexcept;
} }
