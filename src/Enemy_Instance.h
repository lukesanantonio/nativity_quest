/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "decl/enemies.h"
namespace game
{
  struct Enemy_Instance
  {
    Enemy_Instance(decl::Enemy e) noexcept
                   : decl(e), entity_data{e->life, e->life, e->defense} {}
    decl::Enemy decl;

    Entity_Data entity_data;

    int not_fighting = 0;

    Vec<int> pos;
  };

}
