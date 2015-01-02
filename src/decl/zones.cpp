/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "zones.h"

#include <cstdint>
#include "SDL.h"

#include "../common/json.h"

namespace game { namespace decl
{
  Zone Zones::get_zone(Vec<int> pos) const noexcept
  {
    static std::vector<std::vector<short> > buttons;

    if(buttons.empty())
    {
        auto doc = parse_json("assets/zones.json");

        for(auto row_iter = doc.Begin(); row_iter != doc.End(); ++row_iter)
        {
            buttons.emplace_back();
            for(auto col_iter = row_iter->Begin();
                col_iter != row_iter->End();
                col_iter += 3)
            {
                buttons.back().push_back(col_iter->GetInt());
            }
        }
    }

    SDL_Surface* png = sprite_->surface();
    if(!png) return no::zone;

    // Bad position!
    if(pos.x < 0 || pos.y < 0) return no::zone;
    if(pos.x > png->w || pos.y > png->h) return no::zone;

    // Find the zone that is represented by the same color as our pixel.
    using std::begin; using std::end;
    auto zone_find = std::find_if(begin(zones_), end(zones_),
    [&pos](auto zone)
    {
      return buttons[pos.y][pos.x] == zone->color;
    });

    if(zone_find == end(zones_))
    {
      return no::zone;
    }

    return *zone_find;

  }
} }
