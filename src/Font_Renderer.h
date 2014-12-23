/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <vector>
#include <tuple>

#include "SDL.h"
#include "SDL_ttf.h"

#include "Color.h"
namespace game
{
  struct Font_Renderer
  {
    Font_Renderer(std::string font) noexcept;
    ~Font_Renderer() noexcept;

    SDL_Surface* render_text(std::string text, int size,
                             Color color) const noexcept;

    inline void filename(std::string file) noexcept
    { file_ = file; font_cache_.clear(); }
    inline std::string filename() const noexcept
    { return file_; }
  private:
    std::string file_;
    mutable std::vector<std::tuple<TTF_Font*, int> > font_cache_;
  };
}
