/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Entity_Health_Mediator.h"
namespace game
{
  void set_bar_to_life(ui::Bar& bar, Entity_Data& et) noexcept
  {
    bar.max(et.max_life);
    bar.cur(et.cur_life);
  }
  void set_bar_animating(ui::Bar& bar, Entity_Data& et, int orig_health,
                         int step, int max_step, int res) noexcept
  {
    // We know where we are and we know where we need to be. Use the
    // progress step / max_step to update the bar, basically.

    // Find the ratio we have.
    auto cur_ratio = (double) orig_health / et.max_life;

    // Find the ratio we want.
    auto next_ratio = (double) et.cur_life / et.max_life;

    // The difference.
    auto delta_ratio = next_ratio - cur_ratio;

    bar.max(res);

    // Start from the current point.
    // Then add a certain amount of the delta ratio based on step / max_step.
    bar.cur((res * cur_ratio) + (res * delta_ratio * step / max_step));
  }

  Entity_Health_Mediator::
  Entity_Health_Mediator(Entity_Data* ed, ui::Bar* b) noexcept
                         : entity_(ed), bar_(b),
                           anim(1, 1, Anim_Repeat_Mode::No_Repeat)
  {
    // Reset the bar.
    reset_bar();

    anim.set_segment_fn([this](int seg)
    {
      set_bar_animating(*bar_, *entity_, cached_cur_health_,
                        seg, anim.segments() - 1, 300);
    });
  }
  void Entity_Health_Mediator::step() noexcept
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
        anim.reset(1, 45, Anim_Repeat_Mode::No_Repeat);
        state_ = detail::Mediator_State::Anim;
      }
    }
    else if(state_ == detail::Mediator_State::Anim)
    {
      // If our animation is done.
      if(anim.done())
      {
        // Control flows here if we are done animating the bar.
        // We're not animating anymore.
        state_ = detail::Mediator_State::None;

        // Everything is up to date.
        cached_cur_health_ = entity_->cur_life;

        // Set the bar.
        set_bar_to_life(*bar_, *entity_);
      }
      else
      {
        // Keep going otherwise
        anim.step();
      }
    }
  }

  Entity_Data* Entity_Health_Mediator::entity_data() const noexcept
  {
    return entity_;
  }
  void Entity_Health_Mediator::entity_data(Entity_Data* ed) noexcept
  {
    entity_ = ed;
    reset_bar();
  }

  ui::Bar* Entity_Health_Mediator::bar() const noexcept
  {
    return bar_;
  }
  void Entity_Health_Mediator::bar(ui::Bar* bar) noexcept
  {
    bar_ = bar;
    reset_bar();
  }

  void Entity_Health_Mediator::reset_bar() noexcept
  {
    // If we don't have a valid bar or anything don't do anything.
    if(!bar_ || !entity_) return;

    cached_cur_health_ = 0;
    state_ = detail::Mediator_State::None;

    bar_->cur(cached_cur_health_);
    bar_->max(entity_->max_life);
  }
}
