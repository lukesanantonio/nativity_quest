/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "render.h"
namespace game
{
  void draw_point(SDL_Renderer* r, Vec<int> p, Color c) noexcept
  {
    set_render_draw_color(r, c);
    SDL_RenderDrawPoint(r, p.x, p.y);
  }
  void fill_volume(SDL_Renderer* r, Volume<int> v, Color c) noexcept
  {
    set_render_draw_color(r, c);

    auto sdl_rect = to_sdl_rect(v);
    SDL_RenderFillRect(r, &sdl_rect);
  }
  void draw_volume(SDL_Renderer* r, Volume<int> v, Color c) noexcept
  {
    set_render_draw_color(r, c);

    auto sdl_rect = to_sdl_rect(v);
    SDL_RenderDrawRect(r, &sdl_rect);
  }
}
