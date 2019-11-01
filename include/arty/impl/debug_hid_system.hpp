#ifndef DEBUG_HID_SYSTEM_HPP
#define DEBUG_HID_SYSTEM_HPP

#include <arty/core/window.h>

#include <arty/core/renderer.hpp>
#include <arty/core/system.hpp>

namespace arty {

class DebugHidSystem : public System {
 public:
  DebugHidSystem(Ptr<Window> window, Ptr<ITextRenderer> renderer)
      : _window(window), _renderer(renderer) {}

 private:
  Ptr<Window> _window;
  Ptr<ITextRenderer> _renderer;

  // System interface
 public:
  Result process(const Ptr<Blackboard> &board);
  Result init(const Ptr<Blackboard> &board);
  void release();
};

}  // namespace arty

#endif  // DEBUG_HID_SYSTEM_HPP
