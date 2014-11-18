/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
namespace game
{
  struct Bad_File
  {
    std::string filename;
  };
  struct Bad_Asset
  {
    std::string filename;
    std::string diagnostic;
  };
}
