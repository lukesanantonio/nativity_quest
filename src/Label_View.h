/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "volume.h"
#include "Graphics_Desc.h"
#include "Sprite_Container.h"
namespace game
{
  template <class Control>
  struct Label_View
  {
    explicit Label_View(Volume<int> const& vol = Volume<int>{},
                        Control c = Control{}) noexcept
                        : vol_(vol), control_(std::move(c)) {}

    inline void handle_event(SDL_Event const&) noexcept;

    inline Control& control() noexcept;
    inline Control const& control() const noexcept;

    inline Volume<int> const& vol() const noexcept;
    inline void vol(Volume<int> vol) noexcept;

    inline void add_label(std::string const&) noexcept;
    inline void add_label(Managed_Label const& l) noexcept;

    inline std::vector<Managed_Label> const& labels() const noexcept;
    inline std::vector<Managed_Label>& labels() noexcept;

    inline void remove_label(std::vector<Managed_Label>::iterator) noexcept;

    inline void layout(Graphics_Desc& g) noexcept;
    void render(Graphics_Desc& g, Sprite_Container&) const noexcept;
  private:
    Volume<int> vol_;

    Control control_;

    std::vector<Managed_Label> labels_;
  };

  template <class Control> inline void Label_View<Control>::
  handle_event(SDL_Event const& event) noexcept
  {
    control_.handle_event(event);
  }

  template <class Control>
  inline Control& Label_View<Control>::control() noexcept
  {
    return control_;
  }
  template <class Control>
  inline Control const& Label_View<Control>::control() const noexcept
  {
    return control_;
  }

  template <class Control>
  inline Volume<int> const& Label_View<Control>::vol() const noexcept
  {
    return vol_;
  }
  template <class Control> inline void Label_View<Control>::
  vol(Volume<int> vol) noexcept
  {
    vol_ = vol;
  }
  template <class Control> inline void Label_View<Control>::
  add_label(std::string const& str) noexcept
  {
    Managed_Label l;
    l.str(str);
    labels_.push_back(l);
  }
  template <class Control> inline void Label_View<Control>::
  add_label(Managed_Label const& l) noexcept
  {
    labels_.push_back(l);
  }

  template <class Control> inline std::vector<Managed_Label> const&
  Label_View<Control>::labels() const noexcept
  {
    return labels_;
  }
  template <class Control> inline std::vector<Managed_Label>&
  Label_View<Control>::labels() noexcept
  {
    return labels_;
  }

  template <class Control> inline void Label_View<Control>::
  remove_label(std::vector<Managed_Label>::iterator i) noexcept
  {
    labels_.erase(i);
  }

  template <class Control>
  inline void Label_View<Control>::layout(Graphics_Desc& g) noexcept
  {
    control_.layout(*this, g);
  }

  template <class Control>
  void Label_View<Control>::render(Graphics_Desc& g,
                                   Sprite_Container& sprites) const noexcept
  {
    control_.pre_render(g, *this, sprites);

    // Color, hardcoded for now.
    SDL_SetRenderDrawColor(g.renderer, 0xff, 0xff, 0xff, 0xff);

    // Render our volume.
    SDL_Rect vol_rect;
    vol_rect.x = vol_.pos.x;
    vol_rect.y = vol_.pos.y;
    vol_rect.w = vol_.width;
    vol_rect.h = vol_.height;
    SDL_RenderFillRect(g.renderer, &vol_rect);

    for(auto const& label : labels_)
    {
      label.render(g);
    }

    // Render anything the input controller wants.
    control_.render(g, *this, sprites);
  }
}
