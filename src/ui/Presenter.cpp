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
    if(col == "black")
    {
      return {0x00, 0x00, 0x00};
    }
    return {0xff, 0xff, 0xff};
  }

  void Presenter::present(Model& model, View& view,
                          Vec<int> bounds) const noexcept
  {
    buttons_.clear();
    labels_.clear();

    constexpr auto text_height = 34;

    // Go through each 'side' element first.
    for(auto& elem : model.elements)
    {
      if(!elem.is_visible) continue;

      if(elem.align.which() != 1) continue;

      auto side = boost::get<Side>(elem.align);

      // Only Label_View elements support 'side' layout.
      if(elem.element.which() != 3) continue;

      auto& label_view = boost::get<Label_View>(elem.element);
      // And it only supports bottom.
      if(side == Side::Bottom)
      {
        // Find the height.
        auto extents_vec = std::vector<Vec<int> >{};
        for(auto str : label_view.labels)
        {
          extents_vec.push_back(view.text_size(str, text_height));
        }

        using std::begin; using std::end;
        auto max_vec = std::max_element(begin(extents_vec), end(extents_vec),
        [](auto const& v1, auto const& v2)
        {
          return v1.y < v2.y;
        });

        auto height = max_vec->y * label_view.rows;
        bounds.y -= height;

        auto max_per_row = label_view.labels.size() / label_view.rows +
                           label_view.labels.size() % label_view.rows;

        auto vol = Volume<int>{{0, bounds.y}, bounds.x, height};
        auto cell_width = vol.width / max_per_row;
        auto start = Vec<int>{cell_width / 2, bounds.y};
        auto cur_pos = start;

        auto color = get_color_from_str(label_view.col);

        // Render the background.
        view.box({vol, color});

        int i = 0;
        for(int str_i = 0; str_i < label_view.labels.size(); ++str_i)
        {
          auto cell_vol = Volume<int>{{cur_pos.x - cell_width / 2, cur_pos.y},
                                      (int)cell_width,
                                      height / label_view.rows};

          // Render the actual text, centered in the cell.
          auto pos = Vec<int>{cell_vol.pos.x + (cell_width / 2) -
                              (extents_vec[str_i].x / 2),
                              cell_vol.pos.y + (cell_vol.height / 2) -
                              (extents_vec[str_i].y / 2)};
          view.label({label_view.labels[str_i], text_height,
                      pos, color});

          if(label_view.is_selectable)
          {
            labels_.push_back({label_view, str_i,
                               {{cur_pos.x - cell_width / 2, cur_pos.y},
                                (int) cell_width,
                                vol.height / label_view.rows}});

            // Render the marker if this item is selected.
            if(label_view.selected == str_i)
            {
              view.box({cell_vol, color});
            }
          }

          cur_pos.x += cell_width;

          if(i == label_view.labels.size() / label_view.rows - 1)
          {
            cur_pos.y += vol.height / label_view.rows;
            cur_pos.x = start.x;
          }

          ++i;
        }
      }
    }

    // Now go through each non-side element.
    for(auto& elem : model.elements)
    {
      if(!elem.is_visible) continue;

      if(elem.align.which() != 0) continue;
      auto& align = boost::get<Alignment>(elem.align);
      auto h_align = align.horizontal;
      auto v_align = align.vertical;

      auto pos = Vec<int>{};
      if(h_align == Horizontal_Alignment::Center) pos.x = bounds.x / 2;
      else if(h_align == Horizontal_Alignment::Right) pos.x = bounds.x;

      if(v_align == Vertical_Alignment::Center) pos.y = bounds.y / 2;
      else if(v_align == Vertical_Alignment::Bottom) pos.y = bounds.y;

      if(h_align == Horizontal_Alignment::Left)
      { pos.x += elem.padding.left; }
      else if(h_align == Horizontal_Alignment::Right)
      { pos.x -= elem.padding.right; }

      if(v_align == Vertical_Alignment::Top)
      { pos.y += elem.padding.top; }
      else if(v_align == Vertical_Alignment::Bottom)
      { pos.y -= elem.padding.bottom; }

      if(elem.element.which() == 0 || elem.element.which() == 1)
      {
        auto text = boost::apply_visitor(Text_Visitor{}, elem.element);
        auto text_size = view.text_size(text, text_height);

        auto vol = Volume<int>{{pos.x - 1, pos.y - 1},
                               text_size.x + 2, text_size.y + 2};

        align_vol(vol, h_align, v_align);

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
      else if(elem.element.which() == 2)
      {
        auto spr_elem = boost::get<ui::Sprite>(elem.element);
        auto sprite = sprites_->get_sprite(spr_elem.src, spr_elem.frame);

        auto src = Volume<int>{{0, 0}, sprite->surface()->w,
                               sprite->surface()->h};
        if(spr_elem.vol)
        {
          src = spr_elem.vol.value();
        }

        auto dst = Volume<int>{pos,
                               (int) (sprite->surface()->w * spr_elem.scale),
                               (int) (sprite->surface()->h * spr_elem.scale)};

        align_vol(dst, h_align, v_align);

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
      else
      {
        auto vol = Volume<int>{pos, bounds.x / 7, bounds.y / 14};
        align_vol(vol, h_align, v_align);

        auto& bar = boost::get<Bar>(elem.element);
        auto color = get_color_from_str(bar.col);

        view.box({vol, color});

        vol.width *= bar.cur / (double) bar.max;
        view.box({vol, color});
      }
    }
  }

  bool Presenter::event_notify(SDL_Event const& event) noexcept
  {
    if(!handle_events_) return false;

    bool ret = false;
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
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
      for(auto const& area : labels_)
      {
        auto pt = Vec<int>{event.motion.x, event.motion.y};
        if(is_in(area.vol, pt) && !area.lv.done)
        {
          area.lv.selected = area.index;
          area.lv.done = true;
          ret = true;
        }
      }
    }
    else if(event.type == SDL_MOUSEMOTION)
    {
      for(auto const& area : labels_)
      {
        auto pt = Vec<int>{event.motion.x, event.motion.y};
        if(is_in(area.vol, pt) && !area.lv.done)
        {
          if(area.lv.selected != area.index)
          {
            area.lv.selected = area.index;
            ret = true;
          }
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
