/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "json.h"
#include <cstdlib>
#include "rapidjson/filereadstream.h"
namespace game
{
  struct File_Wrapper
  {
    File_Wrapper(char const* const filename, char const* const mode) noexcept
                 : fd(std::fopen(filename, mode)) {}
    inline ~File_Wrapper()
    {
      if(fd) std::fclose(fd);
    }

    std::FILE* fd;
  };

  rapidjson::Document parse(std::string const& filename)
  {
    auto file = File_Wrapper{filename.c_str(), "r"};
    if(!file.fd) throw Bad_File{filename};

    constexpr int BUFFER_SIZE = 50;
    char buffer[BUFFER_SIZE];
    auto file_stream = rapidjson::FileReadStream{file.fd, buffer, BUFFER_SIZE};

    auto doc = rapidjson::Document{};
    doc.ParseStream(file_stream);

    if(doc.HasParseError())
    {
      throw Bad_Json{"Error code: " + std::to_string(doc.GetParseError())};
    }

    return doc;
  }

  bool has_json_members(rapidjson::Value const& val,
                        std::vector<std::string> const& mems)
  {
    for(auto str : mems)
    {
      if(!val.HasMember(str.c_str())) return false;
    }
    return true;
  }

  void if_has_member(rapidjson::Value const& val, std::string const& mem,
                     std::function<void (rapidjson::Value const&)> fn) noexcept
  {
    if(val.HasMember(mem.c_str()))
    {
      fn(val[mem.c_str()]);
    }
  }
}
