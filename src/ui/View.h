/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../common/volume.h"
#include "../game/Graphics_Desc.h"

#include <memory>
namespace game { namespace ui
{
  struct View;
  using Shared_View = std::shared_ptr<View>;

  struct Small_Volume_Error {};

  struct View
  {
    View(Graphics_Desc& graphics) noexcept : graphics_(graphics) {}
    virtual ~View() {}

    inline bool layout(Vec<int>);
    inline bool layout(Volume<int>);
    inline void render() const noexcept;

    virtual Vec<int> get_minimum_extents() const noexcept = 0;

    virtual void dispatch_event(SDL_Event const&) noexcept = 0;

  protected:
    inline Volume<int> const& volume_() const noexcept;

    Graphics_Desc& graphics_;
  private:
    Volume<int> layed_out_vol_;
    bool layed_out_ = false;

    inline virtual void layout_(Volume<int>) {}
    virtual void render_() const noexcept = 0;
  };

  inline Volume<int> const& View::volume_() const noexcept
  {
    return layed_out_vol_;
  }

  inline bool View::layout(Vec<int> size)
  {
    return layout({{0, 0}, size.x, size.y});
  }
  inline bool View::layout(Volume<int> vol)
  {
    try
    {
      layout_(std::move(vol));
      layed_out_ = true;
      layed_out_vol_ = vol;
    }
    catch(Small_Volume_Error& e)
    {
      return false;
    }
    catch(...)
    {
      throw;
    }
    return true;
  }
  inline void View::render() const noexcept
  {
    if(layed_out_)
    {
      render_();
    }
  }
} }
