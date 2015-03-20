/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>

#include "../common/vec.h"
#include "../common/except.h"
#include "../common/json.h"

#include "rapidjson/document.h"

#include "items.h"
#include "../assets/assets.h"

#include <boost/optional.hpp>
namespace game { namespace decl
{
  struct Invalid_Decl_Filename
  {
    std::string filename;
  };

  struct Invalid_Class_Reference
  {
    std::string class_str;
  };

  struct Zone_Impl
  {
    std::string str;
    Item required_item;
    short speed_cost;
    bool important;
    short color;
  };

  using Zone = std::shared_ptr<const Zone_Impl>;

  namespace no
  {
    const Zone zone;
  }

  struct Zones
  {
    template <class Doc>
    void initialize_zones(Items const& items, Doc const& classes,
                          Doc const& zones) noexcept;

    inline std::shared_ptr<assets::Image_Asset> sprite() const noexcept
    { return sprite_; }
    inline void sprite(std::shared_ptr<assets::Image_Asset> sprite) noexcept
    { sprite_ = sprite; }

    Zone get_zone(Vec<int> pos) const noexcept;
  private:
    std::vector<Zone> zones_;
    std::shared_ptr<assets::Image_Asset> sprite_;
  };

  namespace
  {
    struct Zone_Class
    {
      std::string str;
      boost::optional<Item> required_item;
      boost::optional<short> speed_cost;
      boost::optional<bool> important;
    };

    Zone_Class parse_class(Items const& items,
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

      return c;
    }

    void apply_class(Zone_Class const& cl, Zone_Impl& zone) noexcept
    {
      if(cl.required_item) zone.required_item = cl.required_item.value();
      if(cl.speed_cost) zone.speed_cost = cl.speed_cost.value();
      if(cl.important) zone.important = cl.important.value();
    }
  }

  template <class Doc>
  void Zones::initialize_zones(Items const& items,
                               Doc const& classes, Doc const& zones) noexcept
  {
    auto zone_classes = std::vector<Zone_Class>{};
    for(auto iter = classes.Begin(); iter < classes.End(); ++iter)
    {
      zone_classes.push_back(parse_class(items, *iter));
    }

    for(auto iter = zones.Begin(); iter < zones.End(); ++iter)
    {
      if(!has_json_members(*iter, {"str", "color"}))
      {
        throw Bad_Asset{"Map json", "Bad zone asset"};
      }

      // Set defaults
      auto zone = Zone_Impl{"Unknown", no::item, 1, false, 0};

      // Set the str.
      zone.str.assign((*iter)["str"].GetString());

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
} }
