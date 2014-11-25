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
#include "req.h"
namespace pong
{
  struct Invalid_Response_Exception {};

  enum class Err : unsigned int
  {
    None = 0,
    Parse,
    Bad_Method,
    Bad_Params,
    Bad_Req,
    Bad_Res,
    Window_Open,
    Window_Closed,
    Bad_Window,
    Bad_ID,
    Out_Of_Objs,
    Bad_Prop,
    Invalid_Path,
    Bad_Snd,
    Bad_Exec,
    Bad_Type
  };

  struct Response
  {
    req_id_t id;
    Err code;
    params_t ret_values;
  };

  inline bool has_err(Response const& res) noexcept
  {
    return res.code != Err::None;
  }
}
BEGIN_FORMATTER_SCOPE
{
  DECLARE_FORMATTER(pong::Response);
}
END_FORMATTER_SCOPE
