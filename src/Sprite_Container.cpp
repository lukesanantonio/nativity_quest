/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Sprite_Container.h"

#include <vector>

#include "SDL_image.h"

#include "util/json.h"
namespace game
{
  Sprite_Impl::Sprite_Impl(int id,
                           std::string const& str,
                           std::string const& sprite_filename) noexcept
                           : id(id), str(str)
  {
    img_.gen_func([](auto ptr, auto surface, auto renderer) -> Texture_Ptr
    {
      if(!surface.get()) return nullptr;
      return
      Texture_Ptr{SDL_CreateTextureFromSurface(renderer, surface.get())};
    });

    img_.grab_dependency<0>().reset(IMG_Load(sprite_filename.c_str()));
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
  Sprite_Container::Sprite_Container(std::string const& filename) noexcept
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
        if(!has_json_members(*iter, {"id", "str", "file"}))
        {
          throw Bad_Asset{filename, "Element " + std::to_string(elem) +
                          " is an invalid " +
                          "asset object"};
        }

        // Get the individual members.
        auto id = int{(*iter)["id"].GetInt()};
        auto str = std::string{(*iter)["str"].GetString()};
        auto file = std::string{(*iter)["file"].GetString()};

        // Add this sprite!
        sprites_.emplace_back(std::make_shared<Sprite_Impl>(id, str, file));
      }
    }

    // Set up a color key for each sprite.
    for(auto sprite : sprites_)
    {
      auto sf = sprite->surface();
      SDL_SetColorKey(sf, SDL_TRUE, SDL_MapRGB(sf->format, 0xff, 0x00, 0xff));
    }
  }

  Sprite Sprite_Container::get_sprite(sprite_id id) const noexcept
  {
    struct Find_Visitor : boost::static_visitor<bool>
    {
      Find_Visitor(Sprite s) noexcept : sprite_(s) {}

      inline bool operator()(int id) const noexcept
      {
        return sprite_->id == id;
      }
      inline bool operator()(std::string const& str) const noexcept
      {
        return sprite_->str == str;
      }
    private:
      Sprite sprite_;
    };

    using std::begin; using std::end;
    auto sprite_find = std::find_if(begin(sprites_), end(sprites_),
    [id](auto sprite) -> bool
    {
      return boost::apply_visitor(Find_Visitor{sprite}, id);
    });
    if(sprite_find == end(sprites_))
    {
      throw Unknown_Sprite_Error{id};
    }

    return *sprite_find;
  }
}
