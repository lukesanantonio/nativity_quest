/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include "SDL.h"

#include "pong/render/text.h"

#include "pong/common/vector.h"
namespace game
{
  struct Bad_Window {};
  struct Bad_Renderer {};

  struct Graphics_Desc
  {
    Graphics_Desc(std::string const& title, pong::math::vector<int> extents,
                  std::string const& font_file, pong::Logger* log);
    ~Graphics_Desc() noexcept;

    int get_width() const noexcept;
    int get_height() const noexcept;

    SDL_Window* window;
    SDL_Renderer* renderer;

    struct
    {
      pong::text::Face face;
      pong::text::AntiAliasedRaster raster;
    } font;
  private:
    pong::math::vector<int> extents_;
  };
}
