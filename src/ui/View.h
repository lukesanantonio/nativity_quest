/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "../vec.h"
#include "../volume.h"

#include "../Graphics_Desc.h"
#include "../Color.h"
namespace game { namespace ui
{
  struct Label
  {
    std::string str;
    Vec<int> pos;
    Color color;
  };

  struct Box
  {
    Volume<int> vol;
    Color color;
  };

  struct View
  {
    View(Graphics_Desc& graphics) noexcept : g_(&graphics) {}

    void label(Label const& label) noexcept;
    void box(Box const& box) noexcept;

    void reset() noexcept;
    void render() const noexcept;

    inline Vec<int> size() const noexcept
    { return g_->size(); }

    Vec<int> text_size(std::string const& str) const noexcept;
  private:
    Graphics_Desc* g_;

    std::vector<Label> label_cache_;
    std::vector<Box> box_cache_;
  };
} }
