/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "zone_class.h"
#include "../../util/json.h"
namespace game
{
  Zone_Class parse_class(rapidjson::Value const& val) noexcept
  {
    auto c = Zone_Class{};

    if_has_member(val, "str", [&c](auto const& val)
    {
      c.str.assign(val.GetString(), val.GetStringLength());
    });

    if_has_member(val, "required_item", [&c](auto const& val)
    {
      auto item_str = std::string{val.GetString(), val.GetStringLength()};
      c.required_item = get_item(item_str);
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
