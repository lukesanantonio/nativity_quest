/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Player.h"

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
}
