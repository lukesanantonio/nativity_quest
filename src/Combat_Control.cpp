/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Combat_Control.h"
namespace game
{
  void Combat_Control::handle_event(SDL_Event const& event) noexcept
  {
    if(state != Fight_State::Player_Turn ||
       anim != Combat_Anim_State::None) return;

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
        if(selected == 0)
        {
          --enemy.current_life;
          anim = Combat_Anim_State::Enemy_Life;
          anim_step = 0;
          state = Fight_State::Enemy_Turn;
        }
        if(selected == 1) state = Fight_State::Running;
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

  void render_with_border(Graphics_Desc& g, SDL_Rect rect)
  {
    SDL_RenderFillRect(g.renderer, &rect);

    --rect.x;
    --rect.y;
    rect.w += 2;
    rect.h += 2;

    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderDrawRect(g.renderer, &rect);
  }

  bool render_health(Graphics_Desc& g, SDL_Rect box,
                     int life, int max, int life_step) noexcept
  {
    SDL_Rect bar;
    bar.w = box.w  - 50;
    bar.h = 15;
    bar.x = box.x + box.w / 2 - bar.w / 2;
    bar.y = box.y + box.h / 2 - bar.h / 2;

    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x77, 0x00, 0xff);
    render_with_border(g, bar);

    SDL_SetRenderDrawColor(g.renderer, 0x00, 0xff, 0x00, 0xff);

    bool ret = false;

    if(life_step == -1)
    {
      bar.w = bar.w / max * life;
    }
    else
    {
      auto segment_width = bar.w / max;

      bar.w = segment_width * (life + 1);
      if(segment_width <= life_step)
      {
        bar.w -= segment_width;
        ret = true;
      }
      else bar.w -= life_step;
    }

    SDL_RenderFillRect(g.renderer, &bar);
    return ret;
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

    // Render the enemy and a healthbar.
    auto enemy_sprite = sprites.get_sprite(enemy.decl->sprite);

    SDL_Rect dest;
    dest.y = 0;
    dest.x = g.get_width() / 2;
    dest.w = g.get_width() / 2;
    dest.h = g.get_height() / 2 - 75;

    SDL_RenderCopy(g.renderer, enemy_sprite->texture(g.renderer), NULL, &dest);

    SDL_Rect health_box;

    health_box.x = dest.x;
    health_box.w = dest.w;
    health_box.h = 50;
    health_box.y = dest.y + dest.h - health_box.h;

    SDL_SetRenderDrawColor(g.renderer, 0xff, 0xff, 0xff, 0xff);
    render_with_border(g, health_box);

    int life_step = -1;
    if(anim == Combat_Anim_State::Enemy_Life)
    {
      life_step = anim_step;
    }
    if(render_health(g, health_box, enemy.current_life, enemy.decl->life,
                     life_step))
    {
      anim = Combat_Anim_State::None;
      anim_step = 0;
    }

    // Render the player.
    auto player_sprite = sprites.get_sprite("player_back");

    SDL_Rect player_dest;
    player_dest.w = g.get_width() / 2;
    player_dest.x = 0;

    player_dest.h = g.get_height() / 2;
    player_dest.y = g.get_height() - player_dest.h;

    SDL_RenderCopy(g.renderer, player_sprite->texture(g.renderer),
                   NULL, &player_dest);

    // Render the player health.
    health_box.x = player_dest.x + player_dest.w;
    health_box.y = view.vol().pos.y - health_box.h - 5;

    SDL_SetRenderDrawColor(g.renderer, 0xff, 0xff, 0xff, 0xff);
    render_with_border(g, health_box);

    // Render the health bar.
    life_step = -1;
    if(anim == Combat_Anim_State::Player_Life)
    {
      life_step = anim_step;
    }
    if(render_health(g, health_box, player.life, player.max_life, life_step))
    {
      anim = Combat_Anim_State::None;
      anim_step = 0;
    }
  }

  void Combat_Control::step() noexcept
  {
    if(anim == Combat_Anim_State::Player_Life ||
       anim == Combat_Anim_State::Enemy_Life)
    {
      anim_step += 3;
    }

    if(state == Fight_State::Enemy_Turn && anim == Combat_Anim_State::None)
    {
      --player.life;
      state = Fight_State::Player_Turn;
      anim = Combat_Anim_State::Player_Life;
      anim_step = 0;
    }
  }
}
