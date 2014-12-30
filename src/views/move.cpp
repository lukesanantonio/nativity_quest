/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "move.h"
#include "navigate.h"
namespace game
{
  void Movement_State::step() noexcept
  {
    // Player movement!

    auto& player = ns.map.players[ns.player];

    // Find out the distance we have yet to travel.
    auto delta_len = length(delta);

    // Max step size for the player.
    auto max_speed = 1.0;

    auto max_movement = player.max_movement;

    // Isolate the direction.
    auto unit_delta = normalize<>(delta);
    // How much do we move this step?
    auto move_length =
      std::min(max_speed,
               std::min(max_movement - player.moved, delta_len));
    auto move_delta = unit_delta * move_length;

    // Move the player.
    player.pos += move_delta;

    /*
    auto cur_zone = update_zone(turn);

    // Check to make sure we can be in this zone.
    if(cur_zone->required_item != no::item)
    {
      using std::begin; using std::end;
      auto item_find =
        std::find_if(begin(player.inventory), end(player.inventory),
        [&cur_zone](auto const& item)
        {
          return cur_zone->required_item == item;
        });

      // We don't have the required item.
      if(item_find == end(player.inventory))
      {
        // Cancel the movement.
        player.pos -= move_delta;
        update_zone(turn);
        return Waiting_Data{};
      }
    }
    */

    // We can continue.

    // Mark some distance traveled.
    delta -= move_delta;
    player.moved += move_length;

    // If we have less than the max units per step, it means we just
    // completed that and we can become static.
    if(delta_len < max_speed || max_movement <= player.moved)
    {
      pop_state(game_);
    }
  }
}
