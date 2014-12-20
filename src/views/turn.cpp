/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "turn.h"

#include "../State.h"
#include "../effects.h"
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

  Turn_State change_player(State& state, Turn_Data& turn) noexcept
  {
    auto& player = turn.map->players[next_player(turn)];
    player.moved = 0;
    player.done = false;
    player.combat_defense = 0;
    player.flare = false;
    player.turns_of_haste = std::max(0, player.turns_of_haste - 1);

    // Make each enemy closer to being active
    for(auto& enemy : turn.map->enemies)
    {
      enemy.not_fighting = std::max(0, enemy.not_fighting - 1);
    }

    auto pt = player.pos;

    auto vp_src = view_pt(state.window_extents, turn.map->extents,
                          Vec<int>{pt}, turn.map->scale);

    auto delta = vp_src.pos - turn.map_corner;
    return Change_View_Data{delta};
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
    zone_label.mode(pong::Label_Mode::Bitmap);

    for(auto& player : map->players)
    {
      reset_fog(player, map->zones.get_map_extents());
      player.moved = 0;
      player.max_movement = 100;
      player.entity_data.max_life = 10;
      player.item_parser = &items;
      reset_life(player.entity_data);
    }

    // Give each player their locked items.
    map->players[0].inventory[5] = items.get_item("Wool");
    map->players[1].inventory[5] = items.get_item("Starlight");
    map->players[2].inventory[5] = items.get_item("Gold");
    map->players[3].inventory[5] = items.get_item("Gemstones");
    map->players[4].inventory[5] = items.get_item("Myrrh");
    map->players[5].inventory[5] = items.get_item("Incense");

    map->players[0].spawn_pt = {418, 20};
    map->players[1].spawn_pt = {616, 142};
    map->players[2].spawn_pt = {728, 328};
    map->players[3].spawn_pt = {833, 638};
    map->players[4].spawn_pt = {337, 648};
    map->players[5].spawn_pt = {76, 499};

    for(auto& player : map->players)
    {
      player.pos = player.spawn_pt;
      unfog(player);
    }

    auto& enemies = map->enemies;
    auto& enemy_decl = map->enemy_decl;
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("soldier")};
      enemy_instance.pos = {260, 84};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("giant")};
      enemy_instance.pos = {591, 13};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("wolf")};
      enemy_instance.pos = {688, 72};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("creeper")};
      enemy_instance.pos = {867, 47};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("soldier")};
      enemy_instance.pos = {815, 208};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("creeper")};
      enemy_instance.pos = {575, 275};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("giant")};
      enemy_instance.pos = {785, 565};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("soldier")};
      enemy_instance.pos = {565, 636};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("giant")};
      enemy_instance.pos = {219, 542};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("creeper")};
      enemy_instance.pos = {55, 639};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("wolf")};
      enemy_instance.pos = {263, 355};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("myrr")};
      enemy_instance.pos = {322, 359};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("creeper")};
      enemy_instance.pos = {359, 410};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("wolf")};
      enemy_instance.pos = {461, 351};
      enemies.push_back(enemy_instance);
    }
    {
      auto enemy_instance = Enemy_Instance{enemy_decl.get_enemy("myrr")};
      enemy_instance.pos = {274, 195};
      enemies.push_back(enemy_instance);
    }

    map->chests.push_back(Chest{items.get_item("Bow"), true, {148, 18}});
    map->chests.push_back(Chest{items.get_item("Potion of Strength"),
                                true, {274, 195}});
    map->chests.push_back(Chest{items.get_item("Torch"), true, {567, 26}});
    map->chests.push_back(Chest{items.get_item("Boat"), true, {895, 32}});
    map->chests.push_back(Chest{items.get_item("Dagger"), true, {554, 291}});
    map->chests.push_back(Chest{items.get_item("Cloak of Protection"),
                                true, {924, 492}});
    map->chests.push_back(Chest{items.get_item("Ring of Protection"),
                                true, {612, 516}});
    map->chests.push_back(Chest{items.get_item("Canteen"), true, {558, 566}});
    map->chests.push_back(Chest{items.get_item("Throwing Axe"),
                                true, {236, 518}});
    map->chests.push_back(Chest{items.get_item("Rope"), true, {19, 674}});
    map->chests.push_back(Chest{items.get_item("Longsword"),
                                true, {416, 349}});
    map->chests.push_back(Chest{items.get_item("Potion of Defense"),
                                true, {886, 200}});
    map->chests.push_back(Chest{items.get_item("Potion of Haste"),
                                true, {886, 219}});
    map->chests.push_back(Chest{items.get_item("Flare"),
                                true, {113, 329}});

    map->scale = 4.0;
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

      Turn_State operator()(Inventory_View_Data& data) const noexcept;

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
          invent.label_view.control().health_player =
                                               &turn.map->players[turn.player];
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
        if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
        {
          turn.map->players[turn.player].done = true;
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

      auto selected = data.label_view.control().selected;
      auto selected_item = turn.map->players[turn.player].inventory[selected];
      if(data.label_view.control().done &&
         !is_locked_item(turn.items, selected_item))
      {
        auto selected = data.label_view.control().selected;
        if(selected < 6)
        {
          auto& item = turn.map->players[turn.player].inventory[selected];
          item = data.label_view.control().new_item;
        }
        return data.after_state;
      }
      data.label_view.control().done = false;
      return data;
    }
    Turn_State
    Event_Visitor::operator()(Inventory_View_Data& data) const noexcept
    {
      data.label_view.handle_event(event);

      if(data.label_view.control().enter)
      {
        auto selected = data.label_view.control().selected;
        auto& player = turn.map->players[turn.player];
        auto item = player.inventory[selected];
        if(can_be_used(turn.items, item))
        {
          player.item_parser = &turn.items;
          apply_effect(player, item);

          player.inventory[selected] = no::item;
          data.label_view.labels()[selected].str("No item");
        }

        data.label_view.control().enter = false;
      }

      if(event.type == SDL_KEYDOWN)
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
        auto num = std::rand();

        // Fucking hell std::rand don't make random numbers?!
        // We need to improvise.
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);
        num = num ^ x ^ y ^ std::clock();
        num %= 4;
        if(num == 0)
        {
          data.label_view.control().enemy.not_fighting = 18;
          return data.after_state;
        }
        else
        {
          data.label_view.control().state = Fight_State::Enemy_Turn;
        }
      }
      if(data.label_view.control().player.flare)
      {
        data.label_view.control().enemy.not_fighting = 12;

        auto flare = turn.items.get_item("Flare");

        // Remove the flare!
        auto& invent = data.label_view.control().player.inventory;
        using std::begin; using std::end;
        auto flare_find = std::find_if(begin(invent), end(invent),
        [&flare](Item item)
        {
          return item == flare;
        });
        if(flare_find != end(invent))
        {
          *flare_find = no::item;
        }

        return Flare_Data{};
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
      Turn_State operator()(Flare_Data& data) const noexcept;

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
          return enemy.entity_data.cur_life == 0;
        });

        turn.map->enemies.erase(new_end, end(turn.map->enemies));
      }

      // Respawn ourselves if necessary.
      if(player.entity_data.cur_life == 0)
      {
        player.pos = player.spawn_pt;
        reset_life(player.entity_data);
        return change_player(state, turn);
      }

      // Check for any enemies in our view
      {
        auto enemy_find = std::find_if(begin(turn.map->enemies),
                                       end(turn.map->enemies),
        [&player](auto const& enemy)
        {
          if(enemy.not_fighting) return false;

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
          combat.label_view.add_label("Use item");
          combat.label_view.add_label("Run");

          for(auto& label : combat.label_view.labels())
          {
            label.text_height(35);
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

      // Can it be? Have we reached Bethlehem?
      if(turn.map->zones.get_zone(player.pos)->important)
      {
        // This player just won the game!
        auto win = Winning_Data{};
        win.label.str("You have won the game!");
        win.label.text_height(50);
        win.label.color({0x00, 0x00, 0x00, 0xff});
        return win;
      }

      // Now that we dealt with everything, can we switch players?
      if(player.done)
      {
        return change_player(state, turn);
      }

      return data;
    }
    Turn_State Step_Visitor::operator()(Moving_Data& data) const noexcept
    {
      // Player movement!

      auto& player = turn.map->players[turn.player];

      // Find out the distance we have yet to travel.
      auto delta_len = length(data.delta);

      // Max step size for the player.
      auto max_speed = 1.0;

      auto max_movement = get_max_movement(player) +
                          (player.turns_of_haste != 0 ? 100.0 : 0.0);

      // Isolate the direction.
      auto unit_delta = normalize<>(data.delta);
      // How much do we move this step?
      auto move_length =
        std::min(max_speed,
                 std::min(max_movement - player.moved, delta_len));
      auto move_delta = unit_delta * move_length;

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

      player.item_parser = &turn.items;
      unfog(player);

      // Mark some distance traveled.
      data.delta -= move_delta;
      player.moved += move_length;

      // If we have less than the max units per step, it means we just
      // completed that and we can become static.
      if(delta_len < max_speed || max_movement <= player.moved)
      {
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

          turn.map->players[turn.player].item_parser = &turn.items;
          unfog(turn.map->players[turn.player]);
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
        update_zone(turn);
        return Waiting_Data{};
      }
      return data;
    }
    Turn_State Step_Visitor::operator()(Flare_Data& data) const noexcept
    {
      if(--data.steps == 0)
      {
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

    Vec<double> moving_delta(Turn_State const& data) noexcept
    {
      return boost::apply_visitor(Delta_Visitor{}, data);
    }

  }

  void render_state(State& s, Graphics_Desc& g, Sprite_Container& sprites,
                    Turn_Data& turn) noexcept
  {
    Player& player = turn.state.which() != 6 ? turn.map->players[turn.player] :
                     turn.map->players[next_player(turn)];

    // Recalculate the map corner.
    const auto viewport_width = int(g.get_width() / turn.map->scale);
    const auto viewport_height = int(g.get_height() / turn.map->scale);

    auto map_sprite = sprites.get_sprite(turn.map->zones.map_asset());

    auto viewport_src =
      view_pt({g.get_width(), g.get_height()},
              {map_sprite->surface()->w, map_sprite->surface()->h},
              player.pos, turn.map->scale);

    // Only modify these if we are not currently animating the change between
    // players.
    if(turn.state.which() != 6)
    {
      // These are our calculated corners!
      turn.map_corner = viewport_src.pos;
    }

    viewport_src.pos = turn.map_corner;

    // Render the full map.
    auto viewport_src_rect = to_rect(viewport_src);
    SDL_RenderCopy(g.renderer, map_sprite->texture(g.renderer),
                   &viewport_src_rect, NULL);

    auto additional_sprite = sprites.get_sprite("maplayer");
    SDL_RenderCopy(g.renderer, additional_sprite->texture(g.renderer),
                   &viewport_src_rect, NULL);

    // Figure out where the player will be on-screen.
    auto player_scr_coord = (player.pos - turn.map_corner) * turn.map->scale;

    // Render the path of the player (if applicable).
    auto move = moving_delta(turn.state);

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

      if(enemy.not_fighting == 0)
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
      SDL_RenderCopy(g.renderer, player.fog.texture(g.renderer),
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
    else if(turn.state.which() == 7)
    {
      auto& win = boost::get<Winning_Data>(turn.state);

      win.label.position(
        {g.get_width() / 2 - win.label.surface_extents(g).x / 2,
         g.get_height() / 2 - win.label.surface_extents(g).y / 2});

      SDL_Rect rect;
      rect.x = win.label.position().y;
      rect.y = win.label.position().y;

      auto label_extents = win.label.surface_extents(g);
      rect.w = label_extents.x;
      rect.h = label_extents.y;
      SDL_SetRenderDrawColor(g.renderer, 0xff, 0xff, 0xff, 0xff);
      SDL_RenderFillRect(g.renderer, &rect);

      win.label.render(g);
    }
    else if(turn.state.which() == 8)
    {
      SDL_SetRenderDrawColor(g.renderer, 0xff, 0xff, 0xff, 0xff);
      SDL_RenderFillRect(g.renderer, NULL);
    }
  }
}
