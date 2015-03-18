/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include <thread>
#include <chrono>
#include <iostream>

#include <uv.h>

#include "pong/common/Timer.hpp"

#include "game/Graphics_Desc.h"
#include "game/Game.h"

#include "decl/items.h"
#include "decl/zones.h"
#include "decl/game.h"

#include "states/menu.h"
#include "states/view_hud.h"

#include "assets/assets.h"

#include "common/except.h"
#include "common/log.h"
#include "common/translate.h"

#define GAME_JSON "game"
#define FONT_FILE "assets/DejaVuSans.ttf"

#ifdef LANG_LATIN
  #define INITIAL_LANG_ASSET "lang/latin"
#else
  #define INITIAL_LANG_ASSET "lang/english"
#endif

int main(int argc, char** argv)
{
  try
  {
    // TODO: Make this threadsafe maybe?
    game::Scoped_Log_Init scoped_log_init{};
    game::set_log_level(game::Log_Severity::Debug);

    // Enter the assets directory.
    if(uv_chdir("assets"))
    {
      game::log_e("Failed to enter asset directory.");
      return EXIT_FAILURE;
    }

    // Load any assets discovered in the current directory (assets/).
    namespace asts = game::assets;
    auto assets = asts::load(asts::discover("."));

    // Go up a directory.
    if(uv_chdir(".."))
    {
      game::log_e("Failed to return to original directory.");
      return EXIT_FAILURE;
    }

    const auto game_decl = asts::describe<asts::Game>(assets, GAME_JSON);

    auto g = game::Graphics_Desc{game_decl};

    auto font = game::Font_Renderer{FONT_FILE};
    auto game = game::Game{std::move(g), std::move(font)};

    game::set_lang(asts::describe<asts::Lang>(assets, INITIAL_LANG_ASSET));

    game.assets = std::move(assets);

    if(argc > 1)
    {
      // Load that ui asset and view it.
      push_state(game, std::make_shared<game::View_Hud_State>(game, argv[1]));
    }
    else
    {
      // Otherwise start a standard game in which our menu is going to be our
      // top level state.
      push_state(game, std::make_shared<game::Menu_State>(game));
    }

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
