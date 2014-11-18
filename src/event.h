/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <random>
#include <queue>

namespace game
{
  enum class Enemy
  {
    Monster
  };
  struct Fight
  {
    Enemy enemy;
  };

  enum class Chest_Item
  {
    Pie
  };
  struct Chest
  {
    Chest_Item contents;
  };

  enum class Event_Type
  {
    Fight, Chest
  };
  struct Event
  {
    Event_Type type;
    union
    {
      Fight fight;
      Chest chest;
    };
  };

  struct Event_Gen
  {
    void step() noexcept;
    bool poll_event(Event& event) noexcept;
  private:
    std::mt19937 prng_;
    std::queue<Event> event_queue_;
  };
}
