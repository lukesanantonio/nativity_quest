/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "menu.h"
#include "../common/json.h"
#include "navigate.h"

#define NAVIGATE_SPRITES "assets/navigation_sprites.json"
#define PLAYERS_JSON "assets/player.json"
#define MAP_JSON "assets/map.json"
#define ITEMS_JSON "assets/items.json"
#define ENEMIES_JSON "assets/enemies.json"
namespace game
{
  Menu_State::Menu_State(Game& game, std::string const& menu_json)
                         : model{parse_json(menu_json)}, presenter{},
                           view{game.font}, Game_State(game)
  {
    presenter.present(model, view, game.graphics.size());

    presenter.use_handler("on_start", [this](Vec<int>)
    {
      push_state(game_,
                 std::make_shared<Navigate_State>(game_,
                                                  NAVIGATE_SPRITES,
                                                  PLAYERS_JSON, MAP_JSON,
                                                  ITEMS_JSON, ENEMIES_JSON));
    });
  }

  void Menu_State::handle_event(SDL_Event const& event) noexcept
  {
    presenter.event_notify(event);
  }
  void Menu_State::step() noexcept {}
  void Menu_State::render() const noexcept
  {
    view.render(game_.graphics);
  }
}
