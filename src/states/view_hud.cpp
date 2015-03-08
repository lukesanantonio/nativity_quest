/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "view_hud.h"
#include "../common/render.h"
#include <cstdio> // whatamievendoing
namespace game
{
  View_Hud_State::View_Hud_State(Game& g, std::string asset) noexcept
                                 : Game_State(g, true),
                                   hud(ui::load(g, asset)),
                                   anim(1, 0xff,
                                        Anim_Repeat_Mode::Reverse_Repeat)
  {
    hud->layout(game_.graphics.size());

    anim.set_segment_fn([&](int seg)
    {
      double h = ((double) seg / 0xff) * 360;
      double s = 1.0;
      double v = 1.0;

      int i = (int) floor(h / 60.0) % 6;
      double f = h / 60.0 - floor(h / 60.0);
      double p = v * (double)(1 - s);
      double q = v * (double)(1 - s * f);
      double t = v * (double)(1 - (1 - f) * s);

      switch(i)
      {
        case 0:
          background = color_from_intensity(v, t, p);
          break;
        case 1:
          background = color_from_intensity(q, v, p);
          break;
        case 2:
          background = color_from_intensity(p, v, t);
          break;
        case 3:
          background = color_from_intensity(p, q, v);
          break;
        case 4:
          background = color_from_intensity(t, p, v);
          break;
        case 5:
          background = color_from_intensity(v, p, q);
          break;
      }
    });
  }

  void View_Hud_State::handle_event(SDL_Event const& e) noexcept
  {
    hud->dispatch_event(e);
  }
  void View_Hud_State::step() noexcept
  {
    // Calculate the background color.
    anim.step();
  }
  void View_Hud_State::render() const noexcept
  {
    set_render_draw_color(game_.graphics.renderer, background);
    SDL_RenderClear(game_.graphics.renderer);
    hud->render();
  }
}
