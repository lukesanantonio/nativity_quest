/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "../util/surface.h"

#include "../vec.h"
#include "../volume.h"

#include "../Graphics_Desc.h"
#include "../Color.h"
namespace game { namespace ui
{
  struct Label
  {
    Label(std::string const& str = {},
          Vec<int> pos = {},
          Color c = {}) noexcept
          : str{str}, pos{pos}, color(c) {}

    Label(Label const& label) noexcept;
    Label(Label&& label) noexcept = default;

    Label& operator=(Label const& label) noexcept;
    Label& operator=(Label&& label) noexcept = default;

    std::string str;
    Vec<int> pos;
    Color color;

    mutable Surface_Ptr surface_cache_;
    mutable Texture_Ptr texture_cache_;
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
    void generate_label_surface_cache(Label const& l) const noexcept;

    Graphics_Desc* g_;

    std::vector<Label> label_cache_;
    std::vector<Box> box_cache_;
  };
} }
