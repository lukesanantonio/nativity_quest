/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "load.h"
#include "../common/json.h"
#include "layouts/linear_layout.h"
#include "views/label.h"
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
  Shared_View load_view(Game& game, T const& doc) noexcept
  {
    if(typeof(doc) == "linear_layout")
    {
      Linear_Layout view{game.graphics};
      auto const& children = doc["children"];
      view.orientation = orientof(doc);
      for(auto iter = children.Begin(); iter != children.End(); ++iter)
      {
        view.push_child(load_view(game, *iter));
      }
      return std::make_shared<Linear_Layout>(std::move(view));
    }
    else if(typeof(doc) == "label")
    {
      Label label{game.graphics, game.font};

      label.str(doc["text"].GetString());
      label.size(doc["size"].GetInt());
      label.color({0xff, 0xff, 0xff});

      return std::make_shared<Label>(std::move(label));
    }
    return nullptr;
  }
  Shared_View load(Game& g, std::string file) noexcept
  {
    auto doc = parse_json(file);
    return load_view(g, doc);
  }
} }
