/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Graphics_Desc.h"
namespace game
{
  Graphics_Desc::Graphics_Desc(decl::Game const& game_decl)
                               : extents_(game_decl.size)
  {
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
      throw Bad_Init{};
    }

    // Use the first display mode.
    if(extents_.x == 0 && extents_.y == 0 && game_decl.fullscreen)
    {
      if(SDL_GetNumVideoDisplays() < 1)
      {
        throw Bad_Display{};
      }

      SDL_DisplayMode display;
      if(SDL_GetCurrentDisplayMode(0, &display) != 0)
      {
        throw Bad_Display{};
      };

      extents_.x = display.w;
      extents_.y = display.h;
    }

    auto window_flags = 0;
    if(game_decl.fullscreen)
    {
      window_flags |= SDL_WINDOW_FULLSCREEN;
    }
    window = SDL_CreateWindow(game_decl.title.c_str(),
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              extents_.x, extents_.y, window_flags);
    if(!window) throw Bad_Window{};

    auto render_flags = int{SDL_RENDERER_ACCELERATED};
    if(game_decl.vsync)
    {
      render_flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    renderer = SDL_CreateRenderer(window, -1, render_flags);
    if(!renderer)
    {
      SDL_DestroyWindow(window);
      throw Bad_Renderer{};
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  }
  Graphics_Desc::Graphics_Desc(Graphics_Desc&& g) noexcept
                               : window(g.window),
                                 renderer(g.renderer),
                                 extents_(std::move(g.extents_))
  {
    g.window = nullptr;
    g.renderer = nullptr;
  }
  Graphics_Desc::~Graphics_Desc() noexcept
  {
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
  }
}
