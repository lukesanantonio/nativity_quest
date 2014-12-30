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

#include "Sprite_Container.h"

#include "decl/items.h"
#include "decl/zones.h"

#include "views/menu.h"

#include "common/except.h"

#define SPRITES_JSON "assets/sprites.json"

#define CHAR_JSON "assets/char.json"
#define ITEMS_JSON "assets/items.json"
#define ZONES_JSON "assets/zones.json"
#define ENEMIES_JSON "assets/enemies.json"

#define FONT_FILE "assets/DejaVuSans.ttf"

#define MENU_JSON "assets/menu.json"

int main(int argc, char** argv)
{
  try
  {
    auto g = game::Graphics_Desc{"Tommy's Game", {1000, 720}};
    auto font = game::Font_Renderer{FONT_FILE};
    auto game = game::Game{std::move(g), std::move(font)};
#if 0
                      game::Turn_Data{ITEMS_JSON, ZONES_JSON, CHAR_JSON,
                                      ENEMIES_JSON} };
    {
      auto& turn = boost::get<game::Turn_Data>(state.state_data);

      auto map_sprite = sprites.get_sprite(turn.map->zones.map_asset());
      turn.map->extents = {map_sprite->surface()->w, map_sprite->surface()->h};
    }
#endif

    // Our menu is going to be our top level state.
    push_state(game, std::make_shared<game::Menu_State>(game, MENU_JSON));

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
        if(!game.presenter.event_notify(event))
        {
          state->handle_event(event);
        }
      }

      state->step();

      game::render_all(game);

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
