/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "combat.h"
#include "../decl/combat.h"
namespace game
{
  void set_bar_to_life(ui::Bar& bar, Entity_Data& et) noexcept
  {
    bar.max = et.max_life;
    bar.cur = et.cur_life;
  }

  void set_bar_animating(ui::Bar& bar, Entity_Data& et, int delta_life,
                         int step, int max_step) noexcept
  {
    auto res = 300;

    auto bar_width = double(res / et.max_life);

    bar.max = res;
    bar.cur = int((bar_width * et.cur_life) +
                  ((bar_width * delta_life) *
                  ((max_step - step) / (double) max_step)));
  }

  Combat_State::Combat_State(Game& g, Navigate_State& ns,
                             Enemy_Instance& e) noexcept
                             : Navigate_Sub_State(g, ns), enemy(e),
                               hud(parse_json("assets/combat_hud.json"))
  {
    auto& enemy_sprite = boost::get<ui::Sprite>(hud.elements[0].element);
    enemy_sprite.src = e.decl->sprite;

    auto& player_sprite = boost::get<ui::Sprite>(hud.elements[1].element);
    player_sprite.src = navigate.players.get_sprite();
    player_sprite.frame = active_player().sprite_frame;
    player_sprite.vol = Volume<int>{{5, 144}, 25, 32};

    auto label_view = ui::Label_View{};
    label_view.col = "black";
    hud.elements.push_back({label_view, {}, ui::Side::Bottom});

    switch_to_combat_menu();

    auto& enemy_bar = boost::get<ui::Bar>(hud.elements[2].element);
    set_bar_to_life(enemy_bar, enemy.entity_data);

    auto& player_bar = boost::get<ui::Bar>(hud.elements[3].element);
    set_bar_to_life(player_bar, active_player().entity_data);

    fight_state = Fight_State::Player_Turn;
  }

  void Combat_State::handle_event(SDL_Event const&) noexcept
  {
  }
  void Combat_State::step() noexcept
  {
    // Check if anyone won yet.
    if(enemy.entity_data.cur_life == 0 &&
       fight_state == Fight_State::Enemy_Turn)
    {
      fight_state = Fight_State::Player_Won;
    }
    if(active_player().entity_data.cur_life == 0 &&
       fight_state == Fight_State::Player_Turn)
    {
      fight_state = Fight_State::Enemy_Won;
    }

    constexpr auto animation_speed = 75;

    switch(fight_state)
    {
      case Fight_State::Enemy_Won:
      case Fight_State::Player_Won:
      {
        // The navigation state handles respawning and enemy cleanup!
        pop_state(game_);
        break;
      }
      case Fight_State::Player_Turn:
      {
        auto& label_view =
                       boost::get<ui::Label_View>(hud.elements.back().element);

        // If the player wants to attack.
        if(label_view.done && label_view.selected == 0 &&
           labels_state == Labels_State::Combat)
        {
          // Find our additional attack power
          auto add_attack =navigate.effects.additional_damage(active_player());

          // Do the damage
          last_damage = apply_damage(enemy.entity_data,
                                     decl::damage() + add_attack);
          // Animate the enemy bar
          fight_state = Fight_State::Enemy_Animating;
          cur_step = 0;
          max_step = animation_speed * last_damage /
                     enemy.entity_data.max_life;
        }
        else if(label_view.done && label_view.selected == 2 &&
                labels_state == Labels_State::Combat)
        {
          std::mt19937 prng{random_device()};

          std::uniform_int_distribution<int> dist(0, 2);
          if(dist(prng) == 0)
          {
            enemy.not_fighting = 6;
            pop_state(game_);
          }
          else
          {
            // We don't need to animate anything, just go to enemy's turn.
            fight_state = Fight_State::Enemy_Turn;
          }
        }
        else if(label_view.done && label_view.selected == 1 &&
                labels_state == Labels_State::Combat)
        {
          // Go to inventory view.
          label_view.done = false;
          switch_to_inventory_view();
        }
        else if(label_view.done && labels_state == Labels_State::Inventory)
        {
          if(label_view.selected > 5)
          {
            switch_to_combat_menu();
          }
          else
          {
            auto sel_item = active_player().inventory[label_view.selected];

            auto& effects = navigate.effects;
            if(effects.used_in_combat(sel_item))
            {
              effects.apply_effect(active_player(), sel_item);

              active_player().inventory[label_view.selected] = decl::no::item;

              if(active_player().flare)
              {
                enemy.not_fighting = 12;
                // Replace ourselves with the flare, which will exit the fight.
                replace_state(game_, std::make_shared<Flare_State>(game_));

                return;
              }

              switch_to_combat_menu();
            }
          }
          label_view.done = false;
        }
        break;
      }
      case Fight_State::Enemy_Turn:
      {
        // Find any additional defense of the player.
        auto p_def = navigate.effects.additional_defense(active_player());
        p_def += active_player().combat_defense;

        last_damage = apply_damage(active_player().entity_data,
                                   decl::damage(), p_def);

        fight_state = Fight_State::Player_Animating;

        auto& label_view =
                       boost::get<ui::Label_View>(hud.elements.back().element);
        label_view.done = false;

        cur_step = 0;
        max_step = animation_speed * last_damage /
                   active_player().entity_data.max_life;
        break;
      }
      case Fight_State::Enemy_Animating:
      {
        auto& enemy_bar = boost::get<ui::Bar>(hud.elements[2].element);

        if(max_step == cur_step)
        {
          fight_state = Fight_State::Enemy_Turn;
          set_bar_to_life(enemy_bar, enemy.entity_data);
        }
        else
        {

          set_bar_animating(enemy_bar, enemy.entity_data, last_damage,
                            cur_step, max_step);
          ++cur_step;
        }


        break;
      }
      case Fight_State::Player_Animating:
      {
        auto& player_bar = boost::get<ui::Bar>(hud.elements[3].element);

        if(max_step == cur_step)
        {
          fight_state = Fight_State::Player_Turn;
          set_bar_to_life(player_bar, active_player().entity_data);
        }
        else
        {
          set_bar_animating(player_bar, active_player().entity_data,
                            last_damage, cur_step, max_step);
          ++cur_step;
        }

        break;
      }
      case Fight_State::Running:
      {
        break;
      }
    }

    game_.view.reset();
    game_.presenter.present(hud, game_.view, game_.graphics.size());
  }
  void Combat_State::render() const noexcept
  {
    game_.view.render(game_.graphics);
  }

  void Combat_State::on_enter() noexcept
  {
    game_.presenter.handle_events(true);
  }
  void Combat_State::on_exit() noexcept
  {
    game_.presenter.handle_events(true);

    active_player().combat_defense = 0;
  }

  void Combat_State::switch_to_inventory_view() noexcept
  {
    auto& label_view = boost::get<ui::Label_View>(hud.elements.back().element);

    label_view.labels.clear();

    for(auto item : active_player().inventory)
    {
      if(item) label_view.labels.push_back(item->str);
      else label_view.labels.push_back("No item");
    }

    label_view.labels.push_back("Back");

    labels_state = Labels_State::Inventory;
  }
  void Combat_State::switch_to_combat_menu() noexcept
  {
    auto& label_view = boost::get<ui::Label_View>(hud.elements.back().element);

    label_view.labels.clear();

    label_view.labels.push_back("Attack");
    label_view.labels.push_back("Use item");
    label_view.labels.push_back("Run");

    labels_state = Labels_State::Combat;
  }
}
