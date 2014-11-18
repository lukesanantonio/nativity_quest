/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "event.h"
namespace game
{
  void Event_Gen::step() noexcept
  {
    auto dist = std::uniform_int_distribution<int>{0, 50};

    // If the first generated number is zero spawn an enemy.
    if(dist(prng_) == 0)
    {
      auto event = Event{Event_Type::Fight};
      event.fight = Fight{Enemy::Monster};
      event_queue_.push(event);
    }
    // If the second generated number is zero spawn a pie.
    else if(dist(prng_) == 0)
    {
      auto event = Event{Event_Type::Chest};
      event.chest = Chest{Chest_Item::Pie};
      event_queue_.push(event);
    }
  }
  bool Event_Gen::poll_event(Event& event) noexcept
  {
    if(event_queue_.size())
    {
      event = event_queue_.front();
      event_queue_.pop();
      return true;
    }
    return false;
  }
}
