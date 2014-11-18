/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "zones.h"

#include <cstdint>

#include "../util/surface.h"

#include "SDL.h"
#include "SDL_image.h"

#include "impl/zone_parse.h"
#include "impl/zone_impl.h"

namespace game
{
  SDL_Surface* get_zone_png() noexcept
  {
    static Surface_Ptr zone_file = nullptr;
    if(!zone_file)
    {
      zone_file = Surface_Ptr(IMG_Load("assets/zones.png"));
    }
    return zone_file.get();
  }

  Zone get_zone(Vec<int> pos)
  {
    // TODO handle exceptions.
    static const auto zones = Zone_Parser{"assets/zones.json"};

    SDL_Surface* zone_file = get_zone_png();
    if(!zone_file)
    {
      return no::zone;
    }

    // pixel represents the red component of the color at pixel pos.
    uint8_t* pixel = (uint8_t*) zone_file->pixels;
    pixel += (pos.y * zone_file->pitch) +
             (pos.x * zone_file->format->BytesPerPixel);
    if(!pixel) return no::zone;

    // Find the zone that is represented by the same color as our pixel.
    using std::begin; using std::end;
    auto zone_find = std::find_if(begin(zones.zones), end(zones.zones),
    [&pixel](auto zone)
    {
      return *pixel == zone->color;
    });

    if(zone_find == end(zones.zones))
    {
      return no::zone;
    }

    return *zone_find;
  }
}
