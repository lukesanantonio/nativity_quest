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

  Volume<int> Sprite::layout_()
  {
    // TODO:
    // This is copied from the Label's implementation, find some way to share
    // this.
    auto this_vol = parent_volume();

    auto spr_extents = get_minimum_extents();

    this_vol.pos.x = center(this_vol.pos.x, this_vol.width,  spr_extents.x);
    this_vol.pos.y = center(this_vol.pos.y, this_vol.height, spr_extents.y);

    this_vol.width = spr_extents.x;
    this_vol.height = spr_extents.y;

    return this_vol;
  }
  void Sprite::render_() const noexcept
  {
    if(tex_)
    {
      auto dest = to_sdl_rect(this_volume());
      SDL_RenderCopy(graphics_.renderer, tex_.get(), NULL, &dest);
    }
  }

} }
