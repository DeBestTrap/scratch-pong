#include "paddle.h"

void Paddle::move_up() {
  // Move the paddle up, if the next pos is
  // outside the bounds of the screen, move it back down.
  if (--y_ < 0) { ++y_; }
}

void Paddle::move_down() {
  // Move the paddle down, if the next pos is
  // outside the bounds of the screen, move it back up.
  if (++y_+height_ > max_h_) { --y_; }
}