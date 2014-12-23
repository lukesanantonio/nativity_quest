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
#include "../Font_Renderer.h"
#include "../Color.h"
namespace game { namespace ui
{
  struct Label
  {
    Label(std::string const& str = {},
          int size = 0,
          Vec<int> pos = {},
          Color c = {}) noexcept
          : str{str}, size{size}, pos{pos}, color(c) {}

    Label(Label const& label) noexcept;
    Label(Label&& label) noexcept = default;

    Label& operator=(Label const& label) noexcept;
    Label& operator=(Label&& label) noexcept = default;

    std::string str;
    int size;
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
    View(Font_Renderer& font) noexcept : font_(&font) {}

    void label(Label const& label) noexcept;
    void box(Box const& box) noexcept;

    inline void font_renderer(Font_Renderer& font) noexcept
    { font_ = &font; }
    inline Font_Renderer& font_renderer() const noexcept
    { return *font_; }

    void reset() noexcept;
    void render(Graphics_Desc& g) const noexcept;

    Vec<int> text_size(std::string str, int size) const noexcept;
  private:
    // Regenerates a label's
    void generate_label_surface_cache(Label const& l) const noexcept;

    Font_Renderer* font_;

    std::vector<Label> label_cache_;
    std::vector<Box> box_cache_;
  };
} }
