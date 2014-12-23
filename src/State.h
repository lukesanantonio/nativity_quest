/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <vector>
#include <memory>
#include <array>

#include "Graphics_Desc.h"
#include "Player.h"

#include <boost/variant.hpp>
namespace game
{
  struct Game_State;

  struct State
  {
    bool running = false;

    Vec<int> window_extents;

    using stack_t = std::vector<std::shared_ptr<Game_State> >;
    stack_t state_stack;
  };

  struct Game_State
  {
    virtual ~Game_State() noexcept {}

    virtual void handle_event(State&, SDL_Event const&) noexcept = 0;
    virtual void step(State&) noexcept = 0;
    virtual void render(State const&,
                        Graphics_Desc&,
                        Sprite_Container&) const noexcept = 0;

    virtual bool is_toplevel() const noexcept = 0;
  };

  enum class View
  {
    Turn, Menu
  };

  inline void push_state(State& s, std::shared_ptr<Game_State> gs) noexcept
  {
    s.state_stack.push_back(gs);
  }
  inline void replace_state(State& s, std::shared_ptr<Game_State> gs) noexcept
  {
    s.state_stack.pop_back();
    s.state_stack.push_back(gs);
  }
  inline void pop_state(State& s) noexcept
  {
    s.state_stack.pop_back();
  }

  void render(State const& state, Graphics_Desc& g,
              Sprite_Container& s) noexcept;
  void handle_event(State& s, SDL_Event const& event) noexcept;
  void step(State& s) noexcept;
}
