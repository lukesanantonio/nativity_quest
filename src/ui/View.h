/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../common/volume.h"
#include "../game/Graphics_Desc.h"

#include "Event_Trigger.h"

#include <memory>
#include <vector>
namespace game { namespace ui
{
  struct View;
  using Shared_View = std::shared_ptr<View>;

  struct Small_Volume_Error {};

  struct Trigger_Connection
  {
    // Some pointer or something...
    void disconnect() noexcept {}
  };

  struct View
  {
    View(Graphics_Desc& graphics) noexcept : graphics_(graphics) {}
    virtual ~View() {}

    inline bool layout(Vec<int>);
    inline bool layout(Volume<int>);

    inline void render() const noexcept { if(layed_out_) render_(); }

    virtual Vec<int> get_minimum_extents() const noexcept = 0;

    inline virtual void dispatch_event(SDL_Event const&) noexcept;

    inline Volume<int> const& parent_volume() const noexcept;
    inline Volume<int> const& this_volume() const noexcept;

    inline Trigger_Connection
    add_event_trigger(std::shared_ptr<Event_Trigger> const&) noexcept;

    template <class T, class... Args>
    inline Trigger_Connection add_event_trigger(Args&&...) noexcept;

    std::string id;

  protected:
    Graphics_Desc& graphics_;
  private:
    Volume<int> parent_vol_;
    Volume<int> this_vol_;

    bool layed_out_ = false;

    virtual Volume<int> layout_() = 0;
    virtual void render_() const noexcept = 0;

    std::vector<std::shared_ptr<Event_Trigger> > event_triggers_;
  };

  inline void View::dispatch_event(SDL_Event const& e) noexcept
  {
    for(auto shared_trigger : event_triggers_)
    {
      shared_trigger->try_trigger(*this, e);
    }
  }

  inline Volume<int> const& View::parent_volume() const noexcept
  {
    return parent_vol_;
  }
  inline Volume<int> const& View::this_volume() const noexcept
  {
    return this_vol_;
  }

  inline Trigger_Connection
  View::add_event_trigger(std::shared_ptr<Event_Trigger> const& et) noexcept
  {
    event_triggers_.push_back(std::move(et));
    return {};
  }

  template <class T, class... Args>
  inline Trigger_Connection View::add_event_trigger(Args&&... args) noexcept
  {
    auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
    event_triggers_.push_back(std::move(ptr));
    return {};
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
