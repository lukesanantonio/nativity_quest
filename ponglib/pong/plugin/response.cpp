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
#include "response.h"
namespace pong
{
  struct Json_Result_Visitor : public boost::static_visitor<Json::Value>
  {
    template <typename Type>
    Json::Value operator()(Type const& t) const noexcept
    {
      return FORMATTER_TYPE(Type)::dump(t);
    }
  };
}
BEGIN_FORMATTER_SCOPE
{
  DEFINE_PARSER(pong::Response, json)
  {
    pong::Response res;

    res.id = FORMATTER_TYPE(pong::req_id_t)::parse(json[0]);
    try
    {
      res.code = static_cast<pong::Err>(json[1].asUInt());
    }
    catch(std::runtime_error& e)
    {
      throw pong::Invalid_Response_Exception{};
    }

    for(int i = 0; i < json.size() - 2; ++i)
    {
      res.ret_values.push_back(json[i + 2]);
    }

    return res;
  }

  DEFINE_DUMPER(pong::Response, res)
  {
    Json::Value val;

    val.append(FORMATTER_TYPE(pong::req_id_t)::dump(res.id));
    val.append(static_cast<std::underlying_type_t<pong::Err> >(res.code));
    for(int i = 0; i < res.ret_values.size(); ++i)
    {
      val.append(res.ret_values[i]);
    }

    return val;
  }
}
END_FORMATTER_SCOPE
