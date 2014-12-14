/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>

#include "items.h"
namespace game
{
  struct Enemy_Impl
  {
    std::string sprite;
    std::string name;

    int power;
    int life;
  };

  using Enemy = std::shared_ptr<const Enemy_Impl>;

  namespace no
  {
    Enemy enemy;
  }

  struct Enemy_Instance
  {
    Enemy_Instance(Enemy e) noexcept : decl(e), current_line(e.life) {}
    Enemy decl;

    int current_life;
    Item item;
  };

  struct Enemy_Parser
  {
    Enemy_Parser(std::string const& enemy_json);
    inline Enemy get_enemy(std::string const& str) noexcept;
  private:
    std::vector<Enemy> enemies_;
  };

  inline Enemy Enemy_Parser::get_enemy(std::string const& str) noexcept
  {
    using std::begin; using std::end;
    auto e_find = std::find_if(begin(enemies_), end(enemies_),
    [&str](auto const& enemy)
    {
      return str == enemy->name;
    });

    if(e_find == end(enemies_))
    {
      return no::enemy;
    }

    return *e_find;
  }
}
