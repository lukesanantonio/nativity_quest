/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Model.h"
#include "../common/json.h"
namespace game { namespace ui
{
  struct Bad_Element_Data {};
  template <class Doc>
  Element_Data parse_element_type(Doc const& doc)
  {
    if(strcmp(doc.GetString(), "text") == 0) { return Text{}; }
    else if(strcmp(doc.GetString(), "button") == 0) { return Button{}; }
    else if(strcmp(doc.GetString(), "sprite") == 0) { return Sprite{}; }

    throw Bad_Element_Data{};
  }

  struct Bad_Alignment_Type {};
  template <class Doc>
  auto parse_alignment(Doc const& doc) -> decltype(auto)
  {
    auto hor_string = doc["horizontal"].GetString();
    auto vert_string = doc["vertical"].GetString();

    auto ret_tup = std::tuple<Horizontal_Alignment, Vertical_Alignment>{};

    if(strcmp(hor_string, "left") == 0)
    { std::get<0>(ret_tup) = Horizontal_Alignment::Left; }
    if(strcmp(hor_string, "center") == 0)
    { std::get<0>(ret_tup) = Horizontal_Alignment::Center; }
    if(strcmp(hor_string, "right") == 0)
    { std::get<0>(ret_tup) = Horizontal_Alignment::Right; }

    if(strcmp(vert_string, "top") == 0)
    { std::get<1>(ret_tup) = Vertical_Alignment::Top; }
    if(strcmp(vert_string, "center") == 0)
    { std::get<1>(ret_tup) = Vertical_Alignment::Center; }
    if(strcmp(vert_string, "bottom") == 0)
    { std::get<1>(ret_tup) = Vertical_Alignment::Bottom; }

    return ret_tup;
  }

  template <class Doc>
  Element_Data parse_text(Doc const& doc)
  {
    auto text = Text{doc["text"].GetString(), "white"};
    if_has_member(doc, "color",
    [&text](auto const& elem)
    {
      text.col = elem.GetString();
    });
    return text;
  }
  template <class Doc>
  Element_Data parse_button(Doc const& doc)
  {
    auto but = Button{doc["text"].GetString(), doc["event"].GetString(),
                      "white"};
    if_has_member(doc, "color",
    [&but](auto const& elem)
    {
      but.col = elem.GetString();
    });
    return but;
  }

  template <class Doc>
  Element_Data parse_sprite(Doc const& doc)
  {
    auto sprite = Sprite{doc["src"].GetString(), doc["scale"].GetDouble()};
    if_has_member(doc, "border",
    [&sprite](auto const& elem)
    {
      sprite.border_col = elem.GetString();
    });
    return sprite;
  }

  Model::Model(rapidjson::Document const& json)
  {
    for(auto iter = json.Begin(); iter != json.End(); ++iter)
    {
      // Parse element specific data.
      auto e = Element{parse_element_type((*iter)["type"])};

      // Parse alignment
      std::tie(e.h_align, e.v_align) = parse_alignment((*iter)["alignment"]);

      // Parse (optional) padding.
      if_has_member(*iter, "padding",
      [&](auto const& val)
      {
        if_has_member(val, "left",
        [&](auto const& num)
        {
          e.padding.left = num.GetInt();
        });
        if_has_member(val, "right",
        [&](auto const& num)
        {
          e.padding.right = num.GetInt();
        });
        if_has_member(val, "top",
        [&](auto const& num)
        {
          e.padding.top = num.GetInt();
        });
        if_has_member(val, "bottom",
        [&](auto const& num)
        {
          e.padding.bottom = num.GetInt();
        });
      });


      if(e.element.which() == 0) e.element = parse_text(*iter);
      else if(e.element.which() == 1) e.element = parse_button(*iter);
      else e.element = parse_sprite(*iter);

      // Parse autohide parameter (if provided).
      if_has_member(*iter, "autohide",
      [&](auto const& val)
      {
        e.autohide = val.GetDouble();
      });

      elemts_.push_back(e);
    }
  }
  Model::Model() noexcept {}
} }
