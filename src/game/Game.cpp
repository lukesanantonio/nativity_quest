/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Game.h"
#include <algorithm>
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

  auto find_toplevel(Game::stack_t const& stack) noexcept -> decltype(auto)
  {
    using std::begin; using std::end;
    return find_last_if(begin(stack), end(stack),
    [](auto const& state)
    {
      return state->is_toplevel();
    });
  }

  void render_all(Game const& game) noexcept
  {
    using std::begin; using std::end;

    auto toplevel = find_toplevel(game.states);
    if(toplevel == end(game.states)) return;

    SDL_SetRenderDrawColor(game.graphics.renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(game.graphics.renderer);

    std::for_each(toplevel, end(game.states),
    [&](auto& game_state)
    {
      game_state->render();
    });

    SDL_RenderPresent(game.graphics.renderer);
  }
}
