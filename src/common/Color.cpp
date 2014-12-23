/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Color.h"
namespace game
{
  void set_render_draw_color(SDL_Renderer* renderer, Color c) noexcept
  {
    SDL_SetRenderDrawColor(renderer, c.red, c.green, c.blue, 0xff);
  }
}

