/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include "forward.h"
BEGIN_FORMATTER_SCOPE
{
  template <typename Type>
  struct Enum
  {
    static Type parse(Json::Value const&);
    static Json::Value dump(Type const&);
  };

  template <typename Type>
  Type Enum<Type>::parse(Json::Value const& json)
  {
    return static_cast<Type>(json.asUInt());
  }

  template <typename Type>
  Json::Value Enum<Type>::dump(Type const& val)
  {
    return {static_cast<std::underlying_type_t<Type> >(val)};
  }
}
END_FORMATTER_SCOPE
