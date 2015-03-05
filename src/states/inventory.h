/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../ui/ui.h"

#include "navigate.h"
namespace game
{
  struct Inventory_View_State : public Navigate_Sub_State
  {
    Inventory_View_State(Game& g, Navigate_State& ns) noexcept;

    void handle_event(SDL_Event const&) noexcept;
    void step() noexcept;
    void render() const noexcept;

    void on_enter() noexcept override;
    void on_exit() noexcept override;

    ui::Shared_View hud;

    bool clicked = false;

    int selected;

    int delta = 0;
    int cur_step = 0;
    int max_step = 0;
    bool anim = false;
  protected:
    void set_sprite_src(decl::Item item) noexcept;

  private:
    virtual void on_inventory_label_select() noexcept;
    virtual void on_extra_label_select() noexcept;

    void update_labels() noexcept;
  };
}
