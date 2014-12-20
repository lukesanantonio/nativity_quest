/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Combat_Control.h"
#include "decl/combat.h"
#include "effects.h"
#include "render.h"
namespace game
{
  Combat_Control::Combat_Control(Player& player,
                                 Enemy_Instance& enemy) noexcept
                                 : player(player), enemy(enemy)
  {
    for(auto item : player.inventory)
    {
      if(item != no::item) inventory_view.add_label(item->str);
      else inventory_view.add_label("No item");

      inventory_view.labels().back().text_height(35);
    }
  }

  void Combat_Control::handle_event(SDL_Event const& event) noexcept
  {
    if(state != Fight_State::Player_Turn ||
       anim != Combat_Anim_State::None) return;

    if(in_inventory)
    {
      if(event.type == SDL_KEYDOWN)
      {
        if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
          in_inventory = false;
          return;
        }
      }
      inventory_view.handle_event(event);
      if(inventory_view.control().enter)
      {
        auto selected = inventory_view.control().selected;

        if(can_be_combat_used(*player.item_parser, player.inventory[selected]))
        {
          apply_effect(player, player.inventory[selected]);

          player.inventory[selected] = no::item;
          inventory_view.labels()[selected].str("No item");
        }

        inventory_view.control().enter = false;
      }
      return;
    }

    // TODO Fix copied code and put it somewhere (from Discard_Item_Control).
    if(event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.scancode == SDL_SCANCODE_A && 0 < selected)
      {
        --selected;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_D && selected < 2)
      {
        ++selected;
      }
      if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
      {
        if(selected == 0)
        {
          last_damage = apply_damage(enemy.entity_data, damage() +
                                     get_additional_damage(player));
          anim = Combat_Anim_State::Enemy_Life;
          anim_step = 0;
          state = Fight_State::Enemy_Turn;
        }
        if(selected == 1)
        {
          in_inventory = true;
        }
        if(selected == 2) state = Fight_State::Running;
      }
    }
  }
  void Combat_Control::layout(Label_View<Combat_Control>& view,
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

      ++i;
    }
  }

  void Combat_Control::render(Graphics_Desc& g,
                              Label_View<Combat_Control> const& view,
                              Sprite_Container& sprites) const noexcept
  {
    if(in_inventory)
    {
      inventory_view.vol(view.vol());
      inventory_view.layout(g);
      inventory_view.render(g, sprites);
    }
    else
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

    // Render the enemy and a healthbar.
    auto enemy_sprite = sprites.get_sprite(enemy.decl->sprite);

    SDL_Rect dest;
    dest.h = g.get_height() / 2;
    dest.w = dest.h;

    dest.x = g.get_width() - dest.w;
    dest.y = 0;

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
    if(render_health(g, health_box, enemy.entity_data.cur_life,
                     enemy.entity_data.max_life, last_damage, life_step))
    {
      anim = Combat_Anim_State::None;
      anim_step = 0;
    }

    // Render the player.
    auto player_sprite = sprites.get_sprite("player_back");

    SDL_Rect player_dest;
    player_dest.h = g.get_height() / 2;
    player_dest.w = player_dest.h;

    player_dest.x = 0;
    player_dest.y = g.get_height() - player_dest.h;

    SDL_RenderCopy(g.renderer, player_sprite->texture(g.renderer),
                   NULL, &player_dest);

    // Render the player health.
    health_box.x = view.vol().pos.x + view.vol().width - health_box.w;
    health_box.y = view.vol().pos.y - health_box.h - 1;

    SDL_SetRenderDrawColor(g.renderer, 0xff, 0xff, 0xff, 0xff);
    render_with_border(g, health_box);

    // Render the health bar.
    life_step = -1;
    if(anim == Combat_Anim_State::Player_Life)
    {
      life_step = anim_step;
    }
    if(render_health(g, health_box, player.entity_data.cur_life,
                     player.entity_data.max_life, last_damage, life_step))
    {
      anim = Combat_Anim_State::None;
      anim_step = 0;
    }
  }

  void Combat_Control::step() noexcept
  {
    if(state == Fight_State::Enemy_Won || state == Fight_State::Player_Won)
    {
      return;
    }

    if(anim == Combat_Anim_State::Player_Life ||
       anim == Combat_Anim_State::Enemy_Life)
    {
      anim_step += 3;
    }

    // If we are currently in an animation we can't do anything else, really.
    if(anim != Combat_Anim_State::None) return;

    if(enemy.entity_data.cur_life == 0)
    {
      state = Fight_State::Player_Won;
      return;
    }
    if(player.entity_data.cur_life == 0)
    {
      state = Fight_State::Enemy_Won;
      return;
    }

    if(state == Fight_State::Enemy_Turn)
    {
      last_damage = apply_damage(player.entity_data, damage(),
                       get_additional_defense(player) + player.combat_defense);
      state = Fight_State::Player_Turn;
      anim = Combat_Anim_State::Player_Life;
      anim_step = 0;
    }
  }
}
