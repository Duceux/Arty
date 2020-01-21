#include <arty/core/mesh.hpp>
#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/ext/opengl/gl_shape_renderer.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision_rendering_system.hpp>
#include <arty/impl/collision_solver_system.hpp>
#include <arty/impl/collision_system.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/engine.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>
#include <arty/impl/physics_system.hpp>

using namespace arty;

void makeCube(std::string const& name, Transform const& pos,
              Vec3f const& length, float mass, Ptr<Memory> mem) {
  auto entity = mem->createEntity(name);
  mem->write(entity, PhysicsSystem::INOUT_1, pos);
  mem->write(entity, HitBoxRenderingSystem::DRAW_PROP,
             AABox(Vec3f(0.f, 0.f, 0.f), length));
  mem->write(entity, PhysicsSystem::INOUT_2, Physics(mass));
}

int main(void) {
  GlfwWindow* window_impl = new GlfwWindow;
  Ptr<Keyboard> keyboard = window_impl->provideKeyboard();
  Ptr<Window> window(window_impl);
  Ptr<Memory> board(new Memory);
  Ptr<ITextRenderer> textRenderer(new GlTextRenderer());
  Ptr<IShapeRenderer> shapeRenderer(new GlShapeRenderer());

  WorldPhysics world;
  world.gravity_strengh = 9.8f;

  Engine engine;
  engine.setBoard(board)
      .setWindow(window)
      .setKeyboard(keyboard)
      .makeSystem<DebugHidSystem>(window, textRenderer)
      .makeSystem<FixedCameraSystem>(window)
      .makeSystem<HitBoxRenderingSystem>(shapeRenderer)
      .makeSystem<PhysicsSystem>(world)
      .makeSystem<CollisionDetectionSystem>()
      .makeSystem<CollisionRenderingSystem>(shapeRenderer)
      .makeSystem<CollisionSolverSystem>();

  makeCube("unit", Transform(), Vec3f(1.f, 1.f, 1.f), 1.f, board);
  makeCube("cube", Transform(Vec3f(0.f, 0.f, 3.f)), Vec3f(0.5f, 0.5f, 0.5f),
           1.f, board);
  makeCube("floor", Transform(Vec3f(0.f, 0.f, -5.f)), Vec3f(10.f, 10.f, 0.5f),
           0.f, board);

  check_result(engine.start());
  check_result(engine.run());
  return ok();
}
