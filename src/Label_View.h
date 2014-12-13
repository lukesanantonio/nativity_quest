/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "volume.h"
#include "Graphics_Desc.h"
namespace game
{
  template <class Input, class Layout>
  struct Label_View
  {
    explicit Label_View(Volume<int> const& vol = Volume<int>{},
                            Input i = Input{},
                            Layout l = Layout{}) noexcept
                            : vol_(vol),
                              input_(std::move(i)),
                              layout_(std::move(l)) {}

    inline void handle_event(SDL_Event const&) noexcept;

    inline Input& input() noexcept;
    inline Input const& input() const noexcept;

    inline Layout& layout() noexcept;
    inline Layout const& layout() const noexcept;

    inline Volume<int> const& vol() noexcept;
    inline void vol(Volume<int> vol) noexcept;

    inline void add_label(std::string const&) noexcept;
    inline void add_label(Managed_Label const& l) noexcept;

    inline std::vector<Managed_Label> const& labels() const noexcept;
    inline std::vector<Managed_Label>& labels() noexcept;

    inline void remove_label(std::vector<Managed_Label>::iterator) noexcept;

    void render(Graphics_Desc& g) const noexcept;
  private:
    Volume<int> vol_;

    Input input_;
    Layout layout_;

    std::vector<Managed_Label> labels_;
  };

  template <class Input, class Layout> inline void Label_View<Input, Layout>::
  handle_event(SDL_Event const& event) noexcept
  {
    input_.handle_event(event);
  }

  template <class Input, class Layout>
  inline Input& Label_View<Input, Layout>::input() noexcept
  {
    return input_;
  }
  template <class Input, class Layout>
  inline Input const& Label_View<Input, Layout>::input() const noexcept
  {
    return input_;
  }
  template <class Input, class Layout>
  inline Layout& Label_View<Input, Layout>::layout() noexcept
  {
    return layout_;
  }
  template <class Input, class Layout>
  inline Layout const& Label_View<Input, Layout>::layout() const noexcept
  {
    return layout_;
  }

  template <class Input, class Layout>
  inline Volume<int> const& Label_View<Input, Layout>::vol() noexcept
  {
    return vol_;
  }
  template <class Input, class Layout> inline void Label_View<Input, Layout>::
  vol(Volume<int> vol) noexcept
  {
    vol_ = vol;
  }
  template <class Input, class Layout> inline void Label_View<Input, Layout>::
  add_label(std::string const& str) noexcept
  {
    Managed_Label l;
    l.str(str);
    labels_.push_back(l);
  }
  template <class Input, class Layout> inline void Label_View<Input, Layout>::
  add_label(Managed_Label const& l) noexcept
  {
    labels_.push_back(l);
  }

  template <class Input, class Layout> inline std::vector<Managed_Label> const&
  Label_View<Input, Layout>::labels() const noexcept
  {
    return labels_;
  }
  template <class Input, class Layout> inline std::vector<Managed_Label>&
  Label_View<Input, Layout>::labels() noexcept
  {
    return labels_;
  }

  template <class Input, class Layout> inline void Label_View<Input, Layout>::
  remove_label(std::vector<Managed_Label>::iterator i) noexcept
  {
    labels_.erase(i);
  }

  template <class Input, class Layout>
  void Label_View<Input, Layout>::render(Graphics_Desc& g) const noexcept
  {
    // Color, hardcoded for now.
    SDL_SetRenderDrawColor(g.renderer, 0xff, 0xff, 0xff, 0xff);

    // Render our volume.
    SDL_Rect vol_rect;
    vol_rect.x = vol_.pos.x;
    vol_rect.y = vol_.pos.y;
    vol_rect.w = vol_.width;
    vol_rect.h = vol_.height;
    SDL_RenderFillRect(g.renderer, &vol_rect);

    // Layout each inventory item
    layout_.layout(*this);

    for(auto const& label : labels_)
    {
      label.render(g);
    }

    // Render anything the input controller wants.
    input_.render(g, *this);
  }
}
