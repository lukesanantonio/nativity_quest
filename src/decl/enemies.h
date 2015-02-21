/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>

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

    std::random_device r_device_;
  };

  inline Enemy Enemies::get_enemy(std::string const& str) noexcept
  {
    if(str == "random")
    {
      std::mt19937 prng{r_device_()};
      std::uniform_int_distribution<int> dist(0, enemies_.size() - 1);
      return enemies_[dist(prng)];
    }

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
