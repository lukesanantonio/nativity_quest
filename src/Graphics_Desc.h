/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

#include "pong/render/text.h"
#include "pong/render/widgets/Label.h"
#include "pong/common/ID_Map.hpp"

#include "boost/optional.hpp"

#include "vec.h"
namespace game
{
  // Utility

  inline bool operator==(SDL_Color const& c1, SDL_Color const& c2) noexcept
  {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
  }
  inline bool operator!=(SDL_Color const& c1, SDL_Color const& c2) noexcept
  {
    return !(c1 == c2);
  }

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

    inline Vec<int> size() const noexcept
    { return {get_width(), get_height()}; }

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

    TTF_Font* large_font;
    TTF_Font* small_font;
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

  struct Managed_Label
  {
    boost::optional<label_id> id;

    inline std::string str() const noexcept;
    inline void str(std::string const&) noexcept;

    inline int text_height() const noexcept;
    inline void text_height(int size) noexcept;

    inline Vec<int> position() const noexcept;
    inline void position(Vec<int> const&) noexcept;

    inline SDL_Color color() const noexcept;
    inline void color(SDL_Color color) noexcept;

    inline pong::Label_Mode mode() const noexcept;
    inline void mode(pong::Label_Mode mode) noexcept;

    void render(Graphics_Desc& g) const noexcept;

    inline Vec<int> surface_extents(Graphics_Desc& g) const noexcept;
  private:
    void generate_(Graphics_Desc& g) const noexcept;

    mutable boost::optional<label_id> id_;

    mutable bool update_;

    std::string str_;
    int size_;
    Vec<int> pos_;
    SDL_Color color_;
    pong::Label_Mode mode_;
  };

  inline std::string Managed_Label::str() const noexcept
  {
    return str_;
  }
  inline void Managed_Label::str(std::string const& s) noexcept
  {
    if(s != str_)
    {
      str_ = s;
      update_ = true;
    }
  }

  inline int Managed_Label::text_height() const noexcept
  {
    return size_;
  }
  inline void Managed_Label::text_height(int size) noexcept
  {
    if(size != size_)
    {
      size_ = size;
      update_ = true;
    }
  }

  inline Vec<int> Managed_Label::position() const noexcept
  {
    return pos_;
  }
  inline void Managed_Label::position(Vec<int> const& pos) noexcept
  {
    if(pos != pos_)
    {
      pos_ = pos;
      update_ = true;
    }
  }

  inline SDL_Color Managed_Label::color() const noexcept
  {
    return color_;
  }
  inline void Managed_Label::color(SDL_Color color) noexcept
  {
    if(color != color_)
    {
      color_ = color;
      update_ = true;
    }
  }

  inline pong::Label_Mode Managed_Label::mode() const noexcept
  {
    return mode_;
  }
  inline void Managed_Label::mode(pong::Label_Mode mode) noexcept
  {
    if(mode != mode_)
    {
      mode_ = mode;
      update_ = true;
    }
  }

  inline Vec<int>
  Managed_Label::surface_extents(Graphics_Desc& g) const noexcept
  {
    generate_(g);
    auto const& label = g.label(id_.value());
    return {label.surface_width(), label.surface_height()};
  }
}
