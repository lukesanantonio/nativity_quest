/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "render.h"
namespace game
{
#if 0
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
  void render_player_minimap(Graphics_Desc& g, Map const& map,
                             Volume<> map_vol, Player& player,
                             short player_radius) noexcept
  {
    SDL_Rect rect;
    rect.w = player_radius;
    rect.h = player_radius;
    rect.x = std::max(0, player.pos.x - player_radius / 2);
    rect.y = std::max(0, player.pos.y - player_radius / 2);

    use_viewport_scale(rect, map_vol.width / (double) map.img.surface()->w,
                             map_vol.height / (double) map.img.surface()->h);

    rect.x += map_vol.pos.x;
    rect.y += map_vol.pos.y;

    SDL_RenderFillRect(g.renderer, &rect);
  }
#endif
}
