/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <functional>

#include "SDL.h"

#include "Model.h"
#include "View.h"
namespace game { namespace ui
{
  // Handles view events and updates it and the model accordingly.
  struct Presenter
  {
    using event_func_t = std::function<void (Vec<int>)>;

    // Default copy/move construction/assignment.
    Presenter() noexcept = default;
    Presenter(Presenter const&) = default;
    Presenter(Presenter&&) noexcept = default;

    Presenter& operator=(Presenter const&) = default;
    Presenter& operator=(Presenter&&) noexcept = default;

    // Present the model onto the view
    void present(Model& model, View& view, Vec<int> bounds) const noexcept;

    // Modify the view according to an event relative to the view
    void event_notify(SDL_Event const& event) noexcept;

    // Set the event handler
    void use_handler(std::string const& str, event_func_t func) noexcept;

    void sprites(decl::Sprites* sprites) noexcept
    { sprites_ = sprites; }

    decl::Sprites* sprites() const noexcept
    { return sprites_; }
  private:
    decl::Sprites* sprites_;

    struct Click_Area
    {
      std::string event;
      Volume<int> vol;
    };

    mutable std::vector<Click_Area> buttons_;

    struct Event_Handler
    {
      std::string event;
      event_func_t handler;
    };

    std::vector<Event_Handler> events_;
  };
} }
