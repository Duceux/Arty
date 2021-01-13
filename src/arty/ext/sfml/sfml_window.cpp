#include <arty/ext/sfml/sfml_window.hpp>

namespace arty {

SfmlWindow::~SfmlWindow() {
  if (_window) {
    delete _window;
    _window = nullptr;
  }
}

Result SfmlWindow::init() {
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.majorVersion = 3;
  settings.minorVersion = 2;

  _window = new sf::Window;
  if (_mode.type() == WindowMode::FULLSCREEN) {
    auto listsfmode = sf::VideoMode::getFullscreenModes();
    if (listsfmode.size()) {
      _window->create(listsfmode.front(), _name,
                      sf::Style::Titlebar | sf::Style::Close, settings);
      return ok();
    }
  }
  _window->create(sf::VideoMode(_mode.width(), _mode.height()), _name,
                  sf::Style::Titlebar | sf::Style::Close, settings);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();

  return ok();
}

bool SfmlWindow::isOk() const { return _window && _window->isOpen(); }

bool SfmlWindow::isVisible() const { return isOk(); }

void SfmlWindow::refresh() {
  assert(_window);
  // check all the window's events that were triggered since the last iteration
  // of the loop
  sf::Event event;
  while (_window->pollEvent(event)) {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed) close();
  }
  _window->display();
}

int SfmlWindow::width() const {
  assert(_window);
  return _window->getSize().x;
}

int SfmlWindow::height() const {
  assert(_window);
  assert(_window);
  return _window->getSize().y;
}

void SfmlWindow::close() {
  assert(_window);
  _window->close();
}

void SfmlWindow::clear() {
  assert(_window);
  _window->setActive();
}

}  // namespace arty