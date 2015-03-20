/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <memory>
#include "SDL.h"
namespace game
{
  struct Surface_Deleter
  {
    void operator()(SDL_Surface* surface) noexcept
    {
      SDL_FreeSurface(surface);
    }
  };
  using Surface_Ptr = std::unique_ptr<SDL_Surface, Surface_Deleter>;

  using Shared_Surface_Ptr = std::shared_ptr<SDL_Surface>;
  inline Shared_Surface_Ptr
  make_shared_surface(Shared_Surface_Ptr::element_type* ptr = nullptr) noexcept
  {
    return Shared_Surface_Ptr(ptr, Surface_Deleter{});
  }

  /*!
   * \brief This class owns a surface and caches its texture to ease rendering.
   */
  struct Drawable_Surface
  {
    // The class isn't too strict about the up-to-dateness of the texture.
    // Call with a null renderer pointer to basically refresh the cache.
    Surface_Ptr surface;

    SDL_Texture* texture(SDL_Renderer* r) const noexcept;
  private:
    mutable SDL_Texture* texture_;
    mutable SDL_Renderer* renderer_;
  };

  struct Texture_Deleter
  {
    void operator()(SDL_Texture* texture) noexcept
    {
      SDL_DestroyTexture(texture);
    }
  };
  using Texture_Ptr = std::unique_ptr<SDL_Texture, Texture_Deleter>;

  using Shared_Texture_Ptr = std::shared_ptr<SDL_Texture>;
  inline Shared_Texture_Ptr
  make_shared_texture(Shared_Texture_Ptr::element_type* ptr = nullptr) noexcept
  {
    return Shared_Texture_Ptr(ptr, Texture_Deleter{});
  }
}
