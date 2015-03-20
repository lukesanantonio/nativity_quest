/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>
#include "SDL.h"

#include "common/vec.h"
#include "decl/items.h"
#include "common/surface.h"

#include "Entity_Data.h"

namespace game
{
  namespace decl
  {
    struct Effects;
  }

  struct Surface_Cache
  {
    Surface_Ptr surface;

    SDL_Texture* texture(SDL_Renderer* r) const noexcept;
  private:
    mutable SDL_Texture* texture_;
    mutable SDL_Renderer* renderer_;
  };

  struct Player
  {
    // Position on the map.
    Vec<double> pos = {};

    std::array<decl::Item, 6> inventory;

    double view_radius = 60.0;
    Surface_Cache fog = {};

    Entity_Data entity_data;

    double moved = 0.0;
    double max_movement = 150.0;

    Vec<int> spawn_pt = {};

    short sprite_frame = 0;

    // effect variables
    int turns_of_haste = 0;
    int combat_defense = 0;
    bool flare = false;
  };

  void reset_fog(Player&, Vec<int> extents) noexcept;
  void unfog(Player&, decl::Effects&) noexcept;

  inline void respawn(Player& p) noexcept
  {
    p.pos = p.spawn_pt;
  }
}
