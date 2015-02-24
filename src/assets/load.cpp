/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "load.h"

#include "../common/log.h"
#include "../common/json.h"

#include <boost/filesystem.hpp>

#include "SDL_image.h"

namespace game { namespace assets
{
  namespace fs = boost::filesystem;

  void load(Game& game, std::vector<std::string> const& assets) noexcept
  {
    for(auto asset : assets)
    {
      auto path = fs::path{asset};

      // Get the info about it.
      auto file = path.stem().native();
      auto ext = path.extension().native();

      auto fn = path.native();

      auto asset_ptr = std::shared_ptr<Asset>{nullptr};
      if(ext == "json")
      {
        auto json_asset = std::make_shared<Json_Asset>();

        // Load the json
        try
        {
          json_asset->json = parse_json(fn);
        }
        catch(Bad_Asset& e)
        {
          // Log an error.
          log_e("Failed to load json file ('" + fn + "')\n" + e.diagnostic);
          continue;
        }

        asset_ptr = json_asset;
      }
      else if(ext == "png")
      {
        auto image_asset = std::make_shared<Image_Asset>();

        // Load the image.
        image_asset->image = IMG_Load(fn.c_str());

        // Log for failure.
        if(!image_asset->image)
        {
          log_e("Failed to load image file ('" + fn + "')");
          continue;
        }

        asset_ptr = image_asset;
      }
      else
      {
        log_e("Invalid asset with extensions: '" + ext + "'");
        continue;
      }

      // Uses every part of the path except the extension.
      asset_ptr->name = (path.parent_path() / fs::path{file}).native();
      asset_ptr->ext = ext;

      log_i("Successfully loaded asset '" + fn + "'");

      game.assets.push_back(asset_ptr);
    }
  }
} }

