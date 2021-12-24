#ifndef paddle_h_
#define paddle_h_

class Paddle {
public:
  Paddle(int x, int max_height, int w, int h, int left_bool) {
    max_h_ = max_height;
    x_ = x;
    y_ = (max_height-h)/2;
    width_ = w;
    height_ = h;
    left_bool_ = left_bool;
  }
  void move_up();
  void move_down();
  int get_x() {return x_;}
  int get_y() {return y_;}
  int get_width() {return width_;}
  int get_height() {return height_;}
  int is_left_paddle() {return left_bool_;}
private:
  int x_;         // (x,y) is the top left corner of the paddle.
  int y_;
  int width_;     // Width and height of the paddle.
  int height_;
  int max_h_;     // Max height of the screen.
  int left_bool_; // If the paddle is player 1 (left).
};

#endif