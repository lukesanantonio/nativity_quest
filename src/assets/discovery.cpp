/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "discovery.h"

#include "../common/log.h"

#include <boost/filesystem.hpp>
namespace game { namespace assets
{
  namespace fs = boost::filesystem;
  auto discover(std::string subdir) noexcept -> std::vector<std::string>
  {
    auto assets = std::vector<std::string>{};

    auto asset_dir = fs::system_complete(subdir);

    if(fs::is_directory(asset_dir))
    {
      auto iter = fs::recursive_directory_iterator{asset_dir};
      auto end = fs::recursive_directory_iterator{};
      for(; iter != end; ++iter)
      {
        if(fs::is_regular_file(*iter))
        {
          // We have a file that we can presumably load.
          // Store the relative path.
          assets.push_back(iter->path().relative_path().native());
          log_i("Found: " + assets.back());
        }
      }
    }
    else
    {
      log_e("Asset dir provided (" + asset_dir.native() +
            ") not a directory");
    }

    return assets;
  }
} }
