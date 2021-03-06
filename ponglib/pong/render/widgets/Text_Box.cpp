/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Text_Box.h"
#include "../utility.h"
namespace pong
{
  Text_Box::Text_Box(Volume const& vol, int blink_rate,
                     text::Face* face, text::Rasterizer* rasterizer) noexcept
                     : cur_pos_(0), blink_rate_(blink_rate)
  {
    this->label_.rasterizer(rasterizer);
    this->label_.font_face(face);

    this->label_.mode(Label_Mode::Baseline);

    this->volume(vol);
  }
  void Text_Box::render(SDL_Renderer* r) const noexcept
  {
    math::vector<double> label_pos;
    label_pos.x = vol_.pos.x + 10;
    label_pos.y = vol_.pos.y + (vol_.height * .8);
    this->label_.position(label_pos);
    this->label_.render(r);

    SDL_SetRenderDrawColor(r, 0xff, 0xff, 0xff, 0xff);
    render_wireframe(r, vol_);
  }
  void Text_Box::handle_event(const SDL_Event& e) noexcept
  {
    if(e.type == SDL_TEXTINPUT)
    {
      std::string input(e.text.text);
      this->label_.data(this->label_.data() + input);

      // Get the width of this text.
    }
    if(e.type == SDL_KEYDOWN)
    {
      if(e.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
      {
        std::string text = this->label_.data();
        text.pop_back();
        this->label_.data(text);
      }
      if(e.key.keysym.scancode == SDL_SCANCODE_RETURN)
      {
        std::string text = this->label_.data();
        text.push_back('\n');
        this->label_.data(text);
      }
    }
  }
}
