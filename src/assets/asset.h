/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../common/surface.h"

#include "rapidjson/document.h"

#define DECLARE_EXTENSION(ext_str) \
constexpr static char const* const ext = #ext_str;

#include "SDL.h"

namespace game { namespace assets
{
  struct Asset
  {
    virtual ~Asset() noexcept {}

    std::string name;
    std::string ext;
  };

  struct Image_Asset : public Asset
  {
    DECLARE_EXTENSION(png);

    Drawable_Surface image;
  };

  struct Json_Asset : public Asset
  {
    DECLARE_EXTENSION(json);

    rapidjson::Document json;
  };

  struct Font_Asset : public Asset
  {
    DECLARE_EXTENSION(ttf);

    std::string abs_path;
  };

  using Vector = std::vector<std::shared_ptr<Asset> >;
} }
#undef DECLARE_EXTENSION
