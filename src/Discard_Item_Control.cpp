/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Discard_Item_Control.h"
namespace game
{
  void
  Discard_Item_Control::handle_event(SDL_Event const& event) noexcept
  {
    if(event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.scancode == SDL_SCANCODE_W &&
         3 <= selected && selected <= 5)
      {
        selected -= 3;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_S &&
         0 <= selected && selected <= 2)
      {
        selected += 3;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_A &&
         1 <= selected && selected <= 6)
      {
        selected -= 1;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_D &&
         0 <= selected && selected <= 5)
      {
        selected += 1;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
      {
        done = true;
      }
    }
  }
  void
  Discard_Item_Control::layout(Label_View<Discard_Item_Control>& view) noexcept
  {
    auto vol = view.vol();

    auto cur_pos = Vec<int>{0,0};

    int i = 0;
    for(auto& label : view.labels())
    {
      label.position({cur_pos.x + vol.pos.x, cur_pos.y + vol.pos.y});

      cur_pos.x += vol.width / 4;

      if(i == 2)
      {
        cur_pos.y += vol.height / 2;
        cur_pos.x = 0;
      }

      ++i;
    }
  }
  void Discard_Item_Control::render(Graphics_Desc& g,
                   Label_View<Discard_Item_Control> const& view) const noexcept
  {
    auto selected_label_pos = view.labels()[selected].position();

    SDL_Rect marker;
    marker.w = 50;
    marker.h = view.labels()[selected].surface_extents(g).y;

    marker.x = selected_label_pos.x - 10 - marker.w;
    marker.y = selected_label_pos.y;

    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderFillRect(g.renderer, &marker);
  }
}

