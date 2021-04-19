#include <actions_handler.hpp>
//
#include <iomanip>
#include <iostream>

using namespace std;

void actions_handler::set(const string& name, const std::vector<event>& events,
                          callback call) {
  auto& [b, c] = actions[name];
  b = {events};
  c = call;
}

void actions_handler::process_event(const sf::Event& event) {
  for (auto& [name, action] : actions) {
    auto& [bind, _] = action;
    bind.process_event(event);
  }
}

void actions_handler::binding::process_event(const sf::Event& event) {
  for (auto& e : events) {
    if (event.type != e.type) continue;

    // Set bind information based on event type.
    switch (event.type) {
      // Keyboard
      case sf::Event::KeyPressed:
      case sf::Event::KeyReleased:
        if (e.info.code == event.key.code) ++active_events;
        break;

      // Mouse
      case sf::Event::MouseButtonPressed:
      case sf::Event::MouseButtonReleased:
        if (e.info.code == event.mouseButton.button) {
          _data->mouse.x = event.mouseButton.x;
          _data->mouse.y = event.mouseButton.y;
          ++active_events;
        }
        break;
      case sf::Event::MouseWheelScrolled:
        _data->mouse_scroll_delta = event.mouseWheelScroll.delta;
        _data->mouse.x = event.mouseWheelScroll.x;
        _data->mouse.y = event.mouseWheelScroll.y;
        ++active_events;
        break;
      case sf::Event::MouseMoved:
        _data->mouse.x = event.mouseMove.x;
        _data->mouse.y = event.mouseMove.y;
        ++active_events;
        break;
      case sf::Event::MouseEntered:
      case sf::Event::MouseLeft:
        ++active_events;
        break;

      // Misc
      case sf::Event::TextEntered:
        _data->text_entered = event.text.unicode;
        ++active_events;
        break;
    }
  }
}

void actions_handler::update() {
  if (!_is_enabled) return;
  for (auto& [name, action] : actions) {
    auto& [bind, call] = action;
    bind.update();
    if (bind.is_active() && call) std::invoke(call, bind.data());
    bind.reset();
  }
}

void actions_handler::binding::update() {
  for (auto& e : events) {
    switch (e.type) {
      case event::type::realtime::keyboard:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e.info.code)))
          ++active_events;
        break;

      case event::type::realtime::mouse:
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e.info.code)))
          ++active_events;
        break;

      case event::type::realtime::joystick:
        break;
    }
  }
}