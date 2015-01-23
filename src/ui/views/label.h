/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../View.h"
#include "../../game/Font_Renderer.h"

#include "../triggers/Mouse_Click_Trigger.h"

#include "../../common/surface.h"
namespace game { namespace ui
{
  struct Label : public View
  {
    Label(Graphics_Desc& g, Font_Renderer& fr) noexcept : View(g), fr_(fr) {}

    Vec<int> get_minimum_extents() const noexcept override;
    void dispatch_event(SDL_Event const&) noexcept override;

    Volume<int> layout_() override;

    inline std::string str() const noexcept { return str_; }
    inline void str(std::string str) noexcept
    {
      if(str_ != str)
      {
        str_ = str;
        invalidate_cache_();
      }
    }

    inline int size() const noexcept { return size_; }
    inline void size(int size) noexcept
    {
      if(size_ != size)
      {
        size_ = size;
        invalidate_cache_();
      }
    }

    inline Color color() const noexcept { return col_; }
    inline void color(Color col) noexcept
    {
      if(col_ != col)
      {
        col_ = col;
        invalidate_cache_();
      }
    }

    inline bool click() const noexcept { return click_; }
    inline void click(bool c) noexcept { click_ = c; }

    inline void use_click_handler(Mouse_Click_Trigger::func_t func) noexcept
    { click_trigger_.use_handler(func); }
  private:
    std::string str_ = "";
    int size_ = 40;
    Color col_ = Color{};

    bool click_;
    Mouse_Click_Trigger click_trigger_;

    Font_Renderer& fr_;

    void render_() const noexcept override;

    mutable Texture_Ptr texture_cache_;
    void gen_cache_() const noexcept;

    inline void invalidate_cache_() const noexcept
    { texture_cache_.reset(nullptr); }
  };
} }
