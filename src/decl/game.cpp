/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "game.h"
#include "../common/json.h"
namespace game { namespace decl
{
  Game::Game(std::string json) noexcept
  {
    auto doc = parse_json(json);

    size.x = doc["size"]["width"].GetInt();
    size.y = doc["size"]["height"].GetInt();

    fullscreen = doc["fullscreen"].GetBool();
  }
} }
