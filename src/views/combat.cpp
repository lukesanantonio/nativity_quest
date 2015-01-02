/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "combat.h"
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
  }

  void Combat_State::handle_event(SDL_Event const&) noexcept
  {
  }
  void Combat_State::step() noexcept
  {
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
