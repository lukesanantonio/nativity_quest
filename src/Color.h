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

  void set_render_draw_color(SDL_Renderer* renderer, Color c) noexcept;
}
