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

#include "../assets/asset.h"
#include "../assets/find.h"
namespace game
{
  struct Game_State;

  struct Game
  {
    Game(Graphics_Desc&& g, Font_Renderer&& f) noexcept
         : graphics(std::move(g)), font(std::move(f)), running(true) {}

    mutable Graphics_Desc graphics;
    Font_Renderer font;

    bool running = true;

    using stack_t = std::vector<std::shared_ptr<Game_State> >;
    stack_t states;

    assets::Vector assets;
  };

  struct Game_State
  {
    Game_State(Game& game, bool toplevel) noexcept
               : game_(game), is_toplevel_(toplevel) {}
    virtual ~Game_State() noexcept {}

    virtual void handle_event(SDL_Event const&) noexcept = 0;
    virtual void step() noexcept = 0;
    virtual void render() const noexcept = 0;

    virtual void on_enter() noexcept = 0;
    virtual void on_exit() noexcept = 0;

    inline bool is_toplevel() const noexcept { return is_toplevel_; }

  protected:
    Game& game_;
  private:
    bool is_toplevel_;
  };

  enum class View
  {
    Turn, Menu
  };

  inline void push_state(Game& g, std::shared_ptr<Game_State> gs) noexcept
  {
    if(g.states.size()) g.states.back()->on_exit();

    g.states.push_back(gs);
    gs->on_enter();
  }

  // S must satisfy the StandardGameState concept. It has the following
  // requirements:
  // - A constructor with the first parameter being a reference to a Game
  // struct. There can be any number of arguments following.
  template <class S, class... Args>
  inline void push_state(Game& g, Args&&... args) noexcept
  {
    push_state(g, std::make_shared<S>(g, std::forward<Args>(args)...));
  }

  inline void pop_state(Game& g) noexcept
  {
    g.states.back()->on_exit();
    g.states.pop_back();

    if(g.states.size()) g.states.back()->on_enter();
  }
  inline void replace_state(Game& g, std::shared_ptr<Game_State> gs) noexcept
  {
    pop_state(g);
    push_state(g, gs);
  }

  template <class T>
  inline std::shared_ptr<T> get_asset(Game& game, std::string name) noexcept
  {
    return assets::find<T>(game.assets, name);
  }

  void render_all(Game const& game) noexcept;
}
