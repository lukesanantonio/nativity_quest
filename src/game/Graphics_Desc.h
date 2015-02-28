/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "SDL.h"

#include "../common/vec.h"

#include "../assets/desc/game.h"
namespace game
{
  struct Bad_Init {};

  struct Bad_Display {};

  struct Bad_Window {};
  struct Bad_Renderer {};

  struct Graphics_Desc
  {
    Graphics_Desc(assets::Game const& game_decl);
    Graphics_Desc(Graphics_Desc&& g) noexcept;

    ~Graphics_Desc() noexcept;

    inline Vec<int> size() const noexcept
    { return extents_; }

    SDL_Window* window;
    SDL_Renderer* renderer;

  private:
    Vec<int> extents_;
  };
}
