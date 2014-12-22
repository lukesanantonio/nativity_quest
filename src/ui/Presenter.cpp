/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Presenter.h"
#include <numeric>
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
      auto& elemnts = model_->elements();

      int top_aligned = 0, bottom_aligned = 0;
      std::tie(top_aligned, bottom_aligned) = std::accumulate(begin(elemnts),
                                      end(elemnts), std::tuple<int, int>{0, 0},
      [](auto sum, auto const& elemnt)
      {
        if(elemnt.alignment == Alignment_Type::Top)
        {
          ++std::get<0>(sum);
        }
        if(elemnt.alignment == Alignment_Type::Bottom)
        {
          ++std::get<1>(sum);
        }
        return sum;
      });

      int top_aligned_done = 0, bottom_aligned_done = 0;

      // TODO find some sort order to make everything work, for now depend
      // on the order of the json array.
      for(auto& elem : elemnts)
      {
        auto pos = Vec<int>{};
        if(elem.alignment == Alignment_Type::Top)
        {
          auto where = top_aligned - top_aligned_done;
          ++top_aligned_done;

          // Add top padding.
          pos.y += 10;
          // Go down 100 pixels per item.
          pos.y += 100 * (where - 1);
        }
        if(elem.alignment == Alignment_Type::Bottom)
        {
          auto where = bottom_aligned - bottom_aligned_done;
          ++bottom_aligned_done;

          // Add top padding.
          pos.y = view.size().y - 10;
          // Go down 100 pixels per item.
          pos.y -= 100 * (where - 1);
        }

        auto text = boost::apply_visitor(Text_Visitor{}, elem.element);
        auto text_size = view.text_size(text);

        pos.x = view.size().x / 2 - text_size.x / 2;
        if(elem.element.which() == 1)
        {
          pos.y -= text_size.y;

          auto button = boost::get<Button>(elem.element);

          auto vol = Volume<int>{pos, text_size.x, text_size.y};
          view.box({vol, {0xff, 0xff, 0xff}});

          buttons_.push_back({button.event, vol});
        }
        view.label({text, pos, {0xff, 0xff, 0xff}});

        if(elem.element.which() == 1)
        {
          pos.y += text_size.y;
        }
      }
    }
  }

  void event_notify(SDL_Event const& event) noexcept {}

  void register_handler(std::string const& event,
                        std::function<void()>) noexcept {}
} }
