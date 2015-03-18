/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "combat_intro.h"
#include "combat.h"
namespace game
{
  namespace
  {
    ui::Label make_l(Game& g, Enemy_Instance& e) noexcept
    {
      auto l = ui::Label{g.graphics, g.font};
      l.str_name("enemy_encountered");
      l.str_args(game::t(e.decl->str));
      return l;
    }
  }
  Combat_Intro::Combat_Intro(Game& g, Navigate_State& ns,
                             Enemy_Instance& e) noexcept
                             : Intro_State(g, ns, make_l(g, e)), enemy_(e) { }

  void Combat_Intro::done_() noexcept
  {
    push_state();
  }
  void Combat_Intro::push_state() noexcept
  {
    replace_state(game_,
                  std::make_shared<Combat_State>(game_, navigate, enemy_));
  }
}
