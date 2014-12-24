/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>
#include <vector>

namespace game { namespace decl
{
  struct Enemy_Impl
  {
    std::string sprite;
    std::string id;
    std::string str;

    int defense;
    int life;
  };

  using Enemy = std::shared_ptr<const Enemy_Impl>;

  namespace no
  {
    const Enemy enemy;
  }

  struct Enemies
  {
    Enemies(std::string const& enemy_json);
    inline Enemy get_enemy(std::string const& str) noexcept;
  private:
    std::vector<Enemy> enemies_;
  };

  inline Enemy Enemies::get_enemy(std::string const& str) noexcept
  {
    using std::begin; using std::end;
    auto e_find = std::find_if(begin(enemies_), end(enemies_),
    [&str](auto const& enemy)
    {
      return str == enemy->id;
    });

    if(e_find == end(enemies_))
    {
      return no::enemy;
    }

    return *e_find;
  }
} }
