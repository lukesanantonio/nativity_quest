/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "event.h"
namespace game
{
  enum class State_Type
  {
    Normal, Event_View
  };

  struct State
  {
    State_Type type;
    union
    {
      Event event;
    };
  };
}
