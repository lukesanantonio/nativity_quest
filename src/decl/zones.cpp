/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "zones.h"

#include <cstdint>

#include "rapidjson/document.h"

#include "../util/except.h"
#include "../util/json.h"
#include "../util/surface.h"

#include "SDL.h"
#include "SDL_image.h"

#include <boost/optional.hpp>

namespace game
{
  namespace
  {
    struct Zone_Class
    {
      std::string str;
      boost::optional<Item> required_item;
      boost::optional<short> speed_cost;
      boost::optional<bool> important;
      boost::optional<double> spawn_mod;
    };

    Zone_Class parse_class(Item_Parser const& items,
                           rapidjson::Value const& val) noexcept
    {
      auto c = Zone_Class{};

      if_has_member(val, "str", [&c](auto const& val)
      {
        c.str.assign(val.GetString(), val.GetStringLength());
      });

      if_has_member(val, "required_item", [&](auto const& val)
      {
        auto item_str = std::string{val.GetString(), val.GetStringLength()};
        c.required_item = items.get_item(item_str);
      });

      if_has_member(val, "speed_cost", [&c](auto const& val)
      {
        c.speed_cost = val.GetInt();
      });

      if_has_member(val, "important", [&c](auto const& val)
      {
        c.important = val.GetBool();
      });

      if_has_member(val, "item_spawn_modifier", [&c](auto const& val)
      {
        c.spawn_mod = val.GetDouble();
      });

      return c;
    }

    void apply_class(Zone_Class const& cl, Zone_Impl& zone) noexcept
    {
      if(cl.required_item) zone.required_item = cl.required_item.value();
      if(cl.speed_cost) zone.speed_cost = cl.speed_cost.value();
      if(cl.important) zone.important = cl.important.value();
      if(cl.spawn_mod) zone.item_spawn_modifier = cl.spawn_mod.value();
    }
  }
  Zone_Parser::Zone_Parser(std::string const& json, Item_Parser const& items)
  {
    auto d = parse_json(json);

    if(!has_json_members(d, {"map", "decl", "classes", "zones"}))
    {
      throw Bad_Asset{json,
                      "Missing 'map', 'decl', 'classes' or 'zones' member"
                      " (or a combonation of these)"};
    }


    map_asset_.assign(d["map"].GetString());

    std::string zones_file(d["decl"].GetString());
    zone_decl_.reset(IMG_Load(zones_file.c_str()));
    if(!zone_decl_)
    {
      throw Invalid_Decl_Filename{zones_file};
    }

    auto zone_classes = std::vector<Zone_Class>{};
    for(auto iter = d["classes"].Begin(); iter < d["classes"].End(); ++iter)
    {
      zone_classes.push_back(parse_class(items, *iter));
    }

    for(auto iter = d["zones"].Begin(); iter < d["zones"].End(); ++iter)
    {
      if(!has_json_members(*iter, {"str", "color"}))
      {
        throw Bad_Asset{json, "Bad zone asset"};
      }

      // Set defaults
      auto zone = Zone_Impl{"Unknown", no::item, 0, false, 0, 0.0};

      // Set the str.
      zone.str.assign((*iter)["str"].GetString(),
                      (*iter)["str"].GetStringLength());

      // Set the color
      zone.color = (*iter)["color"].GetInt();

      // Apply the class.
      if(iter->HasMember("class"))
      {
        // Stringify the c-style string from the classname in the zone object.
        const auto class_reference_str =
          std::string{(*iter)["class"].GetString(),
                      (*iter)["class"].GetStringLength()};

        // Find that in our vector of available zone classes.
        using std::begin; using std::end;
        auto class_find = std::find_if(begin(zone_classes), end(zone_classes),
        [&class_reference_str](auto const& zone_class)
        {
          return class_reference_str == zone_class.str;
        });

        // Do we not have that class?
        if(class_find == end(zone_classes))
        {
          throw Invalid_Class_Reference{class_reference_str};
        }
        apply_class(*class_find, zone);
      }

      // Push the zone.
      zones_.push_back(Zone{new Zone_Impl(zone)});
    }
  }

  Zone Zone_Parser::get_zone(Vec<int> pos) const noexcept
  {
    SDL_Surface* png = zone_decl_.get();
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

    return *zone_find;
  }
}
