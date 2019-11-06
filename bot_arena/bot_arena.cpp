#include <arty/ext/opengl/opengl_renderer.h>
#include <arty/ext/opengl/opengl_window.h>
#include <arty/impl/engine.h>

#include <arty/core/mesh.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/ext/opengl/gl_shape_renderer.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision_rendering_system.hpp>
#include <arty/impl/collision_solver_system.hpp>
#include <arty/impl/collision_system.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/mesh_loader_system.hpp>
#include <arty/impl/physics_system.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace arty;

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
  Ptr<Blackboard> board(new Blackboard);
  Ptr<ITextRenderer> textRenderer(new GlTextRenderer());
  Ptr<IShapeRenderer> shapeRenderer(new GlShapeRenderer());
  WorldPhysics world;
  world.gravity_strengh = 0.0f;
  world.air_friction = 1.f;
  world.ground_friction = 0.9f;
  Engine engine;
  engine.set_board(board)
      .set_window(window)
      .add_system(Ptr<System>(new CollisionSolverSystem()))
      .add_system(Ptr<System>(new PhysicsSystem(world)))
      .add_system(Ptr<System>(new CameraSystem(window)))
      .add_system(Ptr<System>(new CollisionRenderingSystem(shapeRenderer)))
      .add_system(Ptr<System>(new DebugHidSystem(window, textRenderer)))
      .add_system(Ptr<System>(new MeshLoaderSystem))
      .add_system(Ptr<System>(new OpenGlRenderer))
      .add_system(Ptr<System>(new CollisionSystem()));

  auto pyramid = board->createEntity("pyramid");
  board->set(pyramid, "model2load", std::string("../models/test_pyramid.obj"));
  board->set(pyramid, "transform", Transform(Vec3f{3.f, 0.f, 0.f}));
  board->set(pyramid, "physics", Physics());

  auto cube = board->createEntity("cube");
  board->set(cube, "model2load", std::string("../models/test_cube.obj"));
  Mat4x4f tf = /*rotation(1.f, 0.f, 0.f) +*/ translation(-3.f, 0.f, 0.f);
  board->set(cube, "transform", Transform::from(tf));
  board->set(cube, "camtarget", cube);
  board->set(cube, "physics", Physics());

  check_result(engine.start());
  check_result(engine.run());
  return ok();
}
