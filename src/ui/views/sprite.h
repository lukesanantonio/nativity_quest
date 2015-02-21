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
    explicit Sprite(Graphics_Desc& graphics) noexcept : View(graphics) {}

    Vec<int> get_minimum_extents() const noexcept override;

    inline double scale() const noexcept { return scale_; }
    inline void scale(double scale) noexcept { scale_ = scale; }

    void src(std::string str) noexcept;
    inline std::string src() const noexcept { return src_; }

  private:
    Volume<int> layout_() override;
    void render_() const noexcept override;

    Texture_Ptr tex_;

    std::string src_;
    double scale_ = 1.0;
  };
} }
