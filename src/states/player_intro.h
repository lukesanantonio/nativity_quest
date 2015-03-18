/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "intro.h"
namespace game
{
  struct Player_Intro : public Intro_State
  {
    Player_Intro(Game&, Navigate_State&, std::string) noexcept;

    void on_enter() noexcept;
    void on_exit() noexcept;
  private:
    std::string pn_;
  };
}
