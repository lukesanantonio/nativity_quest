/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include <thread>
#include <chrono>
#include <iostream>

#include "pong/common/Timer.hpp"

#include "game/Graphics_Desc.h"
#include "game/Game.h"

#include "decl/items.h"
#include "decl/zones.h"
#include "decl/game.h"

#include "states/menu.h"

#include "assets/discovery.h"
#include "assets/load.h"

#include "common/except.h"
#include "common/log.h"

#define GAME_DECL_JSON "assets/game.json"
#define FONT_FILE "assets/DejaVuSans.ttf"

int main(int argc, char** argv)
{
  try
  {
    const auto game_decl = game::decl::Game(GAME_DECL_JSON);

    auto g = game::Graphics_Desc{game_decl};

    auto font = game::Font_Renderer{FONT_FILE};
    auto game = game::Game{std::move(g), std::move(font)};

    game::Scoped_Log_Init scoped_log_init{};

    // Load assets
    auto assets = game::assets::discover("assets");
    game::assets::load(game, assets);

    // Our menu is going to be our top level state.
    push_state(game, std::make_shared<game::Menu_State>(game));

    pong::Timer<> timer;

    while(game.running && game.states.size())
    {
      // Everything updates 60 times per second (16 ms/frame).
      if(!timer.has_been(std::chrono::milliseconds(16)))
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        continue;
      }

      timer.reset();

      auto state = game.states.back();

      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
        if(event.type == SDL_QUIT)
        {
          game.running = false;
          continue;
        }
        if(event.type == SDL_KEYDOWN)
        {
          if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
          {
            game::pop_state(game);
            continue;
          }
        }

        state->handle_event(event);
      }

      // Did an event cause the state to change?
      if(state != game.states.back()) continue;

      state->step();

      game::render_all(game);

      game::flush_log();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  }
  catch(game::Bad_Asset& e)
  {
    std::cerr << e.filename << ": " << e.diagnostic << std::endl;
    return EXIT_FAILURE;
  }
  catch(game::Bad_File& e)
  {
    std::cerr << "Failed to open '" << e.filename << "'" << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}
