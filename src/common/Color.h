/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"
namespace game
{
  struct Color
  {
    using component_t = unsigned char;
    component_t red;
    component_t green;
    component_t blue;
  };

  bool operator==(Color const& c1, Color const& c2) noexcept
  {
    return c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue;
  }
  bool operator!=(Color const& c1, Color const& c2) noexcept
  {
    return !(c1 == c2);
  }

  void set_render_draw_color(SDL_Renderer* renderer, Color c) noexcept;

  inline SDL_Color to_sdl_color(Color c) noexcept
  {
    auto sc = SDL_Color{};

    sc.r = c.red;
    sc.g = c.green;
    sc.b = c.blue;
    sc.a = 0xff;

    return sc;
  }
}
