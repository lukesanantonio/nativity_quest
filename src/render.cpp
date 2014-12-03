/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "render.h"
namespace game
{
  void render_as_minimap(Graphics_Desc& g, Sprite_Container& sprites,
                         Map const& map,
                         pong::math::vector<int> const& start) noexcept
  {
    // Grab the starting coordinates.
    SDL_Rect dest;
    dest.x = start.x;
    dest.y = start.y;

    // Scale the map image appropiately.
    Sprite map_spr = sprites.get_sprite(map.zones.map_asset());
    dest.w = map_spr->surface()->w * map.mini_scale;
    dest.h = map_spr->surface()->h * map.mini_scale;

    // Render the mini map.
    SDL_RenderCopy(g.renderer, map_spr->texture(g.renderer), NULL, &dest);

    // Render a single-pixel, black border around that mini map.
    --dest.x;
    --dest.y;
    ++dest.w;
    ++dest.h;
    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderDrawRect(g.renderer, &dest);
  }
}
