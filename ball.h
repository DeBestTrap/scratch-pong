#ifndef ball_h_
#define ball_h_

class Ball {
public:
  Ball(int max_width, int max_height) {
    // max_w_ and max_h_ are the max width and length of the screen.
    max_w_ = max_width;
    max_h_ = max_height;
    x_ = max_width/2;
    y_ = max_height/2;
    r_ = 2; // controlls the size of the ball
    velocity_ = 1;
    theta_ = 45;
  }
  void set_theta(int theta) { theta_ = theta; }
  void randomize_theta();
  int get_x() {return x_;}
  int get_y() {return y_;}
  int get_r() {return r_;}
  int set_x(double x) {x_ = x;}
  int set_y(double y) {y_ = y;}
  double update_pos(int x_1, int x_2, int y);
private:
  double x_;        // (x,y) is the top left corner of the paddle.
  double y_;
  int r_;           // Radius of the ball.
  int theta_;       // Angle at which the ball travels.
  double velocity_; // In pixels per update
  int max_w_;       // The max width and height of the screen.
  int max_h_;
};

#endif