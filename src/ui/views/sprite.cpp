/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "sprite.h"
#include "SDL_image.h"

#include "../../common/center.hpp"
namespace game { namespace ui
{
  Sprite::Sprite(Graphics_Desc& g, Image_Asset_Ptr image) noexcept
                 : View(g), src_(image)
  {
    generate();
  }

  Vec<int> Sprite::get_minimum_extents() const noexcept
  {
    auto extents = Vec<int>{};
    SDL_QueryTexture(tex_.get(), NULL, NULL, &extents.x, &extents.y);

    if(src_vol_)
    {
      extents = { src_vol_.value().width, src_vol_.value().height };
    }

    extents *= scale_;
    return extents;
  }

  void Sprite::src(Image_Asset_Ptr src) noexcept
  {
    src_ = src;
    generate();
  }

  void Sprite::set_src_rect(Volume<int> vol) noexcept
  {
    src_vol_ = vol;
  }
  Volume<int> Sprite::get_src_rect() const noexcept
  {
    auto vol = Volume<int>{};
    if(src_vol_) vol = src_vol_.value();
    return vol;
  }
  void Sprite::remove_src_rect() noexcept
  {
    src_vol_ = boost::none;
  }

  void Sprite::generate() noexcept
  {
    if(src_)
    {
      auto surface = src_->image;
      tex_.reset(SDL_CreateTextureFromSurface(graphics_.renderer, surface));
    }
  }

  Volume<int> Sprite::layout_()
  {
    return center_volume(parent_volume(), get_minimum_extents());
  }
  void Sprite::render_() const noexcept
  {
    if(tex_)
    {
      auto dest = to_sdl_rect(this_volume());
      if(src_vol_)
      {
        auto src_rect = to_sdl_rect(src_vol_.value());
        SDL_RenderCopy(graphics_.renderer, tex_.get(), &src_rect, &dest);
      }
      else
      {
        SDL_RenderCopy(graphics_.renderer, tex_.get(), NULL, &dest);
      }
    }
  }

} }
