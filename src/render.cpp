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

    SDL_RenderCopy(g.renderer, map.texture(g.renderer), NULL, &dest);

    --dest.x;
    --dest.y;
    ++dest.w;
    ++dest.h;
    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderDrawRect(g.renderer, &dest);
  }
}
