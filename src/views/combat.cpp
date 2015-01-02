/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "combat.h"
#include "../decl/combat.h"
namespace game
{
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

    label_view.labels.push_back("Attack");
    label_view.labels.push_back("Use item");
    label_view.labels.push_back("Run");

    label_view.col = "black";

    auto sprite_width = player_sprite.vol->width * player_sprite.scale;

    hud.elements.push_back({label_view, {}, ui::Side::Bottom});

    auto& enemy_bar = boost::get<ui::Bar>(hud.elements[2].element);
    auto& player_bar = boost::get<ui::Bar>(hud.elements[3].element);

    enemy_bar.max = enemy.entity_data.max_life;
    player_bar.max = active_player().entity_data.max_life;

    fight_state = Fight_State::Player_Turn;
  }

  void Combat_State::handle_event(SDL_Event const&) noexcept
  {
  }
  void Combat_State::step() noexcept
  {
    if(active_player().entity_data.cur_life == 0 ||
       enemy.entity_data.cur_life == 0)
    {
      pop_state(game_);
    }

    auto& label_view = boost::get<ui::Label_View>(hud.elements.back().element);
    if(label_view.done && label_view.selected == 0 &&
       fight_state == Fight_State::Player_Turn)
    {
      apply_damage(enemy.entity_data, decl::damage());
      fight_state = Fight_State::Enemy_Turn;
    }
    else if(label_view.done && label_view.selected == 2 &&
            fight_state == Fight_State::Player_Turn)
    {
      std::mt19937 prng{random_device()};

      std::uniform_int_distribution<int> dist(0, 2);
      if(dist(prng) == 0)
      {
        pop_state(game_);
      }
      else
      {
        fight_state = Fight_State::Enemy_Turn;
      }
    }
    else if(fight_state == Fight_State::Enemy_Turn)
    {
      apply_damage(active_player().entity_data, decl::damage());
      fight_state = Fight_State::Player_Turn;
      label_view.done = false;
    }

    auto& enemy_bar = boost::get<ui::Bar>(hud.elements[2].element);
    enemy_bar.cur = enemy.entity_data.cur_life;

    auto& player_bar = boost::get<ui::Bar>(hud.elements[3].element);
    player_bar.cur = active_player().entity_data.cur_life;

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
  }
}
