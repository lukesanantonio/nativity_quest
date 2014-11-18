/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "zone_parse.h"
#include "zone_impl.h"

#include "zone_class.h"

#include "rapidjson/document.h"

#include "../../util/except.h"
#include "../../util/json.h"

#include "../items.h"

namespace game
{
  Zone_Parser::Zone_Parser(std::string const& filename)
  {
    auto d = parse_json(filename);

    if(!has_json_members(d, {"classes", "zones"}))
    {
      throw Bad_Asset{filename,
                      "Missing 'classes' or 'zones' member (or both)"};
    }

    auto zone_classes = std::vector<Zone_Class>{};
    for(auto iter = d["classes"].Begin(); iter < d["classes"].End(); ++iter)
    {
      zone_classes.push_back(parse_class(*iter));
    }

    for(auto iter = d["zones"].Begin(); iter < d["zones"].End(); ++iter)
    {
      if(!has_json_members(d, {"str", "color", "class"}))
      {
        throw Bad_Asset{filename, "Bad zone asset"};
      }

      // Set defaults
      auto zone = Zone_Impl{"Unknown", no::item, 0, false, 0};

      // Set the str.
      zone.str.assign((*iter)["str"].GetString(),
                      (*iter)["str"].GetStringLength());

      // Set the color
      zone.color = (*iter)["color"].GetInt();

      // Apply the class.
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
      zones.push_back(Zone{new Zone_Impl(zone)});
    }
  }
}
