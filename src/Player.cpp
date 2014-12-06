/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Player.h"

#include "util/pi.h"

namespace game
{
  Player::Player(Vec<int> extents) noexcept
  {
    fog.reset(SDL_CreateRGBSurface(0, extents.x, extents.y,
                                   32,
                                   0xff000000,
                                   0x00ff0000,
                                   0x0000ff00,
                                   0x000000ff));
    if(fog.surface())
    {
      auto pixel = (uint8_t*) fog.surface()->pixels;
      const auto bpp = fog.surface()->format->BytesPerPixel;
      for(int i = 0; i < fog.surface()->pitch * extents.y / bpp; ++i)
      {
        *(uint32_t*) pixel = 0x000000ff;
        pixel += bpp;
      }
    }
  }

  void unfog(Player& p, int radius) noexcept
  {
    // Sketch a the border of a circle than do a flood fill.
    if(!p.fog.surface()) return;

    SDL_Surface* fog = p.fog.surface();
    p.fog.texture(nullptr);

    auto clear_color = 0x00000000;

    auto right = Vec<double>{radius, 0};

    auto end_iter = int(std::round(2 * PI * radius));
    for(int iter = 0; iter < end_iter; ++iter)
    {
      auto endpt = rotate(right, (2 * PI / end_iter) * iter);

      auto pixel_byte_ptr = (uint8_t*) fog->pixels;
      pixel_byte_ptr += fog->pitch * int(endpt.y + p.pos.y);
      pixel_byte_ptr += fog->format->BytesPerPixel * int(endpt.x + p.pos.x);

      uint32_t* pixel_ptr = (uint32_t*) pixel_byte_ptr;
      *pixel_ptr = clear_color;
    }

    // We just outlined our circle. Start a flood fill from the players
    // position, or center.

    // TODO: Flood fill.
  }
}
