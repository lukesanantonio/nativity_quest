/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "../describe.h"

#include "../../common/vec.h"
namespace game { namespace assets
{
  struct Game
  {
    using asset_t = Json_Asset;

    std::string title;

    Vec<int> size;

    bool fullscreen;
    bool vsync;
  };

  void describe(Asset_Ptr<Game> const& vec, Game& game) noexcept;
} }
