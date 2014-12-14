/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Combat_Control.h"
namespace game
{
  void Combat_Control::handle_event(SDL_Event const& event) noexcept
  {
    // TODO Fix copied code and put it somewhere (from Discard_Item_Control).
    if(event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.scancode == SDL_SCANCODE_A && 0 < selected)
      {
        --selected;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_D && selected < 1)
      {
        ++selected;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
      {
        if(selected == 1) run = true;
      }
    }
  }
  void Combat_Control::layout(Label_View<Combat_Control>& view,
                              Graphics_Desc& g) noexcept
  {
    auto vol = view.vol();

    auto start = Vec<int>{vol.width / 2 / 2, 15};
    auto cur_pos = start;

    int i = 0;
    for(auto& label : view.labels())
    {
      label.position({cur_pos.x + vol.pos.x - (label.surface_extents(g).x / 2),
                      cur_pos.y + vol.pos.y});

      cur_pos.x += vol.width / 2;

      ++i;
    }
  }
  void Combat_Control::render(Graphics_Desc& g,
                              Label_View<Combat_Control> const& view,
                              Sprite_Container& sprites) const noexcept
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
