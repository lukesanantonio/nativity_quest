/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <array>

#include "../volume.h"
namespace game
{
  struct Char_Info
  {
    std::string sprite;

    double scale;

    Volume<int> src;
    Vec<int> center;
  };

  struct Character_Parser
  {
    Character_Parser(std::string json_file);

    inline Char_Info get_sprite_info(short orientation) const noexcept;
  private:
    std::array<Char_Info, 4> infos_;
  };

  inline Char_Info Character_Parser::get_sprite_info(short o) const noexcept
  {
    return infos_[o];
  }
}
