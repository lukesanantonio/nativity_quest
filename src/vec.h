/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once

#include <cmath>
namespace game
{
  template <typename T = int>
  struct Vec
  {
    Vec(T const& x = 0, T const& y = 0) noexcept : x(x), y(y) {}

    template <typename P2>
    Vec(Vec<P2> const& vec) noexcept : x(vec.x), y(vec.y) {}

    T x;
    T y;
  };

  // comparison operators
  template <typename P1, typename P2>
  inline auto operator==(Vec<P1> const& p1, Vec<P2> const& p2) noexcept -> bool
  {
    return p1.x == p2.x && p1.y == p2.y;
  }
  template <typename P1, typename P2>
  inline auto operator!=(Vec<P1> const& p1, Vec<P2> const& p2) noexcept -> bool
  {
    return !(p1 == p2);
  }

  template <typename ret_t = double, typename P>
  inline ret_t length(Vec<P> const& vec) noexcept
  {
    ret_t x = static_cast<ret_t>(vec.x);
    ret_t y = static_cast<ret_t>(vec.y);

    x *= x;
    y *= y;

    return std::sqrt(x + y);
  }

  template <typename Ret_Point_T = double, typename P>
  inline auto normalize(Vec<P> const& vec) noexcept -> decltype(auto)
  {
    Vec<Ret_Point_T> result;

    auto vec_len = length<Ret_Point_T>(vec);
    if(vec_len == 0.0) return result;

    result.x = vec.x / vec_len;
    result.y = vec.y / vec_len;

    return result;
  }

  // fun stuff
  template <typename P, typename Angle_T>
  inline auto rotate(Vec<P> const& vec, Angle_T angle) noexcept -> Vec<P>
  {
    auto ret = Vec<P>{};

    ret.x = (vec.x * std::cos(angle)) - (vec.y * std::sin(angle));
    ret.y = (vec.x * std::sin(angle)) + (vec.y * std::cos(angle));

    return ret;
  }

  // addition
  template <typename P1, typename P2> inline auto
  operator+(Vec<P1> const& lhs, Vec<P2> const& rhs) noexcept -> decltype(auto)
  {
    return Vec<decltype(P1() + P2())>{lhs.x + rhs.x, lhs.y + rhs.y};
  }
  template <typename P1, typename P2> inline auto
  operator+=(Vec<P1>& lhs, Vec<P2> const& rhs) noexcept -> Vec<P1>&
  {
    lhs.x += rhs.x;
    lhs.y += rhs.y;

    return lhs;
  }

  // subtraction
  template <typename P1, typename P2> inline auto
  operator-(Vec<P1> const& lhs, Vec<P2> const& rhs) noexcept -> decltype(auto)
  {
    return Vec<decltype(P1() - P2())>{lhs.x - rhs.x, lhs.y - rhs.y};
  }
  template <typename P1, typename P2> inline auto
  operator-=(Vec<P1>& lhs, Vec<P2> const& rhs) noexcept -> Vec<P1>&
  {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;

    return lhs;
  }

  // multiplication with a scalar
  template <typename P1, typename Scalar> inline auto
  operator*(Vec<P1> const& lhs, Scalar rhs) noexcept -> Vec<P1>
  {
    return Vec<P1>{lhs.x * rhs, lhs.y * rhs};
  }
  template <typename P1, typename Scalar> inline auto
  operator*=(Vec<P1>& lhs, Scalar rhs) noexcept -> Vec<P1>&
  {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;

    return lhs;
  }

  // division with a scalar
  template <typename P1, typename Scalar> inline auto
  operator/(Vec<P1> const& lhs, Scalar rhs) noexcept -> Vec<P1>
  {
    return Vec<P1>{lhs.x / rhs, lhs.y / rhs};
  }
  template <typename P1, typename Scalar> inline auto
  operator/=(Vec<P1>& lhs, Scalar rhs) noexcept -> Vec<P1>&
  {
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;

    return lhs;
  }
}

