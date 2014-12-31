/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <vector>
#include <string>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include "rapidjson/document.h"
namespace game { namespace ui
{
  struct Text
  {
    std::string str;

    std::string col;
  };

  struct Button
  {
    std::string str;
    std::string event;

    std::string col;
  };

  struct Sprite
  {
    std::string src;
    double scale;

    boost::optional<std::string> border_col;
  };

  using Element_Data = boost::variant<Text, Button, Sprite>;

  enum class Horizontal_Alignment
  {
    Left, Center, Right
  };
  enum class Vertical_Alignment
  {
    Top, Center, Bottom
  };

  struct Padding
  {
    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;
  };

  struct Element
  {
    Element_Data element;

    Padding padding;

    Horizontal_Alignment h_align;
    Vertical_Alignment v_align;

    boost::optional<double> autohide;
  };

  struct Model
  {
    Model(rapidjson::Document const& json);
    Model() noexcept;

    std::vector<Element> elements;
  };
} }
