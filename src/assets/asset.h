/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "rapidjson/document.h"

#define DECLARE_EXTENSION(ext_str) \
constexpr static char const* const ext = #ext_str;

struct SDL_Surface;

namespace game { namespace assets
{
  struct Asset
  {
    virtual ~Asset() noexcept {}

    std::string filename;
    std::string ext;
  };

  struct Image_Asset : public Asset
  {
    DECLARE_EXTENSION(png);

    SDL_Surface* image;
  };

  struct Json_Asset : public Asset
  {
    DECLARE_EXTENSION(json);

    rapidjson::Document json;
  };
} }
#undef DECLARE_EXTENSION
