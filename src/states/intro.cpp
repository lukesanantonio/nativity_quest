/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "intro.h"
namespace game
{
  Intro_State::Intro_State(Game& game, Navigate_State& ns,
                           ui::Label&& l, int ms) noexcept
                           : Navigate_Sub_State(game, ns),
                             label(std::move(l)),
                             cur_step(0), max_step(ms)
  {
    label.layout(game.graphics.size());
  }
  void Intro_State::handle_event(SDL_Event const& event) noexcept
  {
    if((event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYDOWN) &&
       max_step / 2 < cur_step)
    {
      done_();
    }
  }
  void Intro_State::step() noexcept
  {
    if(cur_step++ == max_step) done_();
  }
  void Intro_State::render() const noexcept
  {
    label.render();
  }
  void Intro_State::done_() noexcept
  {
    pop_state(game_);
  }
}
