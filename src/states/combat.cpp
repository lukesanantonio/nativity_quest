/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "combat.h"
#include "../decl/combat.h"
#include "inventory.h"

#define COMBAT_JSON "ui/combat"
#define COMBAT_PART "ui_part/combat_grid"
#define INVENTORY_PART "ui_part/inventory_grid"
namespace game
{
  // Shamelessly copied from inventory.cpp. Find a way to share the
  // hover-border code. This should be done in json if possible.
  constexpr int ids_size = 7;
  constexpr char const* const ids[] =
  {
    "item0",
    "item1",
    "item2",
    "item3",
    "item4",
    "item5",
    "back"
  };

  Combat_State::Combat_State(Game& g, Navigate_State& ns,
                             Enemy_Instance& e) noexcept
                             : Navigate_Sub_State(g, ns),
                               enemy(e),
                               hud(ui::load(g, COMBAT_JSON)),
                               combat_grid(ui::load(g, COMBAT_PART)),
                               inventory_grid(ui::load(g, INVENTORY_PART))
  {
    auto enemy_sprite = hud->find_child_r<ui::Sprite>("enemy_sprite");
    enemy_sprite->src(get_asset<assets::Image_Asset>(game_, e.decl->sprite));

    auto player_sprite = hud->find_child_r<ui::Sprite>("player_sprite");

    auto& player = navigate.active_player();
    auto spr_info = navigate.players.get_sprite(player.sprite_frame);

    player_sprite->src(get_asset<assets::Image_Asset>(game_, spr_info));

    auto sprite_info = navigate.players.get_orientation_info(2);
    player_sprite->set_src_rect(sprite_info.src);

    auto use_item_func = [this](int selected, auto const&)
    {
      auto sel_item = this->active_player().inventory[selected];

      auto& effects = navigate.effects;
      if(effects.used_in_combat(sel_item))
      {
        // The player-health mediator will do the animation for us.
        effects.apply_effect(this->active_player(), sel_item);
        this->active_player().inventory[selected] = assets::no::item;

        if(this->active_player().flare)
        {
          enemy.not_fighting = 12;
          // Replace ourselves with the flare, which will exit the fight.
          replace_state(game_, std::make_shared<Flare_State>(game_));
          return;
        }

        this->switch_to_combat();
      }
      else if(sel_item == effects.items->get_item("throwing_axe"))
      {
        apply_damage(enemy.entity_data, enemy.entity_data.defense + 2);
        this->active_player().inventory[selected] = assets::no::item;
      }
    };

    auto set_sel = [this](int sel, auto const&)
    {
      selected = sel;
    };

    using namespace std::placeholders;

    const std::string item_prefix = "item";
    for(int i = 0; i < 6; ++i)
    {
      inventory_grid->find_child_r(item_prefix + std::to_string(i))
        ->add_event_trigger<ui::Mouse_Click>(std::bind(use_item_func, i, _1),
                                             nullptr, true);

      inventory_grid->find_child_r(item_prefix + std::to_string(i))
        ->add_event_trigger<ui::Mouse_Hover>(std::bind(set_sel, i, _1),
                                             nullptr, true);
    }

    auto back_child = inventory_grid->find_child_r("back");
    back_child->add_event_trigger<ui::Mouse_Click>(
    [this](auto const&)
    {
      this->switch_to_combat();
    }, nullptr, true);

    back_child->add_event_trigger<ui::Mouse_Hover>(std::bind(set_sel, 6, _1),
                                                   nullptr, true);

    combat_grid->find_child_r("attack")->add_event_trigger<ui::Mouse_Click>(
    [this](auto)
    {
      if(fight_state != Fight_State::Player_Turn) return;

      // Attack the enemy.
      auto add_attack =
                     navigate.effects.additional_damage(this->active_player());

      // Do the damage
      last_damage = apply_damage(enemy.entity_data,
                                 decl::damage() + add_attack);

      fight_state = Fight_State::Enemy_Turn;
    }, nullptr, true);


    combat_grid->find_child_r("use_item")->add_event_trigger<ui::Mouse_Click>(
    [this](auto)
    {
      // Switch to an inventory view.
      this->switch_to_inventory();
    }, nullptr, true);
    combat_grid->find_child_r("run")->add_event_trigger<ui::Mouse_Click>(
    [this](auto)
    {
      // Run a random number generator or something
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
    }, nullptr, true);

    combat_grid->find_child_r("attack")->add_event_trigger<ui::Mouse_Hover>(
      std::bind(set_sel, 0, _1), nullptr, true);
    combat_grid->find_child_r("use_item")->add_event_trigger<ui::Mouse_Hover>(
      std::bind(set_sel, 1, _1), nullptr, true);
    combat_grid->find_child_r("run")->add_event_trigger<ui::Mouse_Hover>(
      std::bind(set_sel, 2, _1), nullptr, true);

    switch_to_combat();

    player_health_mediator.entity_data(&active_player().entity_data);
    player_health_mediator.bar(hud->find_child_r<ui::Bar>("player_bar").get());

    enemy_health_mediator.entity_data(&enemy.entity_data);
    enemy_health_mediator.bar(hud->find_child_r<ui::Bar>("enemy_bar").get());

    fight_state = Fight_State::Player_Turn;
  }

  void Combat_State::handle_event(SDL_Event const& e) noexcept
  {
    // Don't handle any events for the user until both health bars have
    // stopped. This should prevent spamming the attack button on the user's
    // end when they are going against a tough enemy that they rarely do
    // damage do, for instance.
    // TODO figure out a way to let mouse motion events through.
    // Well, then again... I could just do this.
    if(!bars_animating() || e.type == SDL_MOUSEMOTION)
    {
      hud->dispatch_event(e);
    }
  }
  void Combat_State::step() noexcept
  {
    // Set the border around the selected item, removing borders around
    // unselected label views.
    if(hud->find_child_r("inventory_grid"))
    {
      for(int i = 0; i < ids_size; ++i)
      {
        auto child = hud->find_child_r(ids[i]);
        if(selected == i)
        {
          child->set_border(ui::View_Volume::Parent, {0x00, 0x00, 0x00});
        }
        else
        {
          child->remove_border(ui::View_Volume::Parent);
        }
      }
    }
    else if(hud->find_child_r("combat_grid"))
    {
      static size_t combat_ids_size = 3;
      static constexpr char const* const combat_ids[] =
      {
        "attack",
        "use_item",
        "run"
      };
      for(int i = 0; i < combat_ids_size; ++i)
      {
        auto child = hud->find_child_r(combat_ids[i]);
        if(selected == i)
        {
          child->set_border(ui::View_Volume::Parent, {0x00, 0x00, 0x00});
        }
        else
        {
          child->remove_border(ui::View_Volume::Parent);
        }
      }
    }

    // TODO add hover to the combat_grid.

    player_health_mediator.step();
    enemy_health_mediator.step();

    // Basically don't let the enemy move or the combat to be won until both
    // healthbars correctly reflect their respective entity's health.
    if(bars_animating()) { return; }

    // Check if anyone won yet.
    // TODO make sure the animation finished.
    if(enemy.entity_data.cur_life == 0 &&
       (fight_state == Fight_State::Enemy_Turn ||
       fight_state == Fight_State::Player_Turn))
    {
      fight_state = Fight_State::Player_Won;
    }
    if(active_player().entity_data.cur_life == 0 &&
       (fight_state == Fight_State::Player_Turn ||
       fight_state == Fight_State::Enemy_Turn))
    {
      fight_state = Fight_State::Enemy_Won;
    }

    switch(fight_state)
    {
      case Fight_State::Enemy_Won:
      case Fight_State::Player_Won:
      {
        // The navigation state handles respawning and enemy cleanup!
        pop_state(game_);
        break;
      }
      case Fight_State::Enemy_Turn:
      {
        // Find any additional defense of the player.
        auto p_def = navigate.effects.additional_defense(active_player());
        p_def += active_player().combat_defense;

        last_damage = apply_damage(active_player().entity_data,
                                   decl::damage(), p_def);

        fight_state = Fight_State::Player_Turn;
        break;
      }
    }
  }
  void Combat_State::render() const noexcept
  {
    hud->render();
  }

  void Combat_State::on_enter() noexcept {}
  void Combat_State::on_exit() noexcept
  {
    active_player().combat_defense = 0;
  }

  void Combat_State::switch_to_inventory() noexcept
  {
    constexpr char const* const empty = "menu_empty";
    constexpr char const* const grid = "combat_grid";

    if(!hud->replace_child_r(empty, inventory_grid))
    {
      hud->replace_child_r(grid, inventory_grid);
    }

    update_labels(active_player().inventory, hud);

    selected = 6;

    hud->layout(game_.graphics.size());
  }
  void Combat_State::switch_to_combat() noexcept
  {
    constexpr char const* const empty = "menu_empty";
    constexpr char const* const grid = "inventory_grid";

    if(!hud->replace_child_r(empty, combat_grid))
    {
      hud->replace_child_r(grid, combat_grid);
    }

    selected = 1;

    hud->layout(game_.graphics.size());
  }
}
