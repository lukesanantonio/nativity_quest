/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Font_Renderer.h"

#include <iostream>
#include <cstdlib>
namespace game
{
  Font_Renderer::Font_Renderer(std::string font) noexcept : file_(font)
  {
    if(!TTF_WasInit())
    {
      if(TTF_Init() == -1)
      {
        std::cerr << "Failed to initialize SDL2_ttf: " << TTF_GetError()
                  << std::endl;
        std::abort();
      }
    }
  }
  Font_Renderer::~Font_Renderer() noexcept
  {
    if(TTF_WasInit()) TTF_Quit();
  }
  SDL_Surface* Font_Renderer::render_text(std::string text, int size,
                                          Color color) const noexcept
  {
    using std::begin; using std::end;
    auto font_find = std::find_if(begin(font_cache_), end(font_cache_),
    [size](auto const& font_tup)
    {
      return std::get<1>(font_tup) == size;
    });

    TTF_Font* ttf_font;
    if(font_find == end(font_cache_))
    {
      font_cache_.push_back({TTF_OpenFont(file_.data(), size), size});
      ttf_font = std::get<0>(font_cache_.back());
      if(!ttf_font)
      {
        std::cerr << "Failed to open font: " << TTF_GetError()
                  << std::endl;
        std::abort();
      }
    }
    else
    {
      ttf_font = std::get<0>(*font_find);
    }

    auto fg_col = to_sdl_color(color);
    if(ttf_font) return TTF_RenderText_Blended(ttf_font, text.data(), fg_col);
    else return nullptr;
  }
}

