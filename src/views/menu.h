/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>

#include "../Game.h"

#include "SDL.h"

#include "../ui/Model.h"
#include "../ui/Presenter.h"
#include "../ui/View.h"

namespace game
{
  struct Menu_Data : public Game_State
  {
    Menu_Data(Game& game, std::string const& menu_json);

    ui::Model model;
    ui::Presenter presenter;
    ui::View view;

    void handle_event(SDL_Event const&) noexcept override;
    void step() noexcept override;
    void render() const noexcept override;

    inline bool is_toplevel() const noexcept override { return true; }
  };
}

