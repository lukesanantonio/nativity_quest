/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <functional>
namespace game { namespace ui
{
  template <class... Args>
  struct Event_Trigger
  {
    using func_t = std::function<void(Args...)>;

    inline void use_handler(func_t func) noexcept;

  protected:
    template <class... Params>
    inline void fire_handler(Params&&...) const noexcept;

  private:
    func_t handler_;
  };

  template <class... Args>
  inline void Event_Trigger<Args...>::use_handler(func_t func) noexcept
  {
    handler_ = func;
  }
  template <class... A>
  template <class... P>
  inline void Event_Trigger<A...>::fire_handler(P&&... a) const noexcept
  {
    if(handler_)
    {
      handler_(std::forward<P>(a)...);
    }
  }
} }
