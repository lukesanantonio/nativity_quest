/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../View.h"
#include "../../common/surface.h"
namespace game { namespace ui
{
  struct Sprite : public View
  {
    Sprite(Graphics_Desc& graphics, std::string src) noexcept;

    Vec<int> get_minimum_extents() const noexcept override;
    void dispatch_event(SDL_Event const&) noexcept override;
    void layout_(Volume<int>) override;
    void render_() const noexcept override;

    inline double scale() const noexcept { return scale_; }
    inline void scale(double scale) noexcept { scale_ = scale; }

  private:
    Texture_Ptr tex_;

    double scale_ = 1.0;
  };
} }
