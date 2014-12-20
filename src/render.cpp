/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "render.h"
namespace game
{
  void render_as_minimap(Graphics_Desc& g, Sprite_Container& sprites,
                         Map const& map, Vec<int> const& start) noexcept
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

  void render_with_border(Graphics_Desc& g, SDL_Rect rect)
  {
    SDL_RenderFillRect(g.renderer, &rect);

    --rect.x;
    --rect.y;
    rect.w += 2;
    rect.h += 2;

    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderDrawRect(g.renderer, &rect);
  }

  bool render_health(Graphics_Desc& g, SDL_Rect box,
                     int life, int max, int change_size,
                     int life_step) noexcept
  {
    SDL_Rect bar;
    bar.w = box.w  - 50;
    bar.h = 15;
    bar.x = box.x + box.w / 2 - bar.w / 2;
    bar.y = box.y + box.h / 2 - bar.h / 2;

    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x77, 0x00, 0xff);
    render_with_border(g, bar);

    SDL_SetRenderDrawColor(g.renderer, 0x00, 0xff, 0x00, 0xff);

    bool ret = false;

    if(life_step == -1)
    {
      if(max != life) bar.w = bar.w / max * life;
    }
    else
    {
      auto segment_width = bar.w / max;

      bar.w = segment_width * (life + change_size);
      if(segment_width * change_size <= life_step)
      {
        bar.w -= segment_width * change_size;
        ret = true;
      }
      else bar.w -= life_step;
    }

    SDL_RenderFillRect(g.renderer, &bar);
    return ret;
  }
}
