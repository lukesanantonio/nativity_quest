/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Graphics_Desc.h"
namespace game
{
  void Managed_Label::generate_(Graphics_Desc& g) const noexcept
  {
    auto update = [](Managed_Label const& data, Graphics_Desc::label_t& label)
    {
      label.data(data.str());
      label.text_height(data.text_height());

      auto label_pos = data.position();
      label.position(pong::math::vector<int>{label_pos.x, label_pos.y});

      label.text_color(data.color());
      label.mode(data.mode());

      data.update_ = false;
    };

    // If we don't have a label object yet
    if(!id_)
    {
      // Make one
      id_ = g.add_label();

      auto& new_label = g.label(id_.value());
      new_label.font_face(&g.font.face);
      new_label.rasterizer(&g.font.raster);

      // Make sure to update it
      update_ = true;
    }

    if(update_)
    {
      // Update the label object with our new data (if necessary).
      update(*this, g.label(id_.value()));
    }
  }
  void Managed_Label::render(Graphics_Desc& g) const noexcept
  {
    generate_(g);

    // Render the label itself.
    g.render_label(id_.value());
  }

  Graphics_Desc::Graphics_Desc(std::string const& title,
                               Vec<int> const& extents,
                               std::string const& font_file,
                               pong::Logger* log)
                               : extents_(extents), font{{font_file, log}, {}}
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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  }
  Graphics_Desc::~Graphics_Desc() noexcept
  {
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
  }
  int Graphics_Desc::get_width() const noexcept
  {
    return extents_.x;
  }
  int Graphics_Desc::get_height() const noexcept
  {
    return extents_.y;
  }
}
