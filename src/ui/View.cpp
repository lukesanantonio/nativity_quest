/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "View.h"
namespace game { namespace ui
{
  void View::render() const noexcept
  {
    auto render_border = [&](Volume<int> const& vol, Color const& col)
    {
      SDL_SetRenderDrawColor(graphics_.renderer,
                             col.red, col.green, col.blue, 0xff);
      auto sdl_vol = to_sdl_rect(vol);
      SDL_RenderDrawRect(graphics_.renderer, &sdl_vol);
    };

    if(layed_out_)
    {
      // Render borders
      if(parent_border_)
      {
        render_border(parent_volume(), parent_border_.value());
      }
      if(this_border_)
      {
        render_border(this_volume(), this_border_.value());
      }

      // Render the view.
      render_();
    }
  }

  /*!
   * \brief Creates or sets an existing border around the view.
   */
  void View::set_border(Border_Volume vol, Color color) noexcept
  {
    switch(vol)
    {
      case Border_Volume::This:
      {
        parent_border_ = color;
        break;
      }
      case Border_Volume::Parent:
      {
        parent_border_ = color;
        break;
      }
    }
  }

  /*!
   * \brief Removes a specific border by volume classification.
   *
   * \returns True if something was changed, false otherwise.
   */
  bool View::remove_border(Border_Volume vol) noexcept
  {
    auto& border = vol == Border_Volume::This ? this_border_ : parent_border_;

    if(border)
    {
      border = boost::none;
      return true;
    }
    return false;
  }
} }
