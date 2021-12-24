#include "ball.h"
#include "math.h"

void Ball::randomize_theta() {
  // Randomizes the angle of the ball.
  theta_ = 0;
  while (theta_%90 == 0) {
    theta_ = random(12)*45;
  }
}

double Ball::update_pos(int x_1, int x_2, int y) {
  // Calculate the next position of the ball based on
  // its velocity and angle and given a min x, max x, and min y,
  // determine if the ball should bounce (change angle).
  double radian_theta = (theta_*PI)/180;
  double new_x = x_ + (velocity_*cos(radian_theta));
  double new_y = y_ - (velocity_*sin(radian_theta));
  if (new_y <= y+r_ || new_y >= max_h_-r_) { theta_ = 360 - theta_; }
  if (new_x <= x_1+r_ || new_x >= x_2-r_) { theta_ = 180 - theta_; }
  y_ = new_y;
  x_ = new_x;
  if (theta_ < 0) { theta_ = 360 + theta_; }
  theta_ = theta_%360;
  return theta_;
}