/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>

#include "../State.h"

#include "SDL.h"

#include "../ui/Model.h"
#include "../ui/Presenter.h"
#include "../ui/View.h"

namespace game
{
  struct Menu_Data : public Game_State
  {
    Menu_Data(Graphics_Desc& g, std::string const& menu_json);
    Menu_Data(Menu_Data&& md) noexcept;

    ui::Model model;
    ui::Presenter presenter;
    ui::View view;

    void handle_event(State&, SDL_Event const&) noexcept override;
    void step(State&) noexcept override;
    void render(State const&, Graphics_Desc&,
                Sprite_Container&) const noexcept override;

    inline bool is_toplevel() const noexcept override { return true; }
  };
}

