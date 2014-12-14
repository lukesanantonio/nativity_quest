/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include <thread>
#include <chrono>
#include <iostream>

#include "pong/common/Timer.hpp"

#include "Graphics_Desc.h"
#include "Sprite_Container.h"
#include "decl/items.h"
#include "decl/zones.h"
#include "State.h"
#include "step.h"

#include "util/except.h"

#define SPRITES_JSON "assets/sprites.json"

#define CHAR_JSON "assets/char.json"
#define ITEMS_JSON "assets/items.json"
#define ZONES_JSON "assets/zones.json"
#define ENEMIES_JSON "assets/enemies.json"

#define FONT_FILE "/usr/share/fonts/TTF/DejaVuSans.ttf"

int main(int argc, char** argv)
{
  try
  {
    pong::Logger log;

    game::Graphics_Desc g{"Tommy's Game", {1000, 1000}, FONT_FILE, &log};

    game::Sprite_Container sprites(SPRITES_JSON);

    game::State state;
    state.running = true;
    state.type = game::View::Turn;
    state.state_data = game::Turn_Data{ITEMS_JSON, ZONES_JSON,
                                       CHAR_JSON, ENEMIES_JSON};

    pong::Timer<> timer;

    while(state.running)
    {
      // Everything updates 60 times per second (16 ms/frame).
      if(!timer.has_been(std::chrono::milliseconds(16)))
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        continue;
      }

      timer.reset();

      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
        if(event.type == SDL_QUIT)
        {
          state.running = false;
          continue;
        }
        handle_event(state, event);
      }

      step(state);
      render(state, g, sprites);

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
