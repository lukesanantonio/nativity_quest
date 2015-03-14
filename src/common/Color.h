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

  namespace colors
  {
    constexpr Color white {0xff, 0xff, 0xff};
    constexpr Color black {0x00, 0x00, 0x00};

    constexpr Color red   {0xff, 0x00, 0x00};
    constexpr Color green {0x00, 0xff, 0x00};
    constexpr Color blue  {0x00, 0x00, 0xff};
  }

  inline bool operator==(Color const& c1, Color const& c2) noexcept
  {
    return c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue;
  }
  inline bool operator!=(Color const& c1, Color const& c2) noexcept
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

  inline Color color_from_intensity(double r, double g, double b) noexcept
  {
    auto ret = Color{};
    ret.red = r * 0xff;
    ret.green = g * 0xff;
    ret.blue = b * 0xff;
    return ret;
  }
}
