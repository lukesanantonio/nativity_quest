/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "Player.h"
#include "common/animation.h"
#include "ui/views/bar.h"
namespace game
{
  namespace detail
  {
    enum class Mediator_State
    {
      None, Anim
    };
  }

  /*!
   * \brief Takes a player reference and a bar reference and animates the bar
   * to match the player's health.
   *
   */
  struct Player_Health_Mediator
  {
    Player_Health_Mediator(Entity_Data* ed = nullptr,
                           ui::Bar* b = nullptr) noexcept;

    void step() noexcept;

    Entity_Data* entity_data() const noexcept;
    void entity_data(Entity_Data*) noexcept;

    ui::Bar* bar() const noexcept;
    void bar(ui::Bar*) noexcept;

  private:
    int cached_cur_health_ = 0;

    Entity_Data* entity_;
    ui::Bar* bar_;

    detail::Mediator_State state_;
    Animation anim;

    void reset_bar() noexcept;
  };
}
