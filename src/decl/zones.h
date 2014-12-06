/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <memory>

#include "../util/surface.h"
#include "../vec.h"
#include "../Sprite_Container.h"

#include "items.h"
namespace game
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

  struct Zone_Parser
  {
    explicit Zone_Parser(std::string const& json_file,
                         Item_Parser const& items);
    Zone get_zone(Vec<int> pos) const noexcept;

    inline std::string map_asset() const noexcept;

    inline Vec<int> get_map_extents() const noexcept;
  private:
    std::vector<Zone> zones_;

    Surface_Ptr zone_decl_;

    std::string map_asset_;
  };

  inline std::string Zone_Parser::map_asset() const noexcept
  {
    return map_asset_;
  }
  inline Vec<int> Zone_Parser::get_map_extents() const noexcept
  {
    return {zone_decl_->w, zone_decl_->h};
  }

  namespace no
  {
    const Zone zone;
  }
}
