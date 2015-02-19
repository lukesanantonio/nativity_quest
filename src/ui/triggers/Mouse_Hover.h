/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../../common/vec.h"
#include "../Event_Trigger.h"
namespace game { namespace ui
{
  struct Mouse_Hover : public Event_Trigger
  {
    template <class F>
    explicit Mouse_Hover(F const& func, bool use_parent_vol = false) noexcept
                         : func_(func), use_parent_vol_(use_parent_vol) {}
    bool try_trigger(View& v, SDL_Event const& event) const noexcept override;

  private:
    std::function<void (Vec<int>)> func_;
    bool use_parent_vol_;
  };
} }
