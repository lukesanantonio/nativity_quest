/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "heros.h"
namespace game
{
  struct Hero_Decl
  {
    char const* const name;
    short life;
    short power;
    Item_Type item;
    short visual_id;
  };
  constexpr Hero_Decl SUSANNA = {"Susanna", 6, 5, Item_Type::Wool, 0};
  constexpr Hero_Decl RAFAELA = {"Rafaela", 6, 6, Item_Type::Starlight, 1};
  constexpr Hero_Decl MELCHIOR = {"Melchior", 6, 3, Item_Type::Gold, 2};
  constexpr Hero_Decl DEBORAH = {"Deborah", 7, 2, Item_Type::Gemstones, 3};
  constexpr Hero_Decl CASPER = {"Casper", 4, 4, Item_Type::Mirrh, 4};
  constexpr Hero_Decl BALTHAZAR = {"Balthazar", 2, 6, Item_Type::Incense, 5};

  Hero instantiate_hero(std::string const& player_name, Hero_Id id) noexcept
  {
    switch(id)
    {
      case Hero_Id::Susanna:
      {
        return Hero{player_name, SUSANNA.name, SUSANNA.power,
                    SUSANNA.life, {}, SUSANNA.item, SUSANNA.visual_id};
        break;
      }
      case Hero_Id::Rafaela:
      {
        return Hero{player_name, RAFAELA.name, RAFAELA.power,
                    RAFAELA.life, {}, RAFAELA.item, RAFAELA.visual_id};
        break;
      }
      case Hero_Id::Melchior:
      {
        return Hero{player_name, MELCHIOR.name, MELCHIOR.power,
                    MELCHIOR.life, {}, MELCHIOR.item, MELCHIOR.visual_id};
        break;
      }
      case Hero_Id::Deborah:
      {
        return Hero{player_name, DEBORAH.name, DEBORAH.power,
                    DEBORAH.life, {}, DEBORAH.item, DEBORAH.visual_id};
        break;
      }
      case Hero_Id::Casper:
      {
        return Hero{player_name, CASPER.name, CASPER.power,
                    CASPER.life, {}, CASPER.item, CASPER.visual_id};
        break;
      }
      case Hero_Id::Balthazar:
      {
        return Hero{player_name, BALTHAZAR.name, BALTHAZAR.power,
                    BALTHAZAR.life, {}, BALTHAZAR.item, BALTHAZAR.visual_id};
        break;
      }
    }
  }
}
