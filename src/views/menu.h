/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>

#include "SDL.h"

#include "../Graphics_Desc.h"

#include "../ui/Model.h"
#include "../ui/Presenter.h"
#include "../ui/View.h"

namespace game
{
  struct Menu_Data
  {
    Menu_Data(Graphics_Desc& g, std::string const& menu_json);
    Menu_Data(Menu_Data&& md) noexcept;

    ui::Model model;
    ui::Presenter presenter;
    ui::View view;
  };

  struct State;
  struct Sprite_Container;

  void handle_event_state(State& s, Menu_Data&, SDL_Event const&) noexcept;
  void step_state(State& s, Menu_Data&) noexcept;
  void render_state(State& s, Graphics_Desc&,
                    Sprite_Container&, Menu_Data&) noexcept;
}

