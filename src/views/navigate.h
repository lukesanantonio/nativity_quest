/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../game/Game.h"
#include "../Map.h"
#include "../common/vec.h"
#include "../decl/sprites.h"
#include "../decl/players.h"
namespace game
{
  struct Navigate_State : public Game_State
  {
    Navigate_State(Game& game, std::string sprite_json,
                   std::string players_json, std::string map_json,
                   std::string items_decl_json,
                   std::string enemies_decl_json,
                   std::string hud_json) noexcept;

    decl::Sprites sprites;
    decl::Players players;

    ui::Model hud;

    Map map;

    player_id player;

    // map coordinates.
    Vec<int> map_corner;

    void handle_event(SDL_Event const&) noexcept override;
    void step() noexcept override;
    void render() const noexcept override;

    void on_enter() noexcept override;
    void on_exit() noexcept override;

    inline bool is_toplevel() const noexcept override { return true; }
  };
}
