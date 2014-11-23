/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <memory>

#include "../util/surface.h"
#include "../common/vec.h"

#include "items.h"
namespace game
{
  struct Invalid_Decl_Image
  {
    std::string given;
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

  struct Zone_Parser
  {
    Zone_Parser(std::string const& json_file, Item_Parser const& items);
    Zone get_zone(Vec<int> pos) const noexcept;
  private:
    std::vector<Zone> zones_;

    Surface_Ptr png_;
  };

  namespace no
  {
    const Zone zone;
  }
}
