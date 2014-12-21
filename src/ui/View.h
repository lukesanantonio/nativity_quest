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

    void render() const noexcept;

    inline Vec<int> size() const noexcept
    { return g_->size(); }

    Vec<int> text_size(std::string const& str) const noexcept;
  private:
    Graphics_Desc* g_;

    // Finds or makes a label that can store the text that needs to be
    // rendered.
    Label& get_unused_label(std::string const& str) const noexcept;
    mutable std::vector<std::tuple<Label, bool> > label_cache_;

    // Temporary storage for any volumes that need to be rendered.
    mutable std::vector<Box> box_cache_;
  };
} }
