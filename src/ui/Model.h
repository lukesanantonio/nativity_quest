/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <vector>
#include <string>

#include <boost/variant.hpp>
#include "rapidjson/document.h"
namespace game { namespace ui
{
  struct Text
  {
    std::string str;
  };

  struct Button
  {
    std::string str;
    std::string event;
  };

  using Element_Data = boost::variant<Text, Button>;

  enum class Alignment_Type
  {
    Centered, Top, Left, Bottom, Right
  };

  struct Element
  {
    Alignment_Type alignment;
    Element_Data element;
  };

  enum class Layout_Type
  {
    Vertical, Horizontal
  };

  struct Model
  {
    Model(rapidjson::Document const& json);
    Model() noexcept;

    inline void layout(Layout_Type layout) noexcept
    { layout_ = layout; }

    inline Layout_Type layout() const noexcept
    { return layout_; }

    inline std::vector<Element>& elements() noexcept
    { return elemts_; }

    inline std::vector<Element> const& elements() const noexcept
    { return elemts_; }

  private:
    Layout_Type layout_;
    std::vector<Element> elemts_;
  };
} }
