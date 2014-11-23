/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "State.h"
namespace game
{
  State::State() noexcept : type(View::Menu), state_data(Menu_Data{}) {}
}
