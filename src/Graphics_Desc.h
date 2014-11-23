/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include "SDL.h"

#include "common/vec.h"
namespace game
{
  struct Bad_Window {};
  struct Bad_Renderer {};

  struct Graphics_Desc
  {
    Graphics_Desc(std::string const& title, Vec<int> extents);
    ~Graphics_Desc() noexcept;

    int get_width() const noexcept;
    int get_height() const noexcept;

    SDL_Window* window;
    SDL_Renderer* renderer;
  private:
    Vec<int> extents_;
  };
}
