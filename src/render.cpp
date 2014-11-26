/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "render.h"
namespace game
{
  void render_as_minimap(Graphics_Desc& g,
                         Map const& map,
                         Volume<> vol) noexcept
  {
    SDL_Rect dest;
    dest.x = vol.pos.x;
    dest.y = vol.pos.y;
    dest.w = vol.width;
    dest.h = vol.height;

    SDL_RenderCopy(g.renderer, map.img.texture(g.renderer), NULL, &dest);

    --dest.x;
    --dest.y;
    ++dest.w;
    ++dest.h;
    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderDrawRect(g.renderer, &dest);
  }
  void render_viewport(Graphics_Desc& g, Map const& map, Volume<> viewport,
                       Volume<> destination) noexcept
  {
    SDL_Rect src;
    src.x = viewport.pos.x;
    src.y = viewport.pos.y;
    src.w = viewport.width;
    src.h = viewport.height;

    SDL_Rect dest;
    dest.x = destination.pos.x;
    dest.y = destination.pos.y;
    dest.w = destination.width;
    dest.h = destination.height;

    SDL_RenderCopy(g.renderer, map.img.texture(g.renderer), &src, &dest);
  }
  void use_viewport_scale(SDL_Rect& rect,
                          double xscale, double yscale) noexcept
  {
    rect.x *= xscale;
    rect.y *= yscale;
  }
}
