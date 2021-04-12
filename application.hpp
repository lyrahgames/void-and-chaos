#pragma once
#include <cstdint>
//
#include <SFML/Graphics.hpp>
//
#include <game.hpp>

class application {
 public:
  application();
  ~application();

  void execute();

 private:
  void process_events();
  void render();
  void resize();

 private:
  struct gamepad {
    enum button { A = 0, B, X, Y, LB, RB, SELECT, START, SUPER, LS, RS };
    struct axis {
      using type = sf::Joystick::Axis;
      static constexpr type LS_X = sf::Joystick::X;
      static constexpr type LS_Y = sf::Joystick::Y;
      static constexpr type RS_X = sf::Joystick::U;
      static constexpr type RS_Y = sf::Joystick::V;
      static constexpr type LT = sf::Joystick::Z;
      static constexpr type RT = sf::Joystick::R;
      static constexpr type DPAD_X = sf::Joystick::PovX;
      static constexpr type DPAD_Y = sf::Joystick::PovY;
    };
    void update();
    bool button_hit(button b);
    bool button_released(button b);
    bool button_pressed(button b);
    float position(axis::type a);
    // State
    int id = 0;
    uint32_t hit_mask = 0;
    uint32_t hit = 0;
    uint32_t release = 0;
  } current_gamepad;

 private:
  sf::RenderWindow window;
  sf::View view;
  float cell_size = 20.0f;
  float min_cell_size = 15.0f;
  float max_cell_size = 40.0f;

 private:
  game current_game;

  void update_camera();
  void render_player();
  void render_platforms();
};