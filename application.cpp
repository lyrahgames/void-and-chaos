#include <application.hpp>
//
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

application::application() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  window.create(sf::VideoMode(800, 450), "Game", sf::Style::Default, settings);
  window.setVerticalSyncEnabled(true);
  window.setKeyRepeatEnabled(false);
  // resize();

  view.setCenter({0, 0});
  view.setSize(window.getSize().x, window.getSize().y);
  view.zoom(1 / cell_size);
  window.setView(view);
}

application::~application() {}

void application::execute() {
  while (window.isOpen()) {
    process_events();
    current_game.update();
    update_camera();
    render();
  }
}

void application::process_events() {
  sf::Event event{};
  while (window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::MouseWheelMoved:
        cell_size *= exp(-event.mouseWheel.delta * 0.05f);
        cell_size = clamp(cell_size, min_cell_size, max_cell_size);
        resize();
        break;

      case sf::Event::Resized:
        resize();
        break;

      case sf::Event::KeyPressed:
        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;

          case sf::Keyboard::Space:
            current_game.jump();
            break;
        }
        break;
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) current_game.right();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) current_game.left();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) current_game.up();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) current_game.down();

  if (sf::Joystick::isConnected(current_gamepad.id)) {
    current_gamepad.update();
    if (current_gamepad.button_hit(gamepad::button::START)) window.close();
    if (current_gamepad.button_hit(gamepad::button::A)) current_game.jump();
    float x = current_gamepad.position(gamepad::axis::LS_X);
    x = (abs(x) < 10) ? 0 : (x / 5);
    current_game.move(x);
  }
}

void application::gamepad::update() {
  const auto button_count = sf::Joystick::getButtonCount(id);
  hit = 0;
  release = 0;
  for (int i = 0; i < button_count; ++i) {
    if (sf::Joystick::isButtonPressed(id, i)) {
      hit |= (uint32_t{1} << i) & (~hit_mask);
      hit_mask |= uint32_t{1} << i;
    } else {
      release |= (uint32_t{1} << i) & hit_mask;
      hit_mask &= (~(uint32_t{1} << i));
    }
  }
}

bool application::gamepad::button_hit(button b) {
  return hit & (uint32_t{1} << b);
}

bool application::gamepad::button_released(button b) {
  return release & (uint32_t{1} << b);
}

bool application::gamepad::button_pressed(button b) {
  return sf::Joystick::isButtonPressed(id, b);
}

float application::gamepad::position(axis::type a) {
  return sf::Joystick::getAxisPosition(id, a);
}

void application::resize() {
  view.setSize(window.getSize().x, window.getSize().y);
  view.zoom(1 / cell_size);
  window.setView(view);
}

void application::render() {
  window.clear(sf::Color::White);
  render_platforms();
  render_player();
  window.display();
}

void application::update_camera() {
  view.setCenter({current_game.player.p.x, current_game.player.p.y});
  window.setView(view);
}

void application::render_player() {
  sf::RectangleShape box(
      {current_game.player.size.x, current_game.player.size.y});
  box.setOrigin(0.5 * current_game.player.size.x,
                0.5 * current_game.player.size.y);
  box.setPosition(current_game.player.p.x, current_game.player.p.y);
  box.setFillColor(sf::Color::Red);
  window.draw(box);
}

void application::render_platforms() {
  for (const auto& p : current_game.platforms) {
    sf::RectangleShape box({p.max.x - p.min.x, p.max.y - p.min.y});
    box.setPosition(p.min.x, p.min.y);
    box.setFillColor(sf::Color::Black);
    window.draw(box);
  }
}