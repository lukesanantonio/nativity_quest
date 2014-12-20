/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Inventory_View_Control.h"
namespace game
{
  void Inventory_View_Control::handle_event(SDL_Event const& event) noexcept
  {
    if(enter) return;

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
         1 <= selected && selected <= 5)
      {
        selected -= 1;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_D &&
         0 <= selected && selected <= 4)
      {
        selected += 1;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
      {
        enter = true;
      }
    }
  }
  void Inventory_View_Control::layout(Label_View<Inventory_View_Control>& view,
                                      Graphics_Desc& g) noexcept
  {
    auto vol = view.vol();

    auto start = Vec<int>{vol.width / 3 / 2, 15};
    auto cur_pos = start;

    int i = 0;
    for(auto& label : view.labels())
    {
      label.position({cur_pos.x + vol.pos.x - (label.surface_extents(g).x / 2),
                      cur_pos.y + vol.pos.y});

      cur_pos.x += vol.width / 3;

      if(i == view.labels().size() / 2 - 1)
      {
        cur_pos.y += vol.height / 2;
        cur_pos.x = start.x;
      }

      ++i;
    }
  }
  void Inventory_View_Control::render(Graphics_Desc& g,
                                Label_View<Inventory_View_Control> const& view,
                                Sprite_Container&) const noexcept
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
