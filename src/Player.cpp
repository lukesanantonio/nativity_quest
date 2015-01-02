/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Player.h"

#include "common/pi.h"
#include "decl/effects.h"
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
    static auto view_radius = 0;
    static auto pixels = std::vector<Vec<int> >{};

    auto player_view_radius = e.view_radius(p);

    if(view_radius != player_view_radius)
    {
      view_radius = player_view_radius;
      pixels = gen_pixels(view_radius);
    }

    // Sketch a the border of a circle than do a flood fill.
    if(!p.fog.surface()) return;

    SDL_Surface* fog = p.fog.surface();
    p.fog.texture(nullptr);

    // This function clears the specified pixel of the fog surface.
    auto clear_pixel = [fog, &p](Vec<int> pt)
    {
      pt.x += p.pos.x;
      pt.y += p.pos.y;

      if(pt.x < 0 || fog->w < pt.x) return;
      if(pt.y < 0 || fog->h < pt.y) return;

      constexpr auto clear_color = 0x00000000;

      auto pixel_byte_ptr = (uint8_t*) fog->pixels;
      pixel_byte_ptr += fog->pitch * int(pt.y);
      pixel_byte_ptr += fog->format->BytesPerPixel * int(pt.x);

      uint32_t* pixel_ptr = (uint32_t*) pixel_byte_ptr;
      *pixel_ptr = clear_color;
    };

    for(auto const& pt : pixels)
    {
      clear_pixel(pt);
    }
  }
}
