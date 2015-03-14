/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Player_Health_Mediator.h"
namespace game
{
  Player_Health_Mediator::
  Player_Health_Mediator(Entity_Data* ed, ui::Bar* b) noexcept
                         : entity_(ed), bar_(b),
                           anim(1, 1, Anim_Repeat_Mode::No_Repeat)
  {
    // Reset the bar.
    reset_bar();

    anim.set_segment_fn([this](int seg)
    {
      // The segment is the distance traveled from our cached current health
      // to our actual current health.
      int cur;
      if(cached_cur_health_ < entity_->cur_life)
      {
        cur = cached_cur_health_ + seg;
      }
      if(entity_->cur_life < cached_cur_health_)
      {
        cur = cached_cur_health_ - seg;
      }

      bar_->cur(cur);
    });
  }
  void Player_Health_Mediator::step() noexcept
  {
    if(!entity_ || !bar_) return;

    if(state_ == detail::Mediator_State::None)
    {
      // If our cache matches the entity, ie nothing has changed: return.
      if(cached_cur_health_ == entity_->cur_life)
      {
        return;
      }
      else
      {
        // We need to start animating!
        anim.reset(1, std::abs(cached_cur_health_ - entity_->cur_life),
                   Anim_Repeat_Mode::No_Repeat);

        state_ = detail::Mediator_State::Anim;
      }
    }
    else if(state_ == detail::Mediator_State::Anim)
    {
      // If our animation is done.
      if(anim.done())
      {
        // Control flows here if we are done animating the bar.
        // Set the bar.
        bar_->cur(entity_->cur_life);
        // We're not animating anymore.
        state_ = detail::Mediator_State::None;
        // Everything is up to date.
        cached_cur_health_ = entity_->cur_life;
      }
      else
      {
        // Keep going otherwise
        anim.step();
      }
    }
  }

  Entity_Data* Player_Health_Mediator::entity_data() const noexcept
  {
    return entity_;
  }
  void Player_Health_Mediator::entity_data(Entity_Data* ed) noexcept
  {
    entity_ = ed;
    reset_bar();
  }

  ui::Bar* Player_Health_Mediator::bar() const noexcept
  {
    return bar_;
  }
  void Player_Health_Mediator::bar(ui::Bar* bar) noexcept
  {
    bar_ = bar;
    reset_bar();
  }

  void Player_Health_Mediator::reset_bar() noexcept
  {
    // If we don't have a valid bar or anything don't do anything.
    if(!bar_ || !entity_) return;

    cached_cur_health_ = 0;
    state_ = detail::Mediator_State::None;

    bar_->cur(cached_cur_health_);
    bar_->max(entity_->max_life);
  }
}
