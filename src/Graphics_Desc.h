/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include "SDL.h"

#include "pong/render/text.h"
#include "pong/render/widgets/Label.h"
#include "pong/common/ID_Map.hpp"

#include "vec.h"
namespace game
{
  using label_id = pong::id_type;

  struct Bad_Window {};
  struct Bad_Renderer {};

  struct Graphics_Desc
  {
    using label_t = pong::Label<std::string>;

    Graphics_Desc(std::string const& title, Vec<int> const& extents,
                  std::string const& font_file, pong::Logger* log);
    ~Graphics_Desc() noexcept;

    int get_width() const noexcept;
    int get_height() const noexcept;

    inline label_id add_label() noexcept;
    inline void remove_label(label_id id) noexcept;

    inline label_t& label(label_id id) noexcept;
    inline label_t const& label(label_id id) const noexcept;
    inline label_t const& clabel(label_id id) const noexcept;

    inline void render_label(label_id id) const noexcept;

    SDL_Window* window;
    SDL_Renderer* renderer;

    struct
    {
      pong::text::Face face;
      pong::text::AntiAliasedRaster raster;
    } font;
  private:
    Vec<int> extents_;

    pong::ID_Map<label_t> labels_;
  };

  inline label_id Graphics_Desc::add_label() noexcept
  {
    label_t label;

    label.font_face(&font.face);
    label.rasterizer(&font.raster);

    return labels_.insert(label);
  }
  inline void Graphics_Desc::remove_label(label_id id) noexcept
  {
    labels_.erase(id);
  }

  inline auto Graphics_Desc::label(label_id id) noexcept -> label_t&
  {
    return labels_.find(id);
  }
  inline auto
  Graphics_Desc::label(label_id id) const noexcept -> label_t const&
  {
    return labels_.find(id);
  }
  inline auto
  Graphics_Desc::clabel(label_id id) const noexcept -> label_t const&
  {
    return labels_.find(id);
  }

  inline void Graphics_Desc::render_label(label_id id) const noexcept
  {
    labels_.find(id).render(renderer);
  }
}
