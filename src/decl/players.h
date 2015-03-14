/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <array>

#include "../common/volume.h"
namespace game { namespace decl
{
  struct Orientation_Info
  {
    Volume<int> src;
    Vec<int> center;
  };

  struct Players
  {
    Players(std::string json_file);

    inline double get_sprite_scale() const noexcept
    { return sprite_scale_; }

    inline std::string get_item(short player_index) const noexcept
    { return starting_items_[player_index]; }

    inline std::string get_name(short player_index) const noexcept
    { return player_names_[player_index]; }

    inline std::string get_sprite(short player_index) const noexcept
    { return sprite_srcs_[player_index]; }

    inline Orientation_Info get_orientation_info(short o) const noexcept
    { return orient_info_[o]; }
  private:
    double sprite_scale_;

    std::array<std::string, 6> starting_items_;
    std::array<std::string, 6> player_names_;

    std::array<std::string, 6> sprite_srcs_;
    std::array<Orientation_Info, 4> orient_info_;
  };
} }
