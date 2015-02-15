/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../Event_Trigger.h"
#include "../View.h"
namespace game { namespace ui
{
  struct Mouse_Click : public Event_Trigger
  {
    template <class F> Mouse_Click(F const& f) noexcept : func_(f) {}

    bool try_trigger(View& v,
                       SDL_Event const& event) const noexcept override;
  private:
    std::function<void (Vec<int>)> func_;
  };
} }
