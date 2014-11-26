/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "turn.h"

#include "../render.h"
namespace game
{
  void update_zone(Turn_Data& data) noexcept
  {
    Player& player = data.map.players[data.current_player];
    Zone zone = data.map.zones.get_zone(player.pos);
    data.zone_label.data(zone ? zone->str : "Unknown");
  }

  Turn_Data::Turn_Data(Map map, std::string char_img) noexcept
                       : map(std::move(map)), current_player(0),
                         character(char_img)
  {
    this->zone_label.text_height(35);
    this->zone_label.text_color({0x00, 0x00, 0x00, 0xff});

    this->map.players[current_player].pos = {500, 0};
    update_zone(*this);
  }

  void handle_event_state(Turn_Data& data, SDL_Event const& event) noexcept
  {
    Player& p = data.map.players[data.current_player];

    if(event.type == SDL_KEYDOWN)
    {
      pong::math::vector<int> delta;
      if(event.key.keysym.scancode == SDL_SCANCODE_W)
      {
        delta.y -= 5;
        p.dir = Player::Direction::Up;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_A)
      {
        delta.x -= 5;
        p.dir = Player::Direction::Left;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_S)
      {
        delta.y += 5;
        p.dir = Player::Direction::Down;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_D)
      {
        delta.x += 5;
        p.dir = Player::Direction::Right;
      }

      // Degrade or flat out forget about the delta if we don't have the item
      // that we are required to have for this zone.
      Zone next_zone = data.map.zones.get_zone(p.pos + delta);
      if(next_zone)
      {
        if(next_zone->required_item)
        {
          using std::begin; using std::end;
          auto item_find = std::find(begin(p.inventory), end(p.inventory),
                                     next_zone->required_item);
          // We didn't find the required item.
          if(item_find == end(p.inventory))
          {
            // No move.
            delta = {0,0};
          }
        }

        if(delta.x < 0) delta.x = delta.x + next_zone->speed_cost;
        if(delta.y < 0) delta.y = delta.y + next_zone->speed_cost;

        if(delta.x > 0) delta.x = delta.x - next_zone->speed_cost;
        if(delta.y > 0) delta.y = delta.y - next_zone->speed_cost;

        p.pos += delta;
        update_zone(data);
      }
    }
  }
  void step_state(Turn_Data& data) noexcept {}

#define MINIMAP_SCALE .25
#define VIEWPORT_SIZE 225

  void render_state(Graphics_Desc& g, Turn_Data& data) noexcept
  {
    Player& player = data.map.players[data.current_player];

    // Render the main viewport.
    Volume<> viewport_src;
    viewport_src.pos.x = std::max(0, player.pos.x - VIEWPORT_SIZE / 2);
    viewport_src.pos.y = std::max(0, player.pos.y - VIEWPORT_SIZE / 2);
    viewport_src.width =
       std::min(data.map.img.surface()->w - viewport_src.pos.x, VIEWPORT_SIZE);
    viewport_src.height =
       std::min(data.map.img.surface()->h - viewport_src.pos.y, VIEWPORT_SIZE);

    viewport_src.pos.x -= VIEWPORT_SIZE - viewport_src.width;
    viewport_src.pos.y -= VIEWPORT_SIZE - viewport_src.height;

    viewport_src.width = VIEWPORT_SIZE;
    viewport_src.height = VIEWPORT_SIZE;

    render_viewport(g, data.map, viewport_src,
                    {{0, 0}, g.get_width(), g.get_height()});

    // Render the mini map.
    Volume<> minimap_vol = {{5, 5},
                            int(data.map.img.surface()->w * MINIMAP_SCALE),
                            int(data.map.img.surface()->h * MINIMAP_SCALE)};

    render_as_minimap(g, data.map, minimap_vol);

    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0xff);
    render_player_minimap(g, data.map, minimap_vol, player, 4);

    // Render the zone.
    data.zone_label.font_face(&g.font.face);
    data.zone_label.rasterizer(&g.font.raster);
    data.zone_label.position(
      {g.get_width() - data.zone_label.surface_width() - 10, 5});
    data.zone_label.render(g.renderer);

    // Render the character sprite.
    SDL_Rect src = get_sprite_from_direction(player.dir);
    SDL_Rect dest;
    dest.x = player.pos.x - viewport_src.pos.x;
    dest.y = player.pos.y - viewport_src.pos.y;
    dest.w = src.w;
    dest.h = src.h;
    use_viewport_scale(dest, g.get_width() / (double) VIEWPORT_SIZE,
                       g.get_height() / (double) VIEWPORT_SIZE);
    SDL_RenderCopy(g.renderer, data.character.texture(g.renderer), &src,&dest);
  }
}
