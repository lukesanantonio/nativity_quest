/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Presenter.h"
#include "../common/Color.h"
#include <numeric>
namespace game { namespace ui
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

    template <class T>
    std::string operator()(T const& t) const noexcept { return ""; }
  };

  void align_vol(Volume<int>& vol, Horizontal_Alignment& h_align,
                 Vertical_Alignment& v_align) noexcept
  {
    if(h_align == Horizontal_Alignment::Right) { vol.pos.x -= vol.width; }
    else if(h_align == Horizontal_Alignment::Center)
    { vol.pos.x -= vol.width / 2; }

    if(v_align == Vertical_Alignment::Bottom) { vol.pos.y -= vol.height; }
    else if(v_align == Vertical_Alignment::Center)
    { vol.pos.y -= vol.height / 2; }
  }

  Color get_color_from_str(std::string col) noexcept
  {
    if(col == "white")
    {
      return {0xff, 0xff, 0xff};
    }
    else if(col == "black")
    {
      return {0x00, 0x00, 0x00};
    }
  }

  void Presenter::present(Model& model, View& view,
                          Vec<int> bounds) const noexcept
  {
    buttons_.clear();

    constexpr auto text_height = 34;

    for(auto& elem : model.elements())
    {
      auto pos = Vec<int>{};
      if(elem.h_align == Horizontal_Alignment::Center) pos.x = bounds.x / 2;
      else if(elem.h_align == Horizontal_Alignment::Right) pos.x = bounds.x;

      if(elem.v_align == Vertical_Alignment::Center) pos.y = bounds.y / 2;
      else if(elem.v_align == Vertical_Alignment::Bottom) pos.y = bounds.y;

      if(elem.h_align == Horizontal_Alignment::Left)
      { pos.x += elem.padding.left; }
      else if(elem.h_align == Horizontal_Alignment::Right)
      { pos.x -= elem.padding.right; }

      if(elem.v_align == Vertical_Alignment::Top)
      { pos.y += elem.padding.top; }
      else if(elem.v_align == Vertical_Alignment::Bottom)
      { pos.y -= elem.padding.bottom; }

      if(elem.element.which() == 0 || elem.element.which() == 1)
      {
        auto text = boost::apply_visitor(Text_Visitor{}, elem.element);
        auto text_size = view.text_size(text, text_height);

        auto vol = Volume<int>{{pos.x - 1, pos.y - 1},
                               text_size.x + 2, text_size.y + 2};

        align_vol(vol, elem.h_align, elem.v_align);

        if(elem.element.which() == 1)
        {
          auto button = boost::get<Button>(elem.element);

          view.box({vol, get_color_from_str(button.col)});
          buttons_.push_back({button.event, vol});
        }

        ++vol.pos.x;
        ++vol.pos.y;

        Color text_color;

        if(elem.element.which() == 0)
        {
          auto text = boost::get<Text>(elem.element);
          text_color = get_color_from_str(text.col);
        }
        else
        {
          auto button = boost::get<Button>(elem.element);
          text_color = get_color_from_str(button.col);
        }
        view.label({text, text_height, vol.pos, text_color});
      }
      else
      {
        auto spr_elem = boost::get<ui::Sprite>(elem.element);
        auto sprite = sprites_->get_sprite(spr_elem.src);

        auto src = Volume<int>{{0, 0}, sprite->surface()->w,
                               sprite->surface()->h};

        auto dst = Volume<int>{pos,
                               (int) (sprite->surface()->w * spr_elem.scale),
                               (int) (sprite->surface()->h * spr_elem.scale)};

        align_vol(dst, elem.h_align, elem.v_align);

        view.image({sprite, src, dst});

        if(spr_elem.border_col)
        {
          Color col = get_color_from_str(spr_elem.border_col.value());

          --dst.pos.x;
          --dst.pos.y;
          dst.width += 2;
          dst.height += 2;

          view.box({dst, col});
        }
      }
    }
  }

  bool Presenter::event_notify(SDL_Event const& event) noexcept
  {
    if(!handle_events_) return false;
    if(event.type != SDL_MOUSEBUTTONDOWN) return false;

    bool ret = false;
    for(auto const& area : buttons_)
    {
      auto pt = Vec<int>{event.button.x, event.button.y};
      if(is_in(area.vol, pt))
      {
        using std::begin; using std::end;
        auto handler_find = std::find_if(begin(events_), end(events_),
        [&area](auto const& handler)
        {
          return handler.event == area.event;
        });

        if(handler_find != end(events_))
        {
          handler_find->handler(pt);
          ret = true;
        }
      }
    }
    return ret;
  }

  void Presenter::use_handler(std::string const& event_str,
                              event_func_t func) noexcept
  {
    using std::begin; using std::end;
    auto handler_find = std::find_if(begin(events_), end(events_),
    [&event_str](auto const& handler)
    {
      return handler.event == event_str;
    });

    if(handler_find == end(events_)) events_.push_back({event_str, func});
    else handler_find->handler = func;
  }
} }
