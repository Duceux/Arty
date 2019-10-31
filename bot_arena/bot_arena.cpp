#include <arty/ext/opengl_renderer.h>
#include <arty/ext/opengl_window.h>
#include <arty/impl/engine.h>

#include <arty/core/mesh.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/mesh_loader_system.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace arty;

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
  Ptr<Blackboard> board(new Blackboard);
  Engine engine;
  engine.set_board(board)
      .set_window(window)
      .add_system(Ptr<System>(new MeshLoaderSystem))
      .add_system(Ptr<System>(new CameraSystem(window)))
      .add_system(Ptr<System>(new OpenGlRenderer));

  auto pyramid = board->createEntity("pyramid");
  board->set(pyramid, "model2load", std::string("../models/bot.obj"));
  board->set(pyramid, "transform", Transform());

  check_result(engine.start());
  check_result(engine.run());
  return ok();
}
