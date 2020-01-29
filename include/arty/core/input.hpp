#ifndef INPUT_H
#define INPUT_H

#include <arty/core/math.hpp>
#include <arty/core/result.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace arty {

class Event {
 private:
  std::string _name;
  int _uuid;
  static int _count;

 public:
  Event(std::string const& name) : _name(name), _uuid(++_count) {}

  int uuid() const { return _uuid; }
  std::string const& name() const { return _name; }

  bool operator==(Event const& other) const { return _uuid == other._uuid; }
};
}  // namespace arty

namespace std {

template <>
class hash<arty::Event> {
 public:
  size_t operator()(arty::Event const& s) const {
    return std::hash<int>()(s.uuid());
  }
};

ostream& operator<<(ostream& out, arty::Event const& event);

}  // namespace std

namespace arty {

class Device {
 public:
  using event_t = Event;
  enum Action { SUNKNOWN, PRESS, HOLD, RELEASE };

  void process(int trigger, Action const& action);

  bool registerEvent(int key, Action const& action, event_t const& event);

  event_t generate(std::string const& name);

  void flush();

  bool occured(event_t const& e);

 protected:
  std::unordered_map<int, std::unordered_map<Action, event_t>> mapping_;
  std::unordered_set<event_t> _incomings;
  std::unordered_set<event_t> _pool;
};

class Keyboard : public Device {
 public:
  using event_t = Event;

  enum Key {
    /* The unknown key */
    CUNKNOWN = -1,
    /* Printable keys */
    SPACE = 32,
    APOSTROPHE = 39, /* ' */
    COMMA = 44,      /* , */
    MINUS = 45,      /* - */
    PERIOD = 46,     /* . */
    SLASH = 47,      /* / */
    K0 = 48,
    K1 = 49,
    K2 = 50,
    K3 = 51,
    K4 = 52,
    K5 = 53,
    K6 = 54,
    K7 = 55,
    K8 = 56,
    K9 = 57,
    SEMICOLON = 59, /* ; */
    EQUAL = 61,     /* = */
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    LEFT_BRACKET = 91,  /* [ */
    BACKSLASH = 92,     /* \ */
    RIGHT_BRACKET = 93, /* ] */
    GRAVE_ACCENT = 96,  /* ` */
    WORLD_1 = 161,      /* non-US #1 */
    WORLD_2 = 162,      /* non-US #2 */
    /* Function keys */
    ESCAPE = 256,
    ENTER = 257,
    TAB = 258,
    BACKSPACE = 259,
    INSERT = 260,
    DELETE = 261,
    RIGHT = 262,
    LEFT = 263,
    DOWN = 264,
    UP = 265,
    PAGE_UP = 266,
    PAGE_DOWN = 267,
    HOME = 268,
    END = 269,
    CAPS_LOCK = 280,
    SCROLL_LOCK = 281,
    NUM_LOCK = 282,
    PRINT_SCREEN = 283,
    PAUSE = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    KP_0 = 320,
    KP_1 = 321,
    KP_2 = 322,
    KP_3 = 323,
    KP_4 = 324,
    KP_5 = 325,
    KP_6 = 326,
    KP_7 = 327,
    KP_8 = 328,
    KP_9 = 329,
    KP_DECIMAL = 330,
    KP_DIVIDE = 331,
    KP_MULTIPLY = 332,
    KP_SUBTRACT = 333,
    KP_ADD = 334,
    KP_ENTER = 335,
    KP_EQUAL = 336,
    LEFT_SHIFT = 340,
    LEFT_CONTROL = 341,
    LEFT_ALT = 342,
    LEFT_SUPER = 343,
    RIGHT_SHIFT = 344,
    RIGHT_CONTROL = 345,
    RIGHT_ALT = 346,
    RIGHT_SUPER = 347,
    MENU = 348,
    LAST = MENU,
  };

  void process(Key const& key, Action const& action) {
    Device::process(key, action);
  }

  bool registerKeyEvent(Key const& key, Action const& action,
                        event_t const& event) {
    return Device::registerEvent(key, action, event);
  }
};

class Mouse : public Device {
 public:
  using position_type = Vec2d;

  enum Button { UNKNOWN = -1, LEFT = 0, RIGHT = 1 };

  virtual position_type position() const = 0;
  virtual void set(position_type const& pos) = 0;
};

class Input {
 public:
  enum Type { Keyboard, Mouse };

  Input(Keyboard::Key const& key, Device::Action const& action)
      : _key(key), _button(Mouse::UNKNOWN), _type(Keyboard), _action(action) {}
  Input(Mouse::Button const& key, Device::Action const& action)
      : _key(Keyboard::CUNKNOWN), _button(key), _type(Mouse), _action(action) {}

  Keyboard::Key const& key() const { return _key; }
  Mouse::Button const& button() const { return _button; }
  Type const& type() const { return _type; }
  Device::Action const& action() const { return _action; }

 private:
  Keyboard::Key _key;
  Mouse::Button _button;
  Type _type;
  Device::Action _action;
};

class InputManager {
 public:
  void setKeyboard(const Ptr<Keyboard>& ptr) { _keyboard = ptr; }

  void setMouse(Ptr<Mouse> const& ptr) { _mouse = ptr; }

  bool attach(Input const& in, Event const& ev);
  bool attach(Keyboard::Key const& key, Device::Action const& action,
              Event const& event);
  bool attach(Mouse::Button const& key, Device::Action const& action,
              Event const& event);

  bool pop(Event const& event);

  Mouse::position_type position() const;
  void set(Mouse::position_type const& pos);

  void flush();

 private:
  Ptr<Mouse> _mouse;
  Ptr<Keyboard> _keyboard;
};

}  // namespace arty

#endif  // INPUT_H
