/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "sprite.h"
#include "SDL_image.h"

#include "../../common/center.hpp"
namespace game { namespace ui
{
  Sprite::Sprite(Graphics_Desc& graphics, std::string src) noexcept
                 : View(graphics)
  {
    auto surface = IMG_Load(src.data());
    tex_.reset(SDL_CreateTextureFromSurface(graphics.renderer, surface));
  }

  Vec<int> Sprite::get_minimum_extents() const noexcept
  {
    auto extents = Vec<int>{};
    SDL_QueryTexture(tex_.get(), NULL, NULL, &extents.x, &extents.y);

    extents *= scale_;
    return extents;
  }

  void Sprite::dispatch_event(SDL_Event const&) noexcept { }

  void Sprite::render_() const noexcept
  {
    auto dest = to_sdl_rect(volume_());

    auto spr_extents = get_minimum_extents();

    dest.x = center(dest.x, dest.w, spr_extents.x);
    dest.y = center(dest.y, dest.h, spr_extents.y);

    dest.w = spr_extents.x;
    dest.h = spr_extents.y;

    if(tex_)
    {
      SDL_RenderCopy(graphics_.renderer, tex_.get(), NULL, &dest);
    }
  }

} }
