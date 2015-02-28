/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "asset.h"
#include "find.h"
namespace game { namespace assets
{
  template <class Tag>
  using Asset_Type = typename Tag::asset_t;

  template <class Tag>
  using Asset_Ptr = std::shared_ptr<Asset_Type<Tag> >;

  template <class Tag, class Ptr>
  Tag describe(Ptr ptr) noexcept
  {
    Tag tag;
    decl(ptr, tag);
    return tag;
  }

  template <class Tag>
  Tag describe(assets::Vector vec, std::string name) noexcept
  {
    // Find the correct asset type using the declaration tag and the passed in
    // name!
    auto asset = find<Asset_Type<Tag> >(vec, name);
    // ^ Probably a shared_ptr with the correct (or nullptr) asset type.

    // Use whatever declaration tag was provided with the asset that was found.
    return describe<Tag>(asset);
  }
} }
