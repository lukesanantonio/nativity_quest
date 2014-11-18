/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <functional>
#include <string>
#include <vector>
#include "rapidjson/document.h"
namespace game
{
  struct Bad_File
  {
    std::string filename;
  };
  struct Bad_Json
  {
    std::string diagnostic;
  };

  rapidjson::Document parse_json(std::string const& filename);

  bool has_json_members(rapidjson::Value const&,
                        std::vector<std::string> const&);

  void if_has_member(rapidjson::Value const&, std::string const&,
                     std::function<void (rapidjson::Value const&)>) noexcept;
}
