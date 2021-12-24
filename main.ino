/*
Very simple pong game made entirely from scratch.
Score 10 points to win.

~ Tyler Chan
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "paddle.h"
#include "ball.h"

// -----===  DEFINITIONS  ===-----
#define SCREEN_COLOR WHITE // OLED display background color
#define OBJ_COLOR BLACK    // OLED display object color
#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 32   // OLED display height, in pixels
#define b1 5               // Player 1 UP Button Pin
#define b2 2               // Player 1 DOWN Button Pin
#define b3 8               // Player 2 UP Button Pin
#define b4 3               // Player 2 DOWN Button Pin

// -----===  FUNCTION PROTOTYPES  ===-----
void Screen_Init();
void GPIO_Init();
void draw_paddle(Paddle &p);
void draw_ball();
void check_interaction(Paddle &p);
bool check_player_scored();

// -----===  GLOBAL VARIABLES  ===-----
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The SDA and SCL pins A4 and A5 on Arduino Nano
Adafruit_SSD1306 disp(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
int p_w = 3;      // Paddle Width
int p_h = 10;     // Paddle Height
Ball ball = Ball(SCREEN_WIDTH, SCREEN_HEIGHT);
Paddle p1 = Paddle(                 10, SCREEN_HEIGHT, p_w, p_h, 1);
Paddle p2 = Paddle(SCREEN_WIDTH-10-p_w, SCREEN_HEIGHT, p_w, p_h, 0);
int score_flag = 0; // Flag is set to 1 if any player scores 
int p1_score = 0;
int p2_score = 0;

void setup() {
  Screen_Init();
  GPIO_Init();
  randomSeed(analogRead(0));
  ball.randomize_theta();
}

void loop() {
  if (digitalRead(b1) == HIGH) { p1.move_up();   }
  if (digitalRead(b2) == HIGH) { p1.move_down(); }
  if (digitalRead(b3) == HIGH) { p2.move_up();   }
  if (digitalRead(b4) == HIGH) { p2.move_down(); }
  if (!check_player_scored()) {
    check_interaction(p1);
    check_interaction(p2);
  }
  if (score_flag) {
    // A person scores -> reset ball and randomize the angle.
    if (p1_score == 10 || p2_score == 10) {
      // If a person gets to 10 points, then they win.
      // After 5 seconds, reset the game.
      disp.setCursor(SCREEN_WIDTH/2-40, SCREEN_HEIGHT/2-5);
      if (p2_score-10) { disp.println("PLAYER 1 WINS"); }
      else             { disp.println("PLAYER 2 WINS"); }
      disp.display();
      delay(5000);
      p1_score = 0;
      p2_score = 0;
    }
    ball = Ball(SCREEN_WIDTH, SCREEN_HEIGHT);
    ball.randomize_theta();
    score_flag = 0;
  }
  ball.update_pos(-1, SCREEN_WIDTH, -1);
  // Draw everything onto the screen and display it.
  disp.fillScreen(SCREEN_COLOR);
  draw_paddle(p1);
  draw_paddle(p2);
  draw_ball();
  disp.setCursor(13+p_w,1);
  disp.println(p1_score);
  disp.setCursor(SCREEN_WIDTH-18-p_w,1);
  disp.println(p2_score);
  // TESTING
  // disp.setCursor(0,0);
  // disp.println(p1.get_pos());
  // disp.println(ball.get_x());
  // disp.println(ball.get_y());
  // disp.println(ball.update_pos(-1, SCREEN_WIDTH, -1));
  // TESTING
  disp.display();
}

// -----===  FUNCTIONS  ===-----
void draw_paddle(Paddle &p) {
  // Draws the provided paddle onto the screen.
  disp.fillRect(p.get_x(), p.get_y(), p.get_width(),
                p.get_height(), OBJ_COLOR);
}

void draw_ball() {
  // Draws the ball onto the screen.
  disp.fillCircle(ball.get_x(), ball.get_y(),
                  ball.get_r(), OBJ_COLOR);
}

void check_interaction(Paddle &p) {
  // Checks to see if the ball collided with the paddles.
  if (ball.get_x() - ball.get_r() <= p.get_x() + p.get_width() &&
      ball.get_x() + ball.get_r() >= p.get_x() &&
      ball.get_y() + ball.get_r() >= p.get_y() &&
      ball.get_y() - ball.get_r() <= p.get_y() + p.get_height()) {
    // Checks to see if the ball is inside or touching the paddle.
    if (ball.get_y() > p.get_y() + p.get_height()) {
      // Edge Case: if the ball is touching the bottom of the paddle
      //            and its center is below the paddle -> bounce downwards.
      ball.set_y(p.get_y()+p.get_height()+ball.get_r());
      ball.update_pos(-1, SCREEN_WIDTH, p.get_y()+p.get_height());
    } else if (ball.get_y() < p.get_y()) {
      // Edge Case: if the ball is touching the top of the paddle
      //            and its center is below the paddle -> bounce upwards.
      ball.set_y(p.get_y()-ball.get_r());
      ball.update_pos(-1, SCREEN_WIDTH, p.get_y());
    } else {
      // Normal Case: bounce of the side of the paddle
      if (p.is_left_paddle()) {
        ball.set_x(p.get_x()+p.get_width()+ball.get_r());
        ball.update_pos(p.get_x()+p.get_width(), SCREEN_WIDTH, -1);
      } else {
        ball.set_x(p.get_x()-ball.get_r());
        ball.update_pos(-1, p.get_x(), -1);
      }
    }
  }
}

bool check_player_scored() {
  // Checks to see if the ball is out of bounds on either player's side,
  // and adds one point to the player that scored.
  if (ball.get_x()-ball.get_r() <= 0) {
    score_flag = 1;
    p2_score++;
    return true;
  } else if (ball.get_x()+ball.get_r() >= SCREEN_WIDTH) {
    score_flag = 1;
    p1_score++;
    return true;
  }
  return false;
}

void GPIO_Init() {
  // Intializes all the GPIO inputs and outputs
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);
}

void Screen_Init() {
  // Intializes the screen.
  Serial.begin(115200);
  if(!disp.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // Address 0x3C for 128x32
    // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  disp.clearDisplay();
  disp.setTextSize(1);
  disp.setTextColor(OBJ_COLOR);
  disp.setCursor(0,0);
  // Display static startup text and wait 1 second before
  // continuing to the game.
  disp.fillScreen(SCREEN_COLOR);
  disp.println("Nyahallo waowd!");
  disp.display(); 
  delay(1000);
}