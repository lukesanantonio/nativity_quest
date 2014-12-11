/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "turn.h"

#include "../render.h"

#include "../util/pi.h"

#include <cmath>

namespace game
{
  namespace
  {
    Zone update_zone(Turn_Data& turn) noexcept
    {
      auto& active_player = turn.map->players[turn.player];

      auto zone = turn.map->zones.get_zone(Vec<int>{active_player.pos});
      turn.zone_label.data(zone ? zone->str : "Unknown");

      return zone;
    }

    void next_player(Turn_Data& turn) noexcept
    {
      if(++turn.player == turn.map->players.size()) turn.player = 0;
    }
  }

  Turn_Data::Turn_Data(std::string const& items_file,
                       std::string const& zones_file,
                       std::string const& char_file)
                       : items(items_file),
                         map{std::make_shared<Map>(zones_file, items)},
                         player(0), state(Waiting_Data{}), map_corner{},
                         character{char_file}
  {
    zone_label.text_height(35);
    zone_label.text_color({0x88, 0x88, 0x88, 0xff});

    for(auto& player : map->players)
    {
      reset_fog(player, map->zones.get_map_extents());
      player.view_radius = 75.0;
    }

    map->players[player].pos = {500, 500};
    unfog(map->players[player]);

    map->scale = 3.5;
    map->mini_scale = .25;

    update_zone(*this);
  }

  namespace
  {
    struct Event_Visitor : boost::static_visitor<Turn_State>
    {
      inline Event_Visitor(Turn_Data& turn, SDL_Event const& event) noexcept
                          : turn(turn), event(event) {}

      Turn_State operator()(Waiting_Data const& data) const noexcept;

      template <typename Data_T>
      Turn_State operator()(Data_T const& d) const noexcept;

      Turn_Data& turn;
      SDL_Event const& event;
    };

    Turn_State
    Event_Visitor::operator()(Waiting_Data const& data) const noexcept
    {
      if(event.type == SDL_MOUSEBUTTONDOWN)
      {
        if(event.button.button == SDL_BUTTON_LEFT)
        {
          Moving_Data md;

          // Get the mouse in screen coordinates.
          auto mouse = Vec<int>{event.button.x, event.button.y};

          // Use the map scale and current top-left corner to calculate the
          // map coordinates.
          auto map_coord = (mouse / turn.map->scale) + turn.map_corner;

          // Calculate our delta movement.
          md.delta = map_coord - turn.map->players[turn.player].pos;

          return md;
        }
      }
      return data;
    }
    template <typename Data_T>
    Turn_State Event_Visitor::operator()(Data_T const& d) const noexcept
    {
      return d;
    }
  }

  void handle_event_state(Turn_Data& turn, SDL_Event const& event) noexcept
  {
    turn.state = boost::apply_visitor(Event_Visitor{turn, event},
                                      turn.state);
  }

  namespace
  {
    template <typename C>
    auto find_inventory_space(C& container) noexcept -> decltype(auto)
    {
      using std::begin; using std::end;
      auto no_item_find = std::find_if(begin(container), end(container),
      [](auto const& i)
      {
        return i == no::item;
      });

      return no_item_find;
    }

    struct Step_Visitor : boost::static_visitor<Turn_State>
    {
      Step_Visitor(Turn_Data& td) noexcept : turn(td) {}

      Turn_State operator()(Waiting_Data& data) const noexcept;
      Turn_State operator()(Moving_Data& data) const noexcept;
      Turn_State operator()(Uncrate_Data& data) const noexcept;

      Turn_Data& turn;
    };

    Turn_State Step_Visitor::operator()(Waiting_Data& data) const noexcept
    {
      auto& player = turn.map->players[turn.player];
      while(true)
      {
        using std::begin; using std::end;
        auto chest_find = std::find_if(begin(turn.map->chests),
                                       end(turn.map->chests),
        [&player](auto const& chest)
        {
          // If the chest isn't visible, don't allow it to be opened/found.
          if(!chest.active) return false;

          // Check if it's a suitable distance.
          auto len = length(player.pos - Vec<double>{chest.pos});
          return len < player.view_radius;
        });

        if(chest_find == end(turn.map->chests))
        {
          break;
        }

        return Uncrate_Data{*chest_find, Waiting_Data{}};
      }

      return data;
    }
    Turn_State Step_Visitor::operator()(Moving_Data& data) const noexcept
    {
      // Player movement!

      auto& player = turn.map->players[turn.player];

      // Find out the distance we have yet to travel.
      auto delta_len = length(data.delta);

      auto max_len = 1.0;

      // Isolate the direction.
      auto unit_delta = normalize<>(data.delta);
      // How much do we move this step?
      auto move_delta = unit_delta * std::min(max_len, delta_len);

      // Move the player.
      player.pos += move_delta;

      unfog(player);
      update_zone(turn);

      // Mark some distance traveled.
      data.delta -= move_delta;

      // If we have less than the max units per step, it means we just
      // completed that and we can become static.
      if(delta_len < max_len)
      {
        //turn.next_player();
        return Waiting_Data{};
      }

      return data;
    }
    Turn_State Step_Visitor::operator()(Uncrate_Data& data) const noexcept
    {
      // TODO Get these constants from json or something.
      constexpr auto max_frame = 4;
      constexpr auto frames_between = 45;
      constexpr auto frames_end = 90;

      if(data.anim_frame == max_frame)
      {
        if(++data.intermediate_counter == frames_end)
        {
          data.chest.active = false;

          Player& player = turn.map->players[turn.player];
          auto inventory = find_inventory_space(player.inventory);
          if(inventory != std::end(player.inventory))
          {
            // Inventory not full!
            *inventory = data.chest.item;
          }

          return data.after_state;
        }
      }
      else if(++data.intermediate_counter == frames_between)
      {
        ++data.anim_frame;
        data.intermediate_counter = 0;
      }
      else
      {
        ++data.intermediate_counter;
      }
      return data;
    }
  }

  void step_state(Turn_Data& turn_data) noexcept
  {
    turn_data.state = boost::apply_visitor(Step_Visitor{turn_data},
                                           turn_data.state);
  }

  namespace
  {
    struct Delta_Visitor : boost::static_visitor<Vec<double> >
    {
      template <typename T>
      Vec<double> operator()(T const&) const noexcept
      {
        return {0.0, 0.0};
      }

      Vec<double> operator()(Moving_Data const& data) const noexcept
      {
        return data.delta;
      }
    };
  }

  Vec<double> moving_delta(Turn_State const& data) noexcept
  {
    return boost::apply_visitor(Delta_Visitor{}, data);
  }

  void render_state(Graphics_Desc& g, Sprite_Container& sprites,
                    Turn_Data& turn) noexcept
  {
    Player& player = turn.map->players[turn.player];

    // Recalculate the map corner.
    const auto viewport_width = int(g.get_width() / turn.map->scale);
    const auto viewport_height = int(g.get_height() / turn.map->scale);

    auto map_sprite = sprites.get_sprite(turn.map->zones.map_asset());

    SDL_Rect viewport_src;

    // Find the coordinates of the top-left corner of a viewport that results
    // in the player being centered on it.
    // However, don't allow the corner to go behind (0,0).
    viewport_src.x = std::max(0.0, player.pos.x - viewport_width / 2);
    viewport_src.y = std::max(0.0, player.pos.y - viewport_height / 2);

    // The width and height of the viewport are always no greater than the
    // calculated values above (using the map scale and screen size). They will
    // be smaller when the max width would result in a src rectangle that went
    // beyond the map image's bounds.
    viewport_src.w =
           std::min(viewport_width, map_sprite->surface()->w - viewport_src.x);
    viewport_src.h =
          std::min(viewport_height, map_sprite->surface()->h - viewport_src.y);

    // Adjust the viewport corners to give the possibly shortened width and
    // height room to be the max.
    viewport_src.x -= viewport_width - viewport_src.w;
    viewport_src.y -= viewport_width - viewport_src.h;

    // The viewport should always have the calculated width. We know this won't
    // go beyond the map image's bounds because we already adjusted the x and y
    // values accordingly.
    viewport_src.w = viewport_width;
    viewport_src.h = viewport_height;

    // These are our calculated corners!
    turn.map_corner.x = viewport_src.x;
    turn.map_corner.y = viewport_src.y;

    // Render the full map.
    SDL_RenderCopy(g.renderer, map_sprite->texture(g.renderer),
                   &viewport_src, NULL);

    // Figure out where the player will be on-screen.
    auto player_scr_coord = (player.pos - turn.map_corner) * turn.map->scale;

    // Render the path of the player (if applicable).
    auto move = moving_delta(turn.state);
    if(turn.render_active_player_path)
    {

      SDL_SetRenderDrawColor(g.renderer, 0x00, 0x77, 0x00, 0xff);
      SDL_RenderDrawLine(g.renderer, player_scr_coord.x, player_scr_coord.y,
                         player_scr_coord.x + (move.x * turn.map->scale),
                         player_scr_coord.y + (move.y * turn.map->scale));
    }

    // Render the character.

    // Determine the character orientation, then the required rotation in
    // addition to that.

    auto move_unit = normalize(move);
    auto angle = std::atan2(move_unit.y, move_unit.x);

    auto segment_len = (2 * PI) / 4;

    auto orient = short{0};

    if(segment_len + segment_len / 2 >= angle &&
       angle >= segment_len - segment_len / 2)
    {
      orient = 0;
      angle = angle - segment_len;
    }
    else if(segment_len / 2 >= angle && angle >= -segment_len / 2)
    {
      orient = 1;
      // angle = angle
    }
    else if(-segment_len - segment_len / 2 <= angle &&
            angle <= -segment_len + segment_len / 2)
    {
      orient = 2;
      angle = angle - -segment_len;
    }
    else if(angle >= 2 * segment_len - segment_len / 2 ||
            2 * -segment_len + segment_len / 2 >= angle)
    {
      orient = 3;
      if(angle > 0)
        angle = angle - segment_len * 2;
      else
        angle = angle - -segment_len * 2;
    }

    auto char_info = turn.character.get_sprite_info(orient);

    SDL_Rect char_src;
    char_src.x = char_info.src.pos.x;
    char_src.y = char_info.src.pos.y;
    char_src.w = char_info.src.width;
    char_src.h = char_info.src.height;

    SDL_Rect char_dest;
    char_dest.x = player_scr_coord.x;
    char_dest.y = player_scr_coord.y;

    // The center point of the sprite should be the player position.
    char_dest.x -= char_info.center.x * turn.map->scale * char_info.scale;
    char_dest.y -= char_info.center.y * turn.map->scale * char_info.scale;

    // The width is scaled first by the map scale, then by the sprite-specific
    // scale.
    char_dest.w = char_info.src.width * turn.map->scale * char_info.scale;
    char_dest.h = char_info.src.height * turn.map->scale * char_info.scale;

    SDL_Point char_center;
    char_center.x = char_info.center.x * turn.map->scale * char_info.scale;
    char_center.y = char_info.center.y * turn.map->scale * char_info.scale;

    Sprite char_sprite = sprites.get_sprite(char_info.sprite);
    SDL_RenderCopyEx(g.renderer, char_sprite->texture(g.renderer),
                     &char_src, &char_dest,
                     angle / PI * 180, &char_center, SDL_FLIP_NONE);

    // Render any chests.
    auto chest_sprite = sprites.get_sprite("chest");

    for(auto const& chest : turn.map->chests)
    {
      // Only render active chests, others have been opened and shouldn't be
      // shown, TODO make an opened chest sprite.
      if(!chest.active) continue;

      auto chest_extent =
              Vec<int>{chest_sprite->surface()->w, chest_sprite->surface()->h};

      SDL_Rect chest_dest;
      chest_dest.x = chest.pos.x - turn.map_corner.x - chest_extent.x / 2;
      chest_dest.y = chest.pos.y - turn.map_corner.y - chest_extent.y / 2;

      // Scale to the viewport
      chest_dest.x *= turn.map->scale;
      chest_dest.y *= turn.map->scale;

      chest_dest.w = chest_sprite->surface()->w * turn.map->scale;
      chest_dest.h = chest_sprite->surface()->h * turn.map->scale;
      SDL_RenderCopy(g.renderer, chest_sprite->texture(g.renderer),
                     NULL, &chest_dest);
    }

    // Render the fog of war of the current player.
    if(player.fog.surface())
    {
      SDL_RenderCopy(g.renderer, player.fog.texture(g.renderer),
                     &viewport_src, NULL);
    }

    // Render the mini map.
    render_as_minimap(g, sprites, *turn.map, {5,5});

    // Render the zone text.
    turn.zone_label.font_face(&g.font.face);
    turn.zone_label.rasterizer(&g.font.raster);

    auto label_pos = turn.zone_label.position();
    label_pos.x = g.get_width() - turn.zone_label.surface_width() - 10;
    label_pos.y = 10;
    turn.zone_label.position(label_pos);

    turn.zone_label.render(g.renderer);

    // If we are uncrating something currently.
    if(turn.state.which() == 2)
    {
      Uncrate_Data const& uncrate = boost::get<Uncrate_Data>(turn.state);

      // Render the large crate in the center of the screen.
      Sprite chest_spr =
        sprites.get_sprite("chest",
                           uncrate.anim_frame < 4 ? uncrate.anim_frame : 3);

      // Center the large chest.
      SDL_Rect chest_dest;
      chest_dest.w = g.get_width() / 2;
      chest_dest.h = g.get_height() / 2;
      chest_dest.x = g.get_width() / 2 - chest_dest.w / 2;
      chest_dest.y = g.get_height() / 2 - chest_dest.h / 2;
      SDL_RenderCopy(g.renderer, chest_spr->texture(g.renderer),
                     NULL, &chest_dest);

      if(uncrate.anim_frame == 4)
      {
        // Render the item atop everything just about as big.
        Sprite item_spritesheet =
                              sprites.get_sprite(turn.items.get_spritesheet());

        SDL_Rect item_src;
        item_src.w = turn.items.get_sprite_extents().x;
        item_src.h = turn.items.get_sprite_extents().y;

        Item uncovered_item = uncrate.chest.item;
        item_src.x = uncovered_item->pos.x * item_src.w;
        item_src.y = uncovered_item->pos.y * item_src.h;

        SDL_RenderCopy(g.renderer, item_spritesheet->texture(g.renderer),
                       &item_src, &chest_dest);
      }
    }
  }
}
