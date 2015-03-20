/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "lang.h"
namespace game { namespace assets
{
  void describe(Asset_Ptr<Lang> const& json, Lang& lang,
                assets::Vector&) noexcept
  {
    auto const& doc = json->json;

    for(auto iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter)
    {
      auto key = std::string{iter->name.GetString()};
      lang.dict[key] = iter->value.GetString();
    }
  }
} }

