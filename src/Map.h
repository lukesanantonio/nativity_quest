/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "SDL.h"
#include "common/surface.h"

#include "Player.h"
#include "Enemy_Instance.h"

#include "decl/items.h"
#include "decl/zones.h"
#include "decl/enemies.h"
#include "decl/sprites.h"
namespace game
{
  using player_id = short;

  struct Chest
  {
    decl::Item item;
    bool visible;
    Vec<int> pos;
  };

  /*!
   * \brief Represents a map image separated into zones housing 6 players.
   *
   * No game state (such as the currently active player) should be here.
   */
  struct Map
  {
    Map(decl::Sprites& sprites, std::string map_json,
        std::string items_decl_json, std::string enemy_decl_json) noexcept;

    decl::Sprites& sprites;

    decl::Items items;
    decl::Enemies enemies_decl;

    std::string map_sprite;
    std::string map_overlay_sprite;
    std::string chest_sprite;

    std::vector<Chest> chests;
    std::vector<Enemy_Instance> enemies;

    std::array<Player, 6> players;

    double scale; // screen pixels / map pixel
    double mini_scale;

    decl::Zones zones;

    inline Vec<int> size() const noexcept;
  };

  inline Vec<int> Map::size() const noexcept
  {
    auto surf = sprites.get_sprite(map_sprite)->surface();
    return {surf->w, surf->h};
  }
}
