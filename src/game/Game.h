/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <vector>
#include <memory>
#include <array>

#include "Graphics_Desc.h"
#include "Font_Renderer.h"
namespace game
{
  struct Game_State;

  struct Game
  {
    mutable Graphics_Desc graphics;
    Font_Renderer font;

    bool running = false;

    using stack_t = std::vector<std::shared_ptr<Game_State> >;
    stack_t states;
  };

  struct Game_State
  {
    Game_State(Game& game) noexcept : game_(game) {}
    virtual ~Game_State() noexcept {}

    virtual void handle_event(SDL_Event const&) noexcept = 0;
    virtual void step() noexcept = 0;
    virtual void render() const noexcept = 0;

    virtual bool is_toplevel() const noexcept = 0;

  protected:
    Game& game_;
  };

  enum class View
  {
    Turn, Menu
  };

  inline void push_state(Game& g, std::shared_ptr<Game_State> gs) noexcept
  {
    g.states.push_back(gs);
  }
  inline void replace_state(Game& g, std::shared_ptr<Game_State> gs) noexcept
  {
    g.states.pop_back();
    g.states.push_back(gs);
  }
  inline void pop_state(Game& game) noexcept
  {
    game.states.pop_back();
  }

  void render_all(Game const& game) noexcept;
}
