/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Model.h"
namespace game { namespace ui
{
  struct Bad_Layout {};
  template <class Doc>
  Layout_Type parse_layout_type(Doc const& doc)
  {
    if(strcmp(doc.GetString(), "vertical") == 0)
    {
      return Layout_Type::Vertical;
    }
    else if(strcmp(doc.GetString(), "horizontal") == 0)
    {
      return Layout_Type::Horizontal;
    }

    throw Bad_Layout{};
  }

  struct Bad_Element_Data {};
  template <class Doc>
  Element_Data parse_element_type(Doc const& doc)
  {
    if(strcmp(doc.GetString(), "text") == 0) { return Text{}; }
    else if(strcmp(doc.GetString(), "button") == 0) { return Button{}; }

    throw Bad_Element_Data{};
  }

  struct Bad_Alignment_Type {};
  template <class Doc>
  Alignment_Type parse_alignment_type(Doc const& doc)
  {
    if(strcmp(doc.GetString(), "center") == 0)
    { return Alignment_Type::Centered; }
    else if(strcmp(doc.GetString(), "top") == 0)
    { return Alignment_Type::Top; }
    else if(strcmp(doc.GetString(), "left") == 0)
    { return Alignment_Type::Left; }
    else if(strcmp(doc.GetString(), "bottom") == 0)
    { return Alignment_Type::Bottom; }
    else if(strcmp(doc.GetString(), "right") == 0)
    { return Alignment_Type::Right; }

    throw Bad_Alignment_Type {};
  }

  template <class Doc>
  Element_Data parse_text(Doc const& doc)
  {
    return Text{doc["text"].GetString()};
  }
  template <class Doc>
  Element_Data parse_button(Doc const& doc)
  {
    return Button{doc["text"].GetString(), doc["event"].GetString()};
  }

  Model::Model(rapidjson::Document const& json)
  {
    layout_ = parse_layout_type(json["layout"]);

    auto const& data = json["data"];
    for(auto iter = data.Begin(); iter != data.End(); ++iter)
    {
      auto align = parse_alignment_type((*iter)["aligned"]);
      auto elem = Element{align, parse_element_type((*iter)["type"])};

      if(elem.element.which() == 0) elem.element = parse_text(*iter);
      else elem.element = parse_button(*iter);

      elemts_.push_back(elem);
    }
  }
  Model::Model() noexcept : layout_(Layout_Type::Vertical) {}
} }
