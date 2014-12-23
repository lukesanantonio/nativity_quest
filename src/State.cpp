/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "State.h"
namespace game
{
  template <class InputIt, class UnaryPredicate>
  InputIt find_last_if(InputIt first, InputIt last, UnaryPredicate pred)
  {
    auto last_found = last;
    for(; first != last; ++first)
    {
      if(pred(*first))
      {
        last_found = first;
      }
    }
    return last_found;
  }

  auto find_toplevel(State::stack_t const& stack) noexcept -> decltype(auto)
  {
    using std::begin; using std::end;
    return find_last_if(begin(stack), end(stack),
    [](auto const& state)
    {
      return state->is_toplevel();
    });
  }

  void render(State const& state, Graphics_Desc& g,
              Sprite_Container& s) noexcept
  {
    using std::begin; using std::end;

    auto toplevel = find_toplevel(state.state_stack);
    if(toplevel == end(state.state_stack)) return;

    SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(g.renderer);

    std::for_each(toplevel, end(state.state_stack),
    [&](auto& game_state)
    {
      game_state->render(state, g, s);
    });

    SDL_RenderPresent(g.renderer);
  }

  void handle_event(State& s, SDL_Event const& event) noexcept
  {
    s.state_stack.back()->handle_event(s, event);
  }
  void step(State& s) noexcept
  {
    s.state_stack.back()->step(s);
  }
}
