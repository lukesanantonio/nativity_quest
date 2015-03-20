/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "zones.h"

#include <cstdint>
#include "SDL.h"

namespace game { namespace decl
{
  Zone Zones::get_zone(Vec<int> pos) const noexcept
  {
    SDL_Surface* sprite = sprite_->image;
    if(!sprite) return no::zone;

    // Convert the png to a format we can use.
    SDL_Surface* png = SDL_ConvertSurfaceFormat(sprite, SDL_PIXELFORMAT_RGB888,
                                                0);
    if(!png) return no::zone;

    // Bad position!
    if(pos.x < 0 || pos.y < 0) return no::zone;
    if(pos.x > png->w || pos.y > png->h) return no::zone;

    // pixel represents the red component of the color at pixel pos.
    uint8_t* pixel = (uint8_t*) png->pixels;
    pixel += (pos.y * png->pitch) +
             (pos.x * png->format->BytesPerPixel);
    if(!pixel) return no::zone;

    // Find the zone that is represented by the same color as our pixel.
    using std::begin; using std::end;
    auto zone_find = std::find_if(begin(zones_), end(zones_),
    [&pixel](auto zone)
    {
      return *pixel == zone->color;
    });

    if(zone_find == end(zones_))
    {
      return no::zone;
    }

    SDL_FreeSurface(png);

    return *zone_find;
  }
} }
