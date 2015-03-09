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
  struct Player_Sprite
  {
    std::string src;
    Volume<int> src_vol;
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

    inline Player_Sprite get_sprite(short orientation) const noexcept
    { return sprite_info_[orientation]; }
  private:
    std::string sprite_;

    double sprite_scale_;

    std::array<std::string, 6> starting_items_;
    std::array<std::string, 6> player_names_;

    std::array<Player_Sprite, 4> sprite_info_;
  };
} }
