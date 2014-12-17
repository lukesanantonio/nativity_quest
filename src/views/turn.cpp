/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "turn.h"

#include "../State.h"

#include "../render.h"

#include "../util/pi.h"
#include "../util/rect.h"

#include <cmath>

namespace game
{
  namespace
  {
    Zone update_zone(Turn_Data& turn) noexcept
    {
      auto& active_player = turn.map->players[turn.player];

      auto zone = turn.map->zones.get_zone(Vec<int>{active_player.pos});
      turn.zone_label.str(zone ? zone->str : "Unknown");

      return zone;
    }

    player_id next_player(Turn_Data const& turn) noexcept
    {
      auto cur_player = turn.player;
      if(++cur_player == turn.map->players.size()) return 0;
      return cur_player;
    }
  }

  Turn_Data::Turn_Data(std::string const& items_file,
                       std::string const& zones_file,
                       std::string const& char_file,
                       std::string const& e_file)
                       : items(items_file),
                         map{std::make_shared<Map>(zones_file, items, e_file)},
                         player(0), state(Waiting_Data{}), map_corner{},
                         character{char_file}
  {
    zone_label.text_height(35);
    zone_label.color({0x88, 0x88, 0x88, 0xff});

    for(auto& player : map->players)
    {
      reset_fog(player, map->zones.get_map_extents());
      player.pos = {500, 0};
      player.view_radius = 75.0;
      unfog(player);
    }


    map->scale = 3.5;
    map->mini_scale = .25;

    update_zone(*this);
  }

  namespace
  {
    struct Event_Visitor : boost::static_visitor<Turn_State>
    {
      inline Event_Visitor(State& s, Turn_Data& turn,
                           SDL_Event const& event) noexcept
                           : state(s), turn(turn), event(event) {}

      Turn_State operator()(Waiting_Data const& data) const noexcept;

      Turn_State operator()(Discard_Item_Data& data) const noexcept;

      Turn_State operator()(Inventory_View_Data const& data) const noexcept;

      Turn_State operator()(Combat_Data& data) const noexcept;

      template <typename Data_T>
      Turn_State operator()(Data_T const& d) const noexcept;

      State& state;
      Turn_Data& turn;
      SDL_Event const& event;
    };

    Turn_State
    Event_Visitor::operator()(Waiting_Data const& data) const noexcept
    {
      if(event.type == SDL_KEYDOWN)
      {
        if(event.key.keysym.scancode == SDL_SCANCODE_I)
        {
          auto invent = Inventory_View_Data{};
          invent.after_state = Waiting_Data{};

          for(auto const& item : turn.map->players[turn.player].inventory)
          {
            if(item != no::item) invent.label_view.add_label(item->str);
            else invent.label_view.add_label("No item");
          }

          for(auto& label : invent.label_view.labels())
          {
            label.text_height(25);
            label.color({0x00, 0x00, 0x00, 0x00});
          }

          return invent;
        }
      }
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
    Turn_State
    Event_Visitor::operator()(Discard_Item_Data& data) const noexcept
    {
      data.label_view.handle_event(event);
      if(data.label_view.control().done)
      {
        auto selected = data.label_view.control().selected;
        if(selected < 6)
        {
          auto& item = turn.map->players[turn.player].inventory[selected];
          item = data.label_view.control().new_item;
        }
        return data.after_state;
      }
      return data;
    }
    Turn_State
    Event_Visitor::operator()(Inventory_View_Data const& data) const noexcept
    {
      if(event.type == SDL_KEYUP)
      {
        if(event.key.keysym.scancode == SDL_SCANCODE_I)
        {
          return data.after_state;
        }
      }
      return data;
    }
    Turn_State
    Event_Visitor::operator()(Combat_Data& data) const noexcept
    {
      data.label_view.handle_event(event);
      if(data.label_view.control().state == Fight_State::Running)
      {
        data.label_view.control().enemy.fighting = false;
        return data.after_state;
      }
      return data;
    }
    template <typename Data_T>
    Turn_State Event_Visitor::operator()(Data_T const& d) const noexcept
    {
      return d;
    }
  }

  void handle_event_state(State& s, Turn_Data& turn,
                          SDL_Event const& event) noexcept
  {
    turn.state = boost::apply_visitor(Event_Visitor{s, turn, event},
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
      Step_Visitor(State& s, Turn_Data& td) noexcept : state(s), turn(td) {}

      Turn_State operator()(Waiting_Data& data) const noexcept;
      Turn_State operator()(Moving_Data& data) const noexcept;
      Turn_State operator()(Uncrate_Data& data) const noexcept;
      Turn_State operator()(Combat_Data& data) const noexcept;
      Turn_State operator()(Change_View_Data& data) const noexcept;

      template <typename Data>
      Turn_State operator()(Data const& data) const noexcept;

      State& state;
      Turn_Data& turn;
    };

    Turn_State Step_Visitor::operator()(Waiting_Data& data) const noexcept
    {
      auto& player = turn.map->players[turn.player];

      using std::begin; using std::end;

      // Remove any dead enemies.
      {
        auto new_end = std::remove_if(begin(turn.map->enemies),
                                      end(turn.map->enemies),
        [](auto const& enemy)
        {
          return enemy.current_life == 0;
        });

        turn.map->enemies.erase(new_end, end(turn.map->enemies));
      }

      // Check for any enemies in our view
      {
        auto enemy_find = std::find_if(begin(turn.map->enemies),
                                       end(turn.map->enemies),
        [&player](auto const& enemy)
        {
          if(!enemy.fighting) return false;

          // Check if they are a reasonable distance.
          auto len = length(player.pos - Vec<double>{enemy.pos});
          return len < player.view_radius;
        });

        if(enemy_find != end(turn.map->enemies))
        {
          auto combat = Combat_Data{Waiting_Data{},
            Label_View<Combat_Control>{Volume<int>{},
                                       Combat_Control{player, *enemy_find}}};

          combat.label_view.add_label("Attack");
          combat.label_view.add_label("Run");

          for(auto& label : combat.label_view.labels())
          {
            label.text_height(40);
            label.color({0x00, 0x00, 0x00, 0xff});
          }

          return combat;
        }
      }

      // If we cleared the area of enemies, check for chests.
      {
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

        if(chest_find != end(turn.map->chests))
        {
          return Uncrate_Data{*chest_find, Waiting_Data{}};
        }
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

      auto cur_zone = update_zone(turn);

      // Check to make sure we can be in this zone.
      if(cur_zone->required_item != no::item)
      {
        using std::begin; using std::end;
        auto item_find =
          std::find_if(begin(player.inventory), end(player.inventory),
          [&cur_zone](auto const& item)
          {
            return cur_zone->required_item == item;
          });

        // We don't have the required item.
        if(item_find == end(player.inventory))
        {
          // Cancel the movement.
          player.pos -= move_delta;
          update_zone(turn);
          return Waiting_Data{};
        }
      }

      // We can continue.

      unfog(player);

      // Mark some distance traveled.
      data.delta -= move_delta;

      // If we have less than the max units per step, it means we just
      // completed that and we can become static.
      if(delta_len < max_len)
      {
        auto pt = turn.map->players[next_player(turn)].pos;

        auto vp_src = view_pt(state.window_extents, turn.map->extents,
                              Vec<int>{pt}, turn.map->scale);

        auto delta = vp_src.pos - turn.map_corner;
        return Change_View_Data{delta};
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
          else
          {
            auto& player = turn.map->players[turn.player];
            Discard_Item_Data discard_data{player, turn.items};

            discard_data.label_view.control().new_item = data.chest.item;
            discard_data.after_state = Waiting_Data{};

            for(int i = 0; i < player.inventory.size(); ++i)
            {
              discard_data.label_view.add_label(player.inventory[i]->str);
            }

            discard_data.label_view.add_label("Discard");

            for(auto& label : discard_data.label_view.labels())
            {
              label.text_height(25);
              label.color({0x00, 0x00, 0x00, 0xff});
            }

            return discard_data;
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
    Turn_State Step_Visitor::operator()(Combat_Data& data) const noexcept
    {
      data.label_view.control().step();
      if(data.label_view.control().state == Fight_State::Player_Won ||
         data.label_view.control().state == Fight_State::Enemy_Won)
      {
        return data.after_state;
      }
      return data;
    }
    Turn_State Step_Visitor::operator()(Change_View_Data& data) const noexcept
    {
      constexpr auto max_speed = 6.0;

      auto unit_delta = normalize(data.delta);
      auto move_delta =
                Vec<int>{unit_delta * std::min(length(data.delta), max_speed)};

      turn.map_corner += move_delta;

      auto before_delta = data.delta;
      data.delta -= move_delta;

      if(before_delta == data.delta)
      {
        turn.player = next_player(turn);
        return Waiting_Data{};
      }
      return data;
    }
    template <typename Data>
    Turn_State Step_Visitor::operator()(Data const& data) const noexcept
    {
      return data;
    }
  }

  void step_state(State& s, Turn_Data& turn_data) noexcept
  {
    turn_data.state = boost::apply_visitor(Step_Visitor{s, turn_data},
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

  void render_state(State& s, Graphics_Desc& g, Sprite_Container& sprites,
                    Turn_Data& turn) noexcept
  {
    Player& player = turn.map->players[turn.player];

    // Recalculate the map corner.
    const auto viewport_width = int(g.get_width() / turn.map->scale);
    const auto viewport_height = int(g.get_height() / turn.map->scale);

    auto map_sprite = sprites.get_sprite(turn.map->zones.map_asset());

    auto viewport_src =
      view_pt({g.get_width(), g.get_height()},
              {map_sprite->surface()->w, map_sprite->surface()->h},
              player.pos, turn.map->scale);

    // Render the full map.
    auto viewport_src_rect = to_rect(viewport_src);
    SDL_RenderCopy(g.renderer, map_sprite->texture(g.renderer),
                   &viewport_src_rect, NULL);

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

    for(auto const& enemy : turn.map->enemies)
    {
      auto enemy_extents = Vec<int>{15, 15};

      // Render fighting enemies red and non-fighting enemies yellow.
      SDL_Rect enemy_dest;
      enemy_dest.x = enemy.pos.x - turn.map_corner.x - enemy_extents.x / 2;
      enemy_dest.y = enemy.pos.y - turn.map_corner.y - enemy_extents.y / 2;

      // Scale to the viewport
      enemy_dest.x *= turn.map->scale;
      enemy_dest.y *= turn.map->scale;

      enemy_dest.w = enemy_extents.x;
      enemy_dest.h = enemy_extents.y;

      if(enemy.fighting)
      {
        SDL_SetRenderDrawColor(g.renderer, 0xff, 0x00, 0x00, 0xff);
      }
      else
      {
        SDL_SetRenderDrawColor(g.renderer, 0xff, 0xff, 0x00, 0xff);
      }

      SDL_RenderFillRect(g.renderer, &enemy_dest);

      --enemy_dest.x;
      --enemy_dest.y;
      enemy_dest.w += 2;
      enemy_dest.h += 2;

      SDL_SetRenderDrawColor(g.renderer, 0x00, 0x00, 0x00, 0xff);
      SDL_RenderDrawRect(g.renderer, &enemy_dest);
    }

    // Render the fog of war of the current player.
    if(player.fog.surface())
    {
      SDL_RenderCopy(g.renderer, fog_player->fog.texture(g.renderer),
                     &viewport_src_rect, NULL);
    }

    // Render the mini map.
    render_as_minimap(g, sprites, *turn.map, {5,5});

    if(turn.state.which() != 5)
    {
      // Render the zone text.
      auto label_pos = turn.zone_label.position();
      label_pos.x = g.get_width() - turn.zone_label.surface_extents(g).x - 5;
      label_pos.y = 10;
      turn.zone_label.position(label_pos);

      turn.zone_label.render(g);
    }

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

    else if(turn.state.which() == 3)
    {
      auto& discard = boost::get<Discard_Item_Data>(turn.state);
      discard.label_view.vol({{0, g.get_height() - 100}, g.get_width(), 100});
      discard.label_view.layout(g);

      Volume<int> chest_dest;
      chest_dest.width = g.get_width() / 2;
      chest_dest.height = g.get_height() / 2;
      chest_dest.pos.x = g.get_width() / 2 - chest_dest.width / 2;
      chest_dest.pos.y = g.get_height() / 2 - chest_dest.height / 2;

      discard.label_view.control().item_volume = chest_dest;
      discard.label_view.render(g, sprites);
    }

    else if(turn.state.which() == 4)
    {
      auto& invent = boost::get<Inventory_View_Data>(turn.state);

      invent.label_view.vol({{0, g.get_height() - 100}, g.get_width(), 100});
      invent.label_view.layout(g);
      invent.label_view.render(g, sprites);
    }
    else if(turn.state.which() == 5)
    {
      auto& combat = boost::get<Combat_Data>(turn.state);

      combat.label_view.vol({{300, g.get_height() - 100},
                             g.get_width() - 300, 100});
      combat.label_view.layout(g);
      combat.label_view.render(g, sprites);
    }
  }
}
