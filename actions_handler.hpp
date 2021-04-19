#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
//
#include <SFML/Graphics.hpp>

class actions_handler {
 public:
  using string = std::string;

  struct event {
    // The type of this custom event struct should extend
    // the standard SFML event type with real-time event types.
    struct type {
      enum class realtime : int {
        keyboard = sf::Event::Count + 1,
        mouse,
        joystick,
        count
      };
      static constexpr size_t count = static_cast<size_t>(realtime::count);

      // Implicit Construction
      // These operations are not save and
      // should not be used from the outside.
      type(sf::Event::EventType t) : id(static_cast<int>(t)) {}
      type(realtime t) : id(static_cast<int>(t)) {}
      operator realtime() { return static_cast<realtime>(id); }
      bool operator==(sf::Event::EventType t) const {
        return id == static_cast<int>(t);
      }
      bool operator!=(sf::Event::EventType t) const {
        return id != static_cast<int>(t);
      }

      // In the end the complete type is given by an ID.
      int id{};
    };

    struct info {
      int code;
    };

    type type;
    info info;
  };

  static constexpr event::type::realtime key_down =
      event::type::realtime::keyboard;
  static constexpr event::type::realtime mouse_down =
      event::type::realtime::mouse;

  // Structure describing a set of events, like shortcuts,
  // and their current state of activity.
  class binding {
   public:
    struct event_data {
      sf::Uint32 text_entered{};
      sf::Vector2i mouse{};
      int mouse_scroll_delta = 0;
    };

    // Bindings do not have to exist.
    // This makes sure a callback can be given without a binding.
    // The binding can then be added later.
    binding() = default;
    binding(const std::vector<event>& v)
        : events{v}, _data{std::make_unique<event_data>()} {}

    // Check if binding has already been created.
    bool is_existent() const { return _data != nullptr; }
    // Check if binding has been activated to run the callback.
    bool is_active() const {
      return is_existent() && (active_events == events.size());
    }
    event_data* data() { return _data.get(); }
    // After the callback has been run, the binding must be reset.
    void reset() {
      active_events = 0;
      *_data = {};
    }
    // Update activity based processed event.
    void process_event(const sf::Event& event);
    // Update activity of binding based on real-time events.
    void update();

   private:
    // In general, bindings will only have very few events.
    // No faster container needed.
    std::vector<event> events{};
    size_t active_events{};
    std::unique_ptr<event_data> _data{};
  };

  using callback = std::function<void(const binding::event_data*)>;

  struct action {
    binding bind;
    callback call;
  };

 public:
  // If there is no focus on the application
  // then the actions handler should be disabled.
  bool is_enabled() { return _is_enabled; }
  void enable() { _is_enabled = true; }
  void disable() { _is_enabled = false; }

  // In every iteration of the game loop, the actions handler
  // has to process all events and then invoke the callbacks
  // if the bindings are activated.
  void process_event(const sf::Event& event);
  void update();

  // Set actions from the outside.
  void set(const string& name, const std::vector<event>& events,
           callback callback);

 private:
  std::unordered_map<string, action> actions;
  bool _is_enabled = true;
};