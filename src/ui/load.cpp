/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "load.h"
#include "../common/json.h"
#include "layouts/linear_layout.h"
#include "layouts/side_layout.h"
#include "layouts/grid_layout.h"
#include "views/label.h"
#include "views/empty.h"
#include "views/sprite.h"
#include "views/bar.h"
namespace game { namespace ui
{
  template <class T>
  std::string typeof(T const& obj) noexcept
  {
    return obj["type"].GetString();
  }

  template <class T>
  Linear_Layout::Orient orientof(T const& doc)
  {
    if(strcmp(doc["orientation"].GetString(), "vertical") == 0)
    {
      return Linear_Layout::Orient::Vertical;
    }
    if(strcmp(doc["orientation"].GetString(), "horizontal") == 0)
    {
      return Linear_Layout::Orient::Horizontal;
    }
    throw Bad_Orientation{};
  }

  template <class T>
  Padding load_padding(T const& doc)
  {
    auto ret = Padding{};
    if(!doc.HasMember("padding")) return ret;

    auto const& pad_obj = doc["padding"];
    if_has_member(pad_obj, "left",
    [&](auto const& val)
    {
      ret.left = val.GetInt();
    });
    if_has_member(pad_obj, "right",
    [&](auto const& val)
    {
      ret.right = val.GetInt();
    });
    if_has_member(pad_obj, "top",
    [&](auto const& val)
    {
      ret.top = val.GetInt();
    });
    if_has_member(pad_obj, "bottom",
    [&](auto const& val)
    {
      ret.bottom = val.GetInt();
    });

    return ret;
  }

  template <class T>
  Alignment load_alignment(T const& doc)
  {
    auto align = Alignment{};

    // Set defaults.
    align.horizontal = Horizontal_Alignment::Center;
    align.vertical = Vertical_Alignment::Center;

    auto const& align_obj = doc["alignment"];

    if(align_obj.HasMember("vertical"))
    {
      if(strcmp(align_obj["vertical"].GetString(), "top") == 0)
      {
        align.vertical = Vertical_Alignment::Top;
      }
      else if(strcmp(align_obj["vertical"].GetString(), "center") == 0)
      {
        align.vertical = Vertical_Alignment::Center;
      }
      else if(strcmp(align_obj["vertical"].GetString(), "bottom") == 0)
      {
        align.vertical = Vertical_Alignment::Bottom;
      }
      else
      {
        throw Invalid_Alignment{align_obj["vertical"].GetString()};
      }
    }
    if(align_obj.HasMember("horizontal"))
    {
      if(strcmp(align_obj["horizontal"].GetString(), "left") == 0)
      {
        align.horizontal = Horizontal_Alignment::Left;
      }
      else if(strcmp(align_obj["horizontal"].GetString(), "center") == 0)
      {
        align.horizontal = Horizontal_Alignment::Center;
      }
      else if(strcmp(align_obj["horizontal"].GetString(), "right") == 0)
      {
        align.horizontal = Horizontal_Alignment::Right;
      }
      else
      {
        throw Invalid_Alignment{align_obj["vertical"].GetString()};
      }
    }

    return align;
  }

  template <class T>
  Color load_color(T const& doc)
  {
    Color c;

    c.red   = doc["red"].GetChar();
    c.green = doc["green"].GetChar();
    c.blue  = doc["blue"].GetChar();

    return c;
  }

  template <class T>
  Shared_View load_view(Game& game, T const& doc) noexcept
  {
    std::string id = doc.HasMember("id") ? doc["id"].GetString() : "";

    if(typeof(doc) == "linear_layout")
    {
      Linear_Layout view{game.graphics};
      view.id = id;

      auto const& children = doc["children"];
      view.orientation = orientof(doc);
      for(auto iter = children.Begin(); iter != children.End(); ++iter)
      {
        Linear_Layout_Params layout;
        // Load the weight if provided.
        layout.weight =
                   iter->HasMember("weight") ?  (*iter)["weight"].GetInt() : 1;
        view.push_child(load_view(game, *iter), layout);
      }
      return std::make_shared<Linear_Layout>(std::move(view));
    }
    else if(typeof(doc) == "side_layout")
    {
      Side_Layout view{game.graphics};
      view.id = id;

      auto const& children = doc["children"];
      for(auto iter = children.Begin(); iter != children.End(); ++iter)
      {
        Side_Layout_Params layout;

        layout.padding = load_padding(*iter);
        layout.alignment = load_alignment(*iter);

        view.push_child(load_view(game, *iter), layout);
      }

      return std::make_shared<Side_Layout>(std::move(view));
    }
    else if(typeof(doc) == "grid_layout")
    {
      Grid_Layout view{game.graphics};

      auto const& children = doc["children"];
      for(auto iter = children.Begin(); iter != children.End(); ++iter)
      {
        Grid_Layout_Params layout;

        layout.row = (*iter)["row"].GetInt();
        layout.col = (*iter)["col"].GetInt();

        view.push_child(load_view(game, *iter), layout);
      }

      return std::make_shared<Grid_Layout>(std::move(view));
    }
    else if(typeof(doc) == "label")
    {
      Label label{game.graphics, game.font};
      label.id = id;

      label.str(doc["text"].GetString());
      label.size(doc["size"].GetInt());
      label.color({0xff, 0xff, 0xff});

      return std::make_shared<Label>(std::move(label));
    }
    else if(typeof(doc) == "sprite")
    {
      auto sprite = Sprite{game.graphics, doc["src"].GetString()};

      sprite.scale(doc["scale"].GetDouble());

      return std::make_shared<Sprite>(std::move(sprite));
    }
    else if(typeof(doc) == "bar")
    {
      auto bar = Bar{game.graphics};

      return std::make_shared<Bar>(std::move(bar));
    }
    else if(typeof(doc) == "empty")
    {
      auto view = Empty{game.graphics};
      view.id = id;
      return std::make_shared<Empty>(std::move(view));
    }
    return nullptr;
  }
  Shared_View load(Game& g, std::string file) noexcept
  {
    auto doc = parse_json(file);
    return load_view(g, doc);
  }
} }
