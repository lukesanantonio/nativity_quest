/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "move.h"
namespace game
{
  void Movement_State::step() noexcept
  {
    // Player movement!

    auto& player = navigate.map.players[navigate.player];

    // Find out the distance we have yet to travel.
    auto delta_len = length(delta);

    // Max step size for the player.
    auto max_speed = 1.0;

    max_speed /= navigate.cur_zone->speed_cost;

    auto max_movement = navigate.effects.max_movement(player);

    if(player.turns_of_haste)
    {
      max_movement += 30.0;
      --player.turns_of_haste;
    }

    // Isolate the direction.
    auto unit_delta = normalize<>(delta);
    // How much do we move this step?
    auto move_length =
      std::min(max_speed,
               std::min(max_movement - player.moved, delta_len));
    auto move_delta = unit_delta * move_length;

    // Move the player.
    player.pos += move_delta;

    navigate.update_cur_zone();

    // Check to make sure we can be in this zone.
    if(navigate.cur_zone->required_item != assets::no::item)
    {
      using std::begin; using std::end;
      auto item_find =
        std::find_if(begin(player.inventory), end(player.inventory),
        [this](auto const& item)
        {
          return navigate.cur_zone->required_item == item;
        });

      // We don't have the required item.
      if(item_find == end(player.inventory))
      {
        // Cancel the movement.
        player.pos -= move_delta;

        navigate.update_cur_zone();

        pop_state(game_);
        return;
      }
    }

    // We can continue.

    // Mark some distance traveled.
    delta -= move_delta;
    player.moved += move_length * navigate.cur_zone->speed_cost;

    unfog(player, navigate.effects);

    navigate.move = move_delta;

    // If we have less than the max units per step, it means we just
    // completed that and we can become static.
    if(delta_len < max_speed || max_movement < player.moved + .9)
    {
      pop_state(game_);
    }
  }
  void Movement_State::on_exit() noexcept
  {
    navigate.move = {0, 0};
  }
}
