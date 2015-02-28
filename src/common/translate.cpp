/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "translate.h"
namespace game
{
  // TODO: Possibly just make this a const&?
  assets::Lang language_;

  void set_lang(assets::Lang const& lang) noexcept
  {
    language_ = lang;
  }
  assets::Lang get_lang() noexcept
  {
    return language_;
  }
}
