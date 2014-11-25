/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "turn.h"
#include "../render.h"
namespace game
{
  void handle_event_state(Turn_Data& data, SDL_Event const& event) noexcept
  {
    Player& p = data.map.players[data.current_player];
    if(event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.scancode == SDL_SCANCODE_W)
      {
        p.pos.y -= 5;
        p.dir = Player::Direction::Up;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_A)
      {
        p.pos.x -= 5;
        p.dir = Player::Direction::Left;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_S)
      {
        p.pos.y += 5;
        p.dir = Player::Direction::Down;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_D)
      {
        p.pos.x += 5;
        p.dir = Player::Direction::Right;
      }
    }
  }
  void step_state(Turn_Data&) noexcept {}

#define MINIMAP_SIZE 175
#define VIEWPORT_SIZE 225

  void use_viewport_scale(SDL_Rect& rect,
                          double xscale, double yscale) noexcept
  {
    rect.x *= xscale;
    rect.y *= yscale;
  }

  void render_state(Graphics_Desc& g, Turn_Data& data) noexcept
  {
    Player& player = data.map.players[data.current_player];
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

    render_as_minimap(g, data.map, {{5, 5}, MINIMAP_SIZE, MINIMAP_SIZE});

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
