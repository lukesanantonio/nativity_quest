/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "sprites.h"

#include <vector>
#include <algorithm>

#include "SDL_image.h"

#include "../common/json.h"
namespace game { namespace decl
{
  Sprite_Impl::Sprite_Impl(SDL_Surface* srf) noexcept : str{""}
  {
    init_cache();
    reset(srf);
  }
  Sprite_Impl::Sprite_Impl(std::string const& str,
                           std::string const& sprite_filename) noexcept
                           : str(str)
  {
    init_cache();
    reset(IMG_Load(sprite_filename.c_str()));
  }

  void Sprite_Impl::init_cache() noexcept
  {
    img_.gen_func([](auto ptr, auto surface, auto renderer) -> Texture_Ptr
    {
      if(!surface.get()) return nullptr;
      return
      Texture_Ptr{SDL_CreateTextureFromSurface(renderer, surface.get())};
    });
  }

  void Sprite_Impl::reset(SDL_Surface* srf) noexcept
  {
    img_.grab_dependency<0>().reset(srf);
    img_.invalidate();
  }

  // Sprite_Impl
  SDL_Surface* Sprite_Impl::surface() const noexcept
  {
    return img_.grab_dependency<0>().get();
  }
  SDL_Texture* Sprite_Impl::texture(SDL_Renderer* r) const noexcept
  {
    img_.set_dependency<1>(r);
    return img_.cache();
  }

  // Sprite_Container
  Sprites::Sprites(std::string const& filename)
  {
    // Get the DOM (is that what it's called?)
    auto doc = parse_json(filename);

    {
      // Iterate through each object.
      int elem = 0;
      auto iter = doc.Begin();
      for(; iter != doc.End(); ++elem, ++iter)
      {
        // Check the validity of each provided object.
        if(!has_json_members(*iter, {"str", "source"}))
        {
          throw Bad_Asset{filename, "Element " + std::to_string(elem) +
                          " is an invalid " + "asset object"};
        }

        auto str = std::string{(*iter)["str"].GetString()};

        auto& source_json = (*iter)["source"];
        if(source_json.IsArray())
        {
          auto sprites_vec = std::vector<Sprite>{};

          for(auto source_iter = source_json.Begin();
              source_iter != source_json.End();
              ++source_iter)
          {
            auto f = std::string{source_iter->GetString()};
            sprites_vec.emplace_back(std::make_shared<Sprite_Impl>(str, f));

            if(!sprites_vec.back()->surface())
            {
              throw Bad_Asset{filename, "Failed to load '" + str + "'"};
            }
          }

          sprites_.emplace_back(std::move(sprites_vec));
        }
        else
        {
          auto source = std::string{(*iter)["source"].GetString()};

          // Add this sprite!
          sprites_.push_back({std::make_shared<Sprite_Impl>(str, source)});
        }
      }
    }

    // Set up a color key for each sprite.
    for(auto const& sprite_vec : sprites_)
    {
      for(auto sprite : sprite_vec)
      {
        auto sf = sprite->surface();
        SDL_SetColorKey(sf, SDL_TRUE, SDL_MapRGB(sf->format, 0xff, 0x00, 0xff));
      }
    }
  }

  Sprite Sprites::get_sprite(std::string id, int frame) const
  {
    using std::begin; using std::end;
    auto sprite_vec_find = std::find_if(begin(sprites_), end(sprites_),
    [&id](auto const& sprite_vec) -> bool
    {
      if(sprite_vec.empty()) return false;
      return sprite_vec[0]->str == id;
    });

    if(sprite_vec_find == end(sprites_))
    {
      throw Unknown_Sprite_Error{id};
    }

    return (*sprite_vec_find)[frame];
  }
} }
