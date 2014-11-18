/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <array>
#include <memory>

#include "SDL.h"
#include "SDL_image.h"

#include "player.h"

#define VIEWPORT_SIZE 400

#define MAP_SIZE 1000

#define MINI_MAP_START 5
#define MINI_MAP_SIZE 250
#define MINI_MAP_PLAYER_SIZE 3

#define WINDOW_SIZE 1000
#define WINDOW_TITLE "Game"

#define MAP_RES 0
#define CHAR_RES 1
#define MONSTER_RES 2
#define PIE_RES 3

namespace game
{
  [[noreturn]] void crash(char const* msg)
  {
    std::cerr << msg << std::endl;
    std::exit(EXIT_FAILURE);
  }

  struct Game
  {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    std::vector<SDL_Texture*> res;

    game::State state{game::State_Type::Normal};

    game::Event_Gen event_gen;

    Player player;
    SDL_Texture* war_fog;
  };

  void handle_event(Game& game, SDL_Event const& event)
  {
    if(event.type == SDL_QUIT)
    {
      game.running = false;
    }
    // Only allow the player to move if we don't have an event going on.
    using game::State_Type;
    if(game.state.type == State_Type::Normal)
    {
      if(event.type == SDL_KEYDOWN)
      {
        // On key down, possibly generate new game events.
        game.event_gen.step();
      }
      handle_player_event(event, game.player);
    }
    else
    {
      if(event.type == SDL_KEYDOWN)
      {
        if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
          game.state.type = State_Type::Normal;
        }
      }
    }
  }

  void update(Game& game)
  {
    using game::State_Type;
    if(game.state.type != State_Type::Normal) return;

    game::Event event;
    if(game.event_gen.poll_event(event))
    {
      game.state.type = State_Type::Event_View;
      game.state.event = event;
    }

    // Create a temporary surface we can draw on.
    SDL_Surface* war_fog = SDL_CreateRGBSurface(0, MAP_SIZE, MAP_SIZE, 32,
                                                0xff000000,
                                                0x00ff0000,
                                                0x0000ff00,
                                                0x000000ff);

    // Make every part of the map hidden.
    SDL_FillRect(war_fog, NULL, SDL_MapRGBA(war_fog->format, 0, 0, 0, 0xff));

    uint32_t clear = SDL_MapRGBA(war_fog->format, 0x00, 0x00, 0x00, 0x00);

    // Draw three rectangles to form some circle around the player position.
    SDL_Rect rect;
    rect.x = game.player.pos.x - VIEWPORT_SIZE / 2;
    rect.y = game.player.pos.y - VIEWPORT_SIZE / 2;
    rect.w = VIEWPORT_SIZE;
    rect.h = VIEWPORT_SIZE;
    SDL_FillRect(war_fog, &rect, clear);

    // Destroy the old war fog, if it existed.
    if(game.war_fog) SDL_DestroyTexture(game.war_fog);
    // Make a new texture.
    game.war_fog = SDL_CreateTextureFromSurface(game.renderer, war_fog);

    // Free the temporary surface.
    SDL_FreeSurface(war_fog);
  }

  SDL_Texture* load_resource(char const* filename, char const* fail_msg,
                             SDL_Renderer* renderer)
  {
    SDL_Surface* img = IMG_Load(filename);
    if(!img)
    {
      crash(fail_msg);
    }

    SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0xff, 0x00, 0xff));
    SDL_SetSurfaceRLE(img, SDL_TRUE);

    // Make the texture
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, img);
    if(!text)
    {
      crash("Failed to create texture!");
    }

    SDL_FreeSurface(img);
    return text;
  }

  void init_game(Game& g)
  {
    // Load the map
    g.res.push_back(load_resource("assets/map.png",
                                  "Failed to load game map!",
                                  g.renderer));
    g.res.push_back(load_resource("assets/char.png",
                                  "Failed to load character!",
                                  g.renderer));
    g.res.push_back(load_resource("assets/monster.png",
                                  "Failed to load enemy!",
                                  g.renderer));
    g.res.push_back(load_resource("assets/pie.png",
                                  "Failed to load pie!",
                                  g.renderer));
  }

  void uninit_game(Game& g)
  {
    for(SDL_Texture* surf : g.res)
    {
      SDL_DestroyTexture(surf);
    }

    // If we still have our war fog, destroy it.
    if(g.war_fog) SDL_DestroyTexture(g.war_fog);
  }

  void render(Game& game)
  {
    SDL_SetRenderDrawColor(game.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(game.renderer);

    {
      // Render the map...
      SDL_Rect viewport;
      viewport.x = game.player.pos.x - (VIEWPORT_SIZE / 2);
      viewport.y = game.player.pos.y - (VIEWPORT_SIZE / 2);
      viewport.w = VIEWPORT_SIZE + std::min(0, viewport.x);
      viewport.h = VIEWPORT_SIZE + std::min(0, viewport.y);

      viewport.w = std::min(MAP_SIZE - viewport.x, viewport.w);
      viewport.h = std::min(MAP_SIZE - viewport.y, viewport.h);

      SDL_Rect dest;
      dest.x = -std::min(0, viewport.x) * WINDOW_SIZE / VIEWPORT_SIZE;
      dest.y = -std::min(0, viewport.y) * WINDOW_SIZE / VIEWPORT_SIZE;
      dest.w = viewport.w * WINDOW_SIZE / VIEWPORT_SIZE;
      dest.h = viewport.h * WINDOW_SIZE / VIEWPORT_SIZE;

      viewport.x = std::max(0, viewport.x);
      viewport.y = std::max(0, viewport.y);

      SDL_RenderCopy(game.renderer, game.res[MAP_RES], &viewport, &dest);

      // ... And player.
      SDL_Rect player_src_sprite = get_sprite_from_direction(game.player.dir);

      dest.x = WINDOW_SIZE / 2;
      dest.y = WINDOW_SIZE / 2;
      dest.w = player_src_sprite.w;
      dest.h = player_src_sprite.h;

      SDL_RenderCopy(game.renderer,
                     game.res[CHAR_RES],
                     &player_src_sprite,
                     &dest);
    }
    // Render the mini-map
    {
      // Mini map
      SDL_Rect dest_mini_map;
      dest_mini_map.x = MINI_MAP_START;
      dest_mini_map.y = MINI_MAP_START;
      dest_mini_map.w = MINI_MAP_SIZE;
      dest_mini_map.h = MINI_MAP_SIZE;
      SDL_RenderCopy(game.renderer, game.res[MAP_RES], NULL, &dest_mini_map);

      SDL_SetRenderDrawColor(game.renderer, 0xff, 0xff, 0xff, 0xff);

      // Player location
      SDL_Rect blip;
      blip.x = static_cast<double>(game.player.pos.x) / MAP_SIZE *
               MINI_MAP_SIZE + MINI_MAP_START - MINI_MAP_PLAYER_SIZE / 2;
      blip.y = static_cast<double>(game.player.pos.y) / MAP_SIZE *
               MINI_MAP_SIZE + MINI_MAP_START - MINI_MAP_PLAYER_SIZE / 2;
      blip.w = MINI_MAP_PLAYER_SIZE;
      blip.h = MINI_MAP_PLAYER_SIZE;
      SDL_RenderDrawRect(game.renderer, &blip);

      // Fog of war.
      SDL_RenderCopy(game.renderer, game.war_fog, NULL, &dest_mini_map);

      // Mini map border.
      SDL_SetRenderDrawColor(game.renderer, 0xff, 0xff, 0xff, 0xff);
      auto points = std::array<SDL_Point, 5>{};
      points[0] = {MINI_MAP_START - 1, MINI_MAP_START - 1};
      points[1] = {MINI_MAP_START + MINI_MAP_SIZE, MINI_MAP_START - 1};
      points[2] = {MINI_MAP_START + MINI_MAP_SIZE,
                   MINI_MAP_START + MINI_MAP_SIZE};
      points[3] = {MINI_MAP_START - 1, MINI_MAP_START + MINI_MAP_SIZE};
      points[4] = points[0];
      SDL_RenderDrawLines(game.renderer, &points[0], points.size());
    }

    // Render the current event.
    if(game.state.type == game::State_Type::Event_View)
    {
      switch(game.state.event.type)
      {
        case game::Event_Type::Fight:
        {
          SDL_RenderCopy(game.renderer, game.res[MONSTER_RES], NULL, NULL);
          break;
        }
        case game::Event_Type::Chest:
        {
          SDL_RenderCopy(game.renderer, game.res[PIE_RES], NULL, NULL);
          break;
        }
      }
    }

    SDL_RenderPresent(game.renderer);
  }
}

int main(int argc, char** argv)
{
  using namespace game;

  Game game;

  // Initialize SDL
  game.window = SDL_CreateWindow(WINDOW_TITLE,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 WINDOW_SIZE, WINDOW_SIZE, 0);
  if(!game.window)
  {
    crash("Failed to create window.");
  }
  game.renderer = SDL_CreateRenderer(game.window,
                                     -1, SDL_RENDERER_ACCELERATED);
  if(!game.renderer)
  {
    crash("Failed to create renderer.");
  }

  int flags = IMG_INIT_PNG;
  int init = IMG_Init(flags);
  if((init & flags) != flags)
  {
    crash("Failed to initialize SDL2_image for PNG files.");
  }

  init_game(game);

  game.running = true;
  while(game.running)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      handle_event(game, event);
    }
    update(game);
    render(game);
  }

  uninit_game(game);

  IMG_Quit();
  SDL_DestroyRenderer(game.renderer);
  SDL_DestroyWindow(game.window);
  return 0;
}
