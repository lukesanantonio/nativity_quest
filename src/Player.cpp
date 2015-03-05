/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Player.h"

#include "common/pi.h"
#include "decl/effects.h"

#include <cmath>
namespace game
{
  void reset_fog(Player& p, Vec<int> extents) noexcept
  {
    p.fog.reset(SDL_CreateRGBSurface(0, extents.x, extents.y,
                                     32,
                                     0xff000000,
                                     0x00ff0000,
                                     0x0000ff00,
                                     0x000000ff));

    auto surf = p.fog.surface();
    if(surf)
    {
      auto pixel = (uint8_t*) surf->pixels;
      const auto bpp = surf->format->BytesPerPixel;
      for(int i = 0; i < surf->pitch * extents.y / bpp; ++i)
      {
        *(uint32_t*) pixel = 0x000000ff;
        pixel += bpp;
      }
    }
  }

  std::vector<Vec<int> > gen_pixels(double view_radius) noexcept
  {
    std::vector<Vec<int> > points;

    const auto right_vec = Vec<double>{view_radius, 0};

    // Clear the player position (relative to player, so 0,0 works).
    points.push_back({0, 0});

    // Circumference of the field of view.
    auto circum = int(std::round(2 * PI * view_radius));
    for(auto rot_step = 0; rot_step < circum; ++rot_step)
    {
      auto endpt = rotate<double>(right_vec, (2 * PI / circum) * rot_step);

      // Since we already cleared the player position we know that at the very
      // least, the endpoints of the circle (center + border) will be changed
      // to the correct color. All of the other points in between will be
      // easier to get to now that we know we don't have to worry about the
      // extremes.
      points.push_back(endpt);

      auto endpt_length = length(endpt);
      for(auto ray_len = 0.0; ray_len < endpt_length;)
      {
        // Find our position on the ray from the center to the outside of the
        // circle.
        Vec<int> inter_pt = normalize(endpt) * ray_len;

        // Increment our position on the ray a little bit.
        ray_len = std::min(endpt_length, ray_len + .5);

        // Only add the point if it wasn't added last iteration, since that
        // means we haven't actually moved pixels at all.
        if(points.back() == inter_pt) continue;

        // Clear the pixel and record the point last cleared.
        points.push_back(inter_pt);
      }
    }

    return points;
  }

  void unfog(Player& p, decl::Effects& e) noexcept
  {
    auto surface = p.fog.surface();
    if(!surface) return;

    SDL_LockSurface(surface);

    auto view_radius = int(e.view_radius(p));
    for(int row = 1; row <= view_radius; ++row)
    {
      using std::floor; using std::sqrt;
      auto half_width = floor(sqrt((2.0 * view_radius * row) - (row * row)));

      auto pos = Vec<int>{p.pos};
      auto left_x = int(pos.x - half_width);

      uint8_t* top_row = (uint8_t*) surface->pixels +
                         (pos.y - (view_radius - row)) * surface->pitch +
                         (left_x * surface->format->BytesPerPixel);
      uint8_t* bottom_row = (uint8_t*) surface->pixels +
                            (pos.y + (view_radius - row)) * surface->pitch +
                            (left_x * surface->format->BytesPerPixel);

      if(pos.y - (view_radius - row) < 0)
      {
        top_row = nullptr;
      }
      if(surface->h <= pos.y + (view_radius - row))
      {
        bottom_row = nullptr;
      }

      for(int col = 0; col < half_width * 2; ++col)
      {
        if(left_x + col < 0 || surface->w <= left_x + col)
        {
          continue;
        }

        if(top_row)
        {
          *(uint32_t*)top_row = 0x00000000;
          top_row += surface->format->BytesPerPixel;
        }
        if(bottom_row)
        {
          *(uint32_t*)bottom_row = 0x00000000;
          bottom_row += surface->format->BytesPerPixel;
        }
      }
    }


    SDL_UnlockSurface(surface);

    p.fog.texture(nullptr);
  }
}
