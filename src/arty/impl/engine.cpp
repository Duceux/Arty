#include <arty/impl/engine.hpp>

namespace arty {

Engine &Engine::setWindow(const Ptr<Window> &ptr) {
  _window = ptr;
  return *this;
}

Engine &Engine::setKeyboard(const Ptr<Keyboard> &ptr) {
  _keyboard = ptr;
  return *this;
}

Engine &Engine::addSystem(const Ptr<System> &system) {
  _systems.push_back(system);
  return *this;
}

Engine &Engine::setBoard(const Ptr<Memory> &board) {
  _state = board;
  return *this;
}

Result Engine::start() {
  check_result(_window->init());
  for (auto const &system : _systems) {
    check_result(system->init(_state, _keyboard));
  }
  return ok();
}

Result Engine::step() {
  Result res;
  _window->clear();
  for (auto system : _systems) {
    if (system) {
      res = system->process(_state, _keyboard);
      if (!res) {
        std::cerr << res.message() << std::endl;
      }
    } else {
      return error("Null ptr system");
    }
  }
  _window->swapBuffer();
  return ok();
}

Result Engine::run() {
  Result res;
  size_t count = 0;
  // For speed computation
  double lastTime = _window->getTime();
  int nbFrames = 0;
  do {
    // Measure speed
    double currentTime = _window->getTime();
    nbFrames++;
    // If last prinf() was more than 1sec ago
    if (currentTime - lastTime >= 1.0) {
      // printf and reset
      std::cout << (currentTime - lastTime) * 1000. / double(nbFrames)
                << " ms/frame" << std::endl;
      nbFrames = 0;
      lastTime = currentTime;
    }
    res = step();
    count++;
  } while (res && _window->isOk() && count < 10000);
  return res;
}

void Engine::stop() {
  for (auto const &system : _systems) {
    system->release();
  }
  _window->close();
}

}  // namespace arty
