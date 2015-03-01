/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <tuple>
#include <type_traits>
namespace game
{
  template <int N, class F, class Tuple_Type, class... Args>
  inline auto
  call_impl(std::enable_if_t<N == std::tuple_size<Tuple_Type>::value, F> f,
            Tuple_Type& tup, Args&&... args) -> decltype(auto)
  {
    return f(std::forward<Args>(args)...);
  }

  template <int N, class F, class Tuple_Type, class... Args>
  inline auto
  call_impl(std::enable_if_t<N < std::tuple_size<Tuple_Type>::value, F> f,
            Tuple_Type& tup, Args&&... args) -> decltype(auto)
  {
    return
    call_impl<N + 1, F, Tuple_Type, Args...>(f, tup,
                                             std::forward<Args>(args)...,
                                             std::get<N>(tup));
  }


  template <class F, class Tuple_Type, class... Args>
  inline auto call(F f, Tuple_Type& tup, Args&&... args) -> decltype(auto)
  {
    return call_impl<0, F, Tuple_Type>(f, tup, std::forward<Args>(args)...);
  }
}
