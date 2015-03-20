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

#include "decl/zones.h"
#include "decl/enemies.h"

#include "assets/assets.h"

#include "game/Game.h"
namespace game
{
  using player_id = short;

  struct Chest
  {
    assets::Item item;
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
    Map(Game& game, std::string map, std::string items,
        std::string enemy_decl) noexcept;

    Game& game;

    assets::Items items;
    decl::Enemies enemies_decl;
    decl::Zones zones;

    std::shared_ptr<assets::Image_Asset> map_sprite;
    std::shared_ptr<assets::Image_Asset> map_overlay_sprite;
    std::shared_ptr<assets::Image_Asset> chest_sprite;

    std::vector<Chest> chests;
    std::vector<Enemy_Instance> enemies;

    std::array<Player, 6> players;

    double scale; // screen pixels / map pixel
    double mini_scale;

    inline Vec<int> size() const noexcept;
  };

  inline Vec<int> Map::size() const noexcept
  {
    return {map_sprite->image.surface->w, map_sprite->image.surface->h};
  }
}
