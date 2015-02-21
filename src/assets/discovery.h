/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <vector>
#include <string>
namespace game { namespace assets
{
  auto discover(std::string subdir) noexcept -> std::vector<std::string>;
} }
