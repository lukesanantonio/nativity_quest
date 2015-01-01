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

#include "../common/volume.h"
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
    double scale = 1.0;

    boost::optional<Volume<int> > vol;

    boost::optional<std::string> border_col;
  };

  struct Label_View
  {
    int rows = 2;
    int selected = 0;
    bool done = false;

    std::vector<std::string> labels = {};

    bool is_selectable = true;

    std::string col;
  };

  struct Bar
  {
    int max = 1;
    int cur = 0;
  };

  using Element_Data = boost::variant<Text, Button, Sprite, Label_View,
                                      Bar>;

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

  struct Alignment
  {
    Horizontal_Alignment horizontal;
    Vertical_Alignment vertical;
  };

  enum class Side
  {
    Top, Right, Bottom, Left
  };

  struct Element
  {
    Element_Data element;

    Padding padding;

    boost::variant<Alignment, Side> align;

    bool is_visible = true;

    boost::optional<double> autohide;
  };

  struct Model
  {
    Model(rapidjson::Document const& json);
    Model() noexcept;

    std::vector<Element> elements;
  };
} }
