/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Presenter.h"
namespace game { namespace ui
{
  void Presenter::present(View& view) const noexcept
  {
    struct Text_Visitor : public boost::static_visitor<std::string>
    {
      std::string operator()(Text const& text) const noexcept
      {
        return text.str;
      }
      std::string operator()(Button const& button) const noexcept
      {
        return button.str;
      }
    };

    if(model_->layout() == Layout_Type::Vertical)
    {
      for(auto& elem : model_->elements())
      {
        auto text = boost::apply_visitor(Text_Visitor{}, elem.element);
        auto text_size = view.text_size(text);

        auto pos = Vec<int>{};
        if(elem.alignment == Alignment_Type::Top)
        {
          pos.y = 10;
          pos.x = view.size().x / 2 - text_size.x / 2;
        }
        else if(elem.alignment == Alignment_Type::Bottom)
        {
          pos.y = view.size().y - 10 - text_size.y;
          pos.x = view.size().x / 2 - text_size.x / 2;
        }

        view.label({text, pos, {0xff, 0xff, 0xff}});
        if(elem.element.which() == 1)
        {
          view.box({{pos, text_size.x, text_size.y}, {0xee, 0xee, 0xee}});
        }
      }
    }
  }

  void event_notify(SDL_Event const& event) noexcept {}

  void register_handler(std::string const& event,
                        std::function<void()>) noexcept {}
} }
