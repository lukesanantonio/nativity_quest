/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "turn.h"

#include "../render.h"

#include <cmath>
#include <iostream>
#define PI 3.14159

namespace game
{
  Turn_Data::Turn_Data(std::string const& items_file,
                       std::string const& zones_file) noexcept
                       : items(items_file), map(zones_file, items),
                         player(0), state(Waiting_Data{}), map_corner{}
  {
    zone_label.text_height(35);
    zone_label.text_color({0x00, 0x00, 0x00, 0xff});

    map.players[player].pos = {500, 0};
    map.scale = 3.5;
    map.mini_scale = .25;

    update_zone();
  }

  void Turn_Data::update_zone() noexcept
  {
    auto& active_player = map.players[player];

    auto zone = map.zones.get_zone(Vec<int>{active_player.pos});
    zone_label.data(zone ? zone->str : "Unknown");
  }

  void Turn_Data::next_player() noexcept
  {
    if(++player == map.players.size()) player = 0;
  }

  void handle_event_state(Turn_Data& turn, SDL_Event const& event) noexcept
  {
    struct Turn_State_Visitor : boost::static_visitor<Turn_State>
    {
      Turn_State_Visitor(Turn_Data& turn, SDL_Event const& event) noexcept
                         : turn(turn), event(event) {}

      Turn_State operator()(Waiting_Data const& data) const noexcept
      {
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
          if(event.button.button == SDL_BUTTON_LEFT)
          {
            Moving_Data md;

            // Get the mouse in screen coordinates.
            auto mouse = Vec<int>{event.button.x, event.button.y};

            // Use the map scale and current top-left corner to calculate the
            // map coordinates.
            auto map_coord = (mouse / turn.map.scale) + turn.map_corner;

            // Calculate our delta movement.
            md.delta = map_coord - turn.map.players[turn.player].pos;

            return md;
          }
        }
        return data;
      }
      Turn_State operator()(Moving_Data const& md) const noexcept
      {
        return md;
      }

      Turn_Data& turn;
      SDL_Event const& event;
    };

    turn.state = boost::apply_visitor(Turn_State_Visitor{turn, event},
                                      turn.state);
  }

  void step_state(Turn_Data& turn_data) noexcept
  {
    struct Turn_State_Visitor : boost::static_visitor<Turn_State>
    {
      Turn_State_Visitor(Turn_Data& td) noexcept : turn(td) {}

      Turn_State operator()(Waiting_Data& data) const noexcept
      {
        return data;
      }
      Turn_State operator()(Moving_Data& data) const noexcept
      {
        auto& player = turn.map.players[turn.player];

        auto delta_len = length(data.delta);

        auto unit_delta = normalize<>(data.delta);
        auto move_delta = unit_delta * std::min(2.0, delta_len);

        player.pos += move_delta;
        data.delta -= move_delta;

        if(delta_len < 1.0)
        {
          //turn.next_player();
          return Waiting_Data{};
        }

        return data;
      }

      Turn_Data& turn;
    };

    turn_data.state = boost::apply_visitor(Turn_State_Visitor{turn_data},
                                           turn_data.state);
  }

  void render_state(Graphics_Desc& g, Sprite_Container& sprites,
                    Turn_Data& turn) noexcept
  {
    Player& player = turn.map.players[turn.player];

    // Recalculate the map corner.
    const auto viewport_width = int(g.get_width() / turn.map.scale);
    const auto viewport_height = int(g.get_height() / turn.map.scale);

    auto map_sprite = sprites.get_sprite(turn.map.zones.map_asset());

    SDL_Rect viewport_src;

    // Find the coordinates of the top-left corner of a viewport that results
    // in the player being centered on it.
    // However, don't allow the corner to go behind (0,0).
    viewport_src.x = std::max(0.0, player.pos.x - viewport_width / 2);
    viewport_src.y = std::max(0.0, player.pos.y - viewport_height / 2);

    // The width and height of the viewport are always no greater than the
    // calculated values above (using the map scale and screen size). They will
    // be smaller when the max width would result in a src rectangle that went
    // beyond the map image's bounds.
    viewport_src.w =
           std::min(viewport_width, map_sprite->surface()->w - viewport_src.x);
    viewport_src.h =
          std::min(viewport_height, map_sprite->surface()->h - viewport_src.y);

    // Adjust the viewport corners to give the possibly shortened width and
    // height room to be the max.
    viewport_src.x -= viewport_width - viewport_src.w;
    viewport_src.y -= viewport_width - viewport_src.h;

    // The viewport should always have the calculated width. We know this won't
    // go beyond the map image's bounds because we already adjusted the x and y
    // values accordingly.
    viewport_src.w = viewport_width;
    viewport_src.h = viewport_height;

    // These are our calculated corners!
    turn.map_corner.x = viewport_src.x;
    turn.map_corner.y = viewport_src.y;

    // Render the full map.
    SDL_RenderCopy(g.renderer, map_sprite->texture(g.renderer),
                   &viewport_src, NULL);

    // Render the character.
    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_Rect rect;
    rect.x = (player.pos.x - viewport_src.x) * turn.map.scale - 25;
    rect.y = (player.pos.y - viewport_src.y) * turn.map.scale - 25;
    rect.w = 50; rect.h = 50;
    SDL_RenderFillRect(g.renderer, &rect);

    // Render the mini map.
    render_as_minimap(g, sprites, turn.map, {5,5});
  }
}
