#include <actions_handler.hpp>
//
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

void actions_handler::set(const string& name, const std::vector<event>& events,
                          callback call) {
  auto& [b, c] = actions[name];
  b = binding{events};
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

actions_handler& actions_handler::set(const string& name,
                                      const string& binding_code,
                                      callback call) {
  set(name, parse_binding(binding_code), call);
  return *this;
}

actions_handler& actions_handler::set(const string& name, callback call) {
  auto& [b, c] = actions[name];
  c = call;
  return *this;
}

actions_handler& actions_handler::set(const string& name,
                                      const string& binding_code) {
  auto& [b, c] = actions[name];
  b = binding{parse_binding(binding_code)};
  return *this;
}

auto actions_handler::parse_binding(const string& source) -> vector<event> {
  unordered_map<string, int> key_map{
      // Letters
      {"a", sf::Keyboard::A},
      {"b", sf::Keyboard::B},
      {"c", sf::Keyboard::C},
      {"d", sf::Keyboard::D},
      {"e", sf::Keyboard::E},
      {"f", sf::Keyboard::F},
      {"g", sf::Keyboard::G},
      {"h", sf::Keyboard::H},
      {"i", sf::Keyboard::I},
      {"j", sf::Keyboard::J},
      {"k", sf::Keyboard::K},
      {"l", sf::Keyboard::L},
      {"m", sf::Keyboard::M},
      {"n", sf::Keyboard::N},
      {"o", sf::Keyboard::O},
      {"p", sf::Keyboard::P},
      {"q", sf::Keyboard::Q},
      {"r", sf::Keyboard::R},
      {"s", sf::Keyboard::S},
      {"t", sf::Keyboard::T},
      {"u", sf::Keyboard::U},
      {"v", sf::Keyboard::V},
      {"w", sf::Keyboard::W},
      {"x", sf::Keyboard::X},
      {"y", sf::Keyboard::Y},
      {"z", sf::Keyboard::Z},
      // Digits
      {"0", sf::Keyboard::Num0},
      {"1", sf::Keyboard::Num1},
      {"2", sf::Keyboard::Num2},
      {"3", sf::Keyboard::Num3},
      {"4", sf::Keyboard::Num4},
      {"5", sf::Keyboard::Num5},
      {"6", sf::Keyboard::Num6},
      {"7", sf::Keyboard::Num7},
      {"8", sf::Keyboard::Num8},
      {"9", sf::Keyboard::Num9},
      // Control Keys
      {"esc", sf::Keyboard::Escape},
      {"ctrl", sf::Keyboard::LControl},
      {"shift", sf::Keyboard::LShift},
      {"alt", sf::Keyboard::LAlt},
      {"system", sf::Keyboard::LSystem},
      {"rctrl", sf::Keyboard::RControl},
      {"rshift", sf::Keyboard::RShift},
      {"ralt", sf::Keyboard::RAlt},
      {"rsystem", sf::Keyboard::RSystem},
      {"menu", sf::Keyboard::Menu},
      {"[", sf::Keyboard::LBracket},
      {"]", sf::Keyboard::RBracket},
      {";", sf::Keyboard::Semicolon},
      {",", sf::Keyboard::Comma},
      {".", sf::Keyboard::Period},
      {"'", sf::Keyboard::Quote},
      {"/", sf::Keyboard::Slash},
      {"\\", sf::Keyboard::Backslash},
      {"tilde", sf::Keyboard::Tilde},
      {"=", sf::Keyboard::Equal},
      {"-", sf::Keyboard::Hyphen},

      {"space", sf::Keyboard::Space},
      {"enter", sf::Keyboard::Enter},
      {"backspace", sf::Keyboard::Backspace},
      {"tab", sf::Keyboard::Tab},
      {"pageup", sf::Keyboard::PageUp},
      {"pagedown", sf::Keyboard::PageDown},
      {"end", sf::Keyboard::End},
      {"home", sf::Keyboard::Home},
      {"insert", sf::Keyboard::Insert},
      {"del", sf::Keyboard::Delete},
      {"add", sf::Keyboard::Add},
      {"sub", sf::Keyboard::Subtract},
      {"mult", sf::Keyboard::Multiply},
      {"div", sf::Keyboard::Divide},

      {"left", sf::Keyboard::Left},
      {"right", sf::Keyboard::Right},
      {"up", sf::Keyboard::Up},
      {"down", sf::Keyboard::Down},

      {"np0", sf::Keyboard::Numpad0},
      {"np1", sf::Keyboard::Numpad1},
      {"np2", sf::Keyboard::Numpad2},
      {"np3", sf::Keyboard::Numpad3},
      {"np4", sf::Keyboard::Numpad4},
      {"np5", sf::Keyboard::Numpad5},
      {"np6", sf::Keyboard::Numpad6},
      {"np7", sf::Keyboard::Numpad7},
      {"np8", sf::Keyboard::Numpad8},
      {"np9", sf::Keyboard::Numpad9},

      {"f1", sf::Keyboard::F1},
      {"f2", sf::Keyboard::F2},
      {"f3", sf::Keyboard::F3},
      {"f4", sf::Keyboard::F4},
      {"f5", sf::Keyboard::F5},
      {"f6", sf::Keyboard::F6},
      {"f7", sf::Keyboard::F7},
      {"f8", sf::Keyboard::F8},
      {"f9", sf::Keyboard::F9},
      {"f10", sf::Keyboard::F10},
      {"f11", sf::Keyboard::F11},
      {"f12", sf::Keyboard::F12},
      {"f13", sf::Keyboard::F13},
      {"f14", sf::Keyboard::F14},
      {"f15", sf::Keyboard::F15},

      {"pause", sf::Keyboard::Pause},
  };

  vector<event> events{};
  bool is_type_given = false;
  struct event::type type = key_down;
  string key{};

  for (size_t i = 0; i < source.size(); ++i) {
    const auto c = source[i];
    if (isspace(c)) continue;
    if (c == '+') {
      cout << key << '\n';
      if (is_type_given)
        events.push_back({type, key_map.at(key)});
      else
        events.push_back({key_down, key_map.at(key)});
      key = {};
      is_type_given = false;
      continue;
    }
    if (c == '~') {
      is_type_given = true;
      type = key_down;
      continue;
    }
    if (c == '!') {
      is_type_given = true;
      type = sf::Event::KeyReleased;
      continue;
    }
    if (c == '*') {
      is_type_given = true;
      type = sf::Event::KeyPressed;
      continue;
    }
    key += tolower(c);
  }
  cout << key << '\n';
  if (is_type_given)
    events.push_back({type, key_map.at(key)});
  else
    events.push_back({sf::Event::KeyPressed, key_map.at(key)});
  return events;
}

actions_handler& actions_handler::load(const string& file_path) {
  ifstream file{file_path, ios::in};
  if (!file.is_open())
    throw runtime_error("Error: 'actions_handler' failed to open file '" +
                        file_path + "'!");

  string name;
  string binding;
  while (getline(file, name, ':')) {
    getline(file, binding);
    set(name, binding);
    cout << "binding added: " << name << '\n';
  }

  return *this;
}

actions_handler& actions_handler::print_invalid_actions() {
  for (auto& [name, action] : actions)
    if (!action.is_valid()) cout << "Invalid Action: " << name << '\n';
  return *this;
}
