/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "game.h"
namespace game { namespace assets
{
  void describe(Asset_Ptr<Game> const& asset, Game& game,
                assets::Vector&) noexcept
  {
    auto const& doc = asset->json;

    game.title = doc["title"].GetString();

    game.size.x = doc["size"]["width"].GetInt();
    game.size.y = doc["size"]["height"].GetInt();

    game.fullscreen = doc["fullscreen"].GetBool();
    game.vsync = doc["vsync"].GetBool();
  }
} }
