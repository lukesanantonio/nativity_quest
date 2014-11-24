/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "turn.h"
#include "../render.h"
namespace game
{
  void handle_event_state(Turn_Data&, SDL_Event const&) noexcept {}
  void step_state(Turn_Data&) noexcept {}

#define MINIMAP_SIZE 150
#define VIEWPORT_SIZE 300

  void render_state(Graphics_Desc& g, Turn_Data& data) noexcept
  {
    render_as_minimap(g, data.map, {{5, 5}, MINIMAP_SIZE, MINIMAP_SIZE});

    Player& player = data.map.players[data.current_player];
    Volume<> viewport_src;
    viewport_src.pos.x = player.pos.x - VIEWPORT_SIZE / 2;
    viewport_src.pos.y = player.pos.y - VIEWPORT_SIZE / 2;
    viewport_src.width = VIEWPORT_SIZE;
    viewport_src.height = VIEWPORT_SIZE;
    render_viewport(g, data.map, viewport_src,
                    {{0, 0}, g.get_width(), g.get_height()});
  }
}
