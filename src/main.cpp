/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include <thread>
#include <chrono>
#include <iostream>

#include "Graphics_Desc.h"
#include "decl/items.h"
#include "decl/zones.h"
#include "State.h"
#include "step.h"

#include "util/except.h"

#define CHAR_SPRITES "assets/char.png"
#define ITEMS_JSON "assets/items.json"
#define ZONES_JSON "assets/zones.json"
#define MAP_IMAGE "assets/map.png"

int main(int argc, char** argv)
{
  try
  {
    game::Graphics_Desc g{"Tommy's Game", {1000, 1000}};

    game::State state;
    state.running = true;
    state.type = game::View::Turn;
    state.state_data = game::Turn_Data{game::Map{MAP_IMAGE,
                                       ITEMS_JSON, ZONES_JSON}};
    while(state.running)
    {
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
      render(state, g);

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
