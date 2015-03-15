/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "combat_intro.h"
#include "combat.h"
namespace game
{
  Combat_Intro::Combat_Intro(Game& g, Navigate_State& ns,
                             Enemy_Instance& e) noexcept
                             : Navigate_Sub_State(g, ns),
                               label_(g.graphics, g.font),
                               enemy_(e),
                               max_step(80)
  {
    label_.str_name("enemy_encountered");
    label_.str_args(game::t("enemy_" + e.decl->id));
    label_.layout(g.graphics.size());
  }

  void Combat_Intro::handle_event(SDL_Event const& e) noexcept
  {
    if((e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN) &&
       max_step / 2 < cur_step)
    {
      this->push_state();
    }
  }
  void Combat_Intro::step() noexcept
  {
    if(cur_step++ == max_step) this->push_state();
  }
  void Combat_Intro::push_state() noexcept
  {
    replace_state(game_,
                  std::make_shared<Combat_State>(game_, navigate, enemy_));
  }
  void Combat_Intro::render() const noexcept
  {
    label_.render();
  }
}
