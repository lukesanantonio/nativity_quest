/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "animation.h"
namespace game
{
  Animation::Animation(int segment_size, int segments,
                       segment_fn_t segment_fn,
                       substep_fn_t substep_fn) noexcept
                       : segment_size_(segment_size),
                         segments_(segments + 1),
                         segment_fn_(segment_fn),
                         substep_fn_(substep_fn) {}

  void Animation::step() noexcept
  {
    int segment = 0;
    if(cur_ != 0) segment = cur_ / segment_size_;

    int substep = 0;
    if(cur_ != 0) substep = cur_ % segment_size_;

    // When our substep is 0 it means we just started a new segment.
    if(substep == 0)
    {
      if(segment_fn_) segment_fn_(segment);
    }
    // When it is 1 or above it means it is our first and subsequent steps into
    // the segment respectively.
    else if(substep != 0)
    {
      if(substep_fn_) substep_fn_(substep - 1);
    }

    ++cur_;
  }
  bool Animation::done() const noexcept
  {
    return cur_ == segments_ * segment_size_;
  }
}
