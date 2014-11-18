/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "zone_impl.h"
#include "../items.h"

#include "rapidjson/document.h"

#include <boost/optional.hpp>
namespace game
{
  struct Zone_Class
  {
    std::string str;
    boost::optional<Item> required_item;
    boost::optional<short> speed_cost;
    boost::optional<bool> important;
  };

  Zone_Class parse_class(rapidjson::Value const& val) noexcept;
  void apply_class(Zone_Class const& cl, Zone_Impl& zone) noexcept;
}
