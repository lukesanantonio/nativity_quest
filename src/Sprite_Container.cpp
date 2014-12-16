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
  Sprite_Impl::Sprite_Impl(SDL_Surface* srf) noexcept
                           : id{0}, str{""}
  {
    init_cache();
    reset(srf);
  }
  Sprite_Impl::Sprite_Impl(int id,
                           std::string const& str,
                           std::string const& sprite_filename) noexcept
                           : id(id), str(str)
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
  Sprite_Container::Sprite_Container(std::string const& filename)
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
        if(!has_json_members(*iter, {"id", "str"}))
        {
          throw Bad_Asset{filename, "Element " + std::to_string(elem) +
                          " is an invalid " +
                          "asset object"};
        }

        // Get the common members.
        auto id = int{(*iter)["id"].GetInt()};
        auto str = std::string{(*iter)["str"].GetString()};

        if(has_json_members(*iter, {"file"}))
        {
          auto f = std::string{(*iter)["file"].GetString()};

          // Add this sprite!
          sprites_.push_back({std::make_shared<Sprite_Impl>(id, str, f)});
        }
        else if(has_json_members(*iter, {"animation"}))
        {
          auto anim_sprites = std::vector<Sprite>{};

          auto& anim_doc = (*iter)["animation"];
          for(auto anim_iter = anim_doc.Begin();
              anim_iter != anim_doc.End();
              ++anim_iter)
          {
            auto f = std::string{anim_iter->GetString()};
            anim_sprites.emplace_back(std::make_shared<Sprite_Impl>(id,str,f));

            if(!anim_sprites.back()->surface())
            {
              throw Bad_Asset{filename, "Failed to load '" + str + "'"};
            }
          }

          sprites_.emplace_back(std::move(anim_sprites));
        }
        else
        {
          throw Bad_Asset{filename, "Element " + std::to_string(elem) +
                                    " is neither a sprite or an animation"};
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

  Sprite Sprite_Container::get_sprite(sprite_id id, int frame) const noexcept
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
    auto sprite_vec_find = std::find_if(begin(sprites_), end(sprites_),
    [id](auto const& sprite_vec) -> bool
    {
      if(sprite_vec.empty()) return false;

      return boost::apply_visitor(Find_Visitor{sprite_vec[0]}, id);
    });
    if(sprite_vec_find == end(sprites_))
    {
      throw Unknown_Sprite_Error{id};
    }

    return (*sprite_vec_find)[frame];
  }
}
