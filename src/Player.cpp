/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Player.h"

#include "util/pi.h"
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
    if(p.fog.surface())
    {
      auto pixel = (uint8_t*) p.fog.surface()->pixels;
      const auto bpp = p.fog.surface()->format->BytesPerPixel;
      for(int i = 0; i < p.fog.surface()->pitch * extents.y / bpp; ++i)
      {
        *(uint32_t*) pixel = 0x000000ff;
        pixel += bpp;
      }
    }
  }

  void unfog(Player& p) noexcept
  {
    // Sketch a the border of a circle than do a flood fill.
    if(!p.fog.surface()) return;

    SDL_Surface* fog = p.fog.surface();
    p.fog.texture(nullptr);

    constexpr auto clear_color = 0x00000000;
    const auto right_vec = Vec<double>{p.view_radius, 0};

    std::vector<Vec<int> > pts;

    // Push the player position (or at least is the position of the player
    // relative to the current player position).
    pts.push_back({0,0});

    // Circumference of the field of view.
    auto circum = int(std::round(2 * PI * p.view_radius));
    for(auto rot_step = 0; rot_step < circum; ++rot_step)
    {
      auto endpt = rotate<double>(right_vec, (2 * PI / circum) * rot_step);

      // Push the endpoint or border point. Since we already pushed the player
      // position we know that at the very least, the endpoints of the circle
      // (center + border) will be changed to a clear color. All of the others
      // in between will be easier to obtain now that we know we don't have to
      // worry about the extremes.
      pts.push_back(endpt);

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
        if(pts.back() == inter_pt) continue;

        pts.push_back(inter_pt);

      }
    }

    // A flood fill would have better results but this works too, sorta.
    // For every point on the circle, set to a clear color.
    for(auto const& clear_pt : pts)
    {
      auto pixel_byte_ptr = (uint8_t*) fog->pixels;
      pixel_byte_ptr += fog->pitch * int(clear_pt.y + p.pos.y);
      pixel_byte_ptr += fog->format->BytesPerPixel * int(clear_pt.x + p.pos.x);

      uint32_t* pixel_ptr = (uint32_t*) pixel_byte_ptr;
      *pixel_ptr = clear_color;
    }
  }
}
