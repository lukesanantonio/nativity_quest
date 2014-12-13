/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"
#include "Label_View.h"
#include "Graphics_Desc.h"
namespace game
{
  struct Vanilla_Control
  {
    void handle_event(SDL_Event const& event) noexcept {}
    void layout(Label_View<Vanilla_Control>& view, Graphics_Desc& g) noexcept;
    void render(Graphics_Desc& g,
                Label_View<Vanilla_Control> const& view) const noexcept {}
  };
}
