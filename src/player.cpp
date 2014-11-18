/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "player.h"

#define PLAYER_SPEED 4

namespace game
{
  SDL_Rect get_sprite_from_direction(Player::Direction dir)
  {
    SDL_Rect rect;
    rect.w = 50;
    rect.h = 50;
    switch(dir)
    {
      case Player::Direction::Up:
      {
        rect.x = 50;
        rect.y = 50;
        break;
      }
      case Player::Direction::Down:
      {
        rect.x = 0;
        rect.y = 50;
        break;
      }
      case Player::Direction::Right:
      {
        rect.x = 0;
        rect.y = 0;
        break;
      }
      case Player::Direction::Left:
      {
        rect.x = 50;
        rect.y = 0;
        break;
      }
    }
    return rect;
  }
  void handle_player_event(SDL_Event const& e, Player& p)
  {
    if(e.type == SDL_KEYDOWN)
    {
      if(e.key.keysym.scancode == SDL_SCANCODE_W)
      {
        p.dir = Player::Direction::Up;
        p.pos.y -= PLAYER_SPEED;
      }
      else if(e.key.keysym.scancode == SDL_SCANCODE_A)
      {
        p.dir = Player::Direction::Left;
        p.pos.x -= PLAYER_SPEED;
      }
      else if(e.key.keysym.scancode == SDL_SCANCODE_S)
      {
        p.dir = Player::Direction::Down;
        p.pos.y += PLAYER_SPEED;
      }
      else if(e.key.keysym.scancode == SDL_SCANCODE_D)
      {
        p.dir = Player::Direction::Right;
        p.pos.x += PLAYER_SPEED;
      }
    }
  }
}
