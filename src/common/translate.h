
/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <type_traits>
#include "../assets/desc/lang.h"
namespace game
{
  void set_lang(assets::Lang const& lang) noexcept;
  assets::Lang get_lang() noexcept;

  namespace
  {
    template <int N, typename... Params>
    struct pack_element
    {
      using type = typename std::tuple_element<N, std::tuple<Params...> >::type;
    };

    template <int N, typename... Params>
    using pack_element_t = typename pack_element<N, Params...>::type;

    std::string to_string_helper(std::string str) noexcept { return str; }

    std::string to_string_helper(char const* const str) noexcept
    { return str; }

    template <class T>
    std::string to_string_helper(T&& v) noexcept { return std::to_string(v); }

    std::string format(std::string s, int pos, std::string i) noexcept
    {
      // If we have no more to replace just append the original string to
      // whatever we have. This will leave extra percent signs if there are
      // less arguments than percent signs but that's intentional.
      return i + s.substr(pos);
    }

    template <class First, class... Args>
    std::string format(std::string str, int pos, std::string i,
                       First&& first, Args&&... args) noexcept
    {
      // If our position is past the end of the string, just return the
      // intermediate string since it should have been completed. The reason
      // we have to handle it here is because the user may provide extra
      // arguments making the template above this one never be instantiated.
      if(static_cast<int>(str.size()) <= pos) return format(str, pos, i);

      auto c = str[pos];
      if(c == '%')
      {
        // Append the first argument as a string instead of the percent sign.
        i += to_string_helper(std::forward<First>(first));

        // Format further without the 'first' argument since it has been
        // used up.
        return format(str, pos + 1, i, std::forward<Args>(args)...);
      }
      else
      {
        // Pass in the original string, the new intermediate, and the
        // arguments as is, since we haven't used them up yet.
        return format(str, pos + 1, i + c,
                      std::forward<First>(first), std::forward<Args>(args)...);
      }
    }
  }

  template <class... Args>
  std::string translate(std::string str, Args&&... args) noexcept
  {
    return format(get_lang().dict.at(str), 0, "", std::forward<Args>(args)...);
  }

  template <class... Args>
  std::string t(std::string str, Args&&... args) noexcept
  {
    return translate(str, std::forward<Args>(args)...);
  }
}
