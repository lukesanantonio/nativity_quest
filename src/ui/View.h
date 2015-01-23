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

    inline void render() const noexcept { if(layed_out_) render_(); }

    virtual Vec<int> get_minimum_extents() const noexcept = 0;

    virtual void dispatch_event(SDL_Event const&) noexcept = 0;

    inline Volume<int> const& parent_volume() const noexcept;
    inline Volume<int> const& this_volume() const noexcept;

    std::string id;

  protected:
    Graphics_Desc& graphics_;
  private:
    Volume<int> parent_vol_;
    Volume<int> this_vol_;

    bool layed_out_ = false;

    virtual Volume<int> layout_() = 0;
    virtual void render_() const noexcept = 0;
  };

  inline Volume<int> const& View::parent_volume() const noexcept
  {
    return parent_vol_;
  }
  inline Volume<int> const& View::this_volume() const noexcept
  {
    return this_vol_;
  }

  inline bool View::layout(Vec<int> size)
  {
    return layout({{0, 0}, size.x, size.y});
  }
  inline bool View::layout(Volume<int> vol)
  {
    try
    {
      parent_vol_ = std::move(vol);
      this_vol_ = layout_();
      layed_out_ = true;
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
} }
