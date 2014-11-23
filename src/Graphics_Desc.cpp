/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Graphics_Desc.h"
namespace game
{
  Graphics_Desc::Graphics_Desc(std::string const& title, Vec<int> extents)
  {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, extents.x, extents.y,
                              0);
    if(!window) throw Bad_Window{};

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
      SDL_DestroyWindow(window);
      throw Bad_Renderer{};
    }
  }
  Graphics_Desc::~Graphics_Desc() noexcept
  {
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
  }
}
