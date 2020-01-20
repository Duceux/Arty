#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <arty/core/system.hpp>

namespace arty {

struct WorldPhysics {
  float gravity_strengh;
  float air_friction;
  float ground_friction;
  float min_ground_level;
};

struct Physics {
  Entity parent;
  std::vector<Entity> childs;

  Transform velocity;
  Transform acceleration;
  std::vector<Vec3f> forces_dir;
  std::vector<Vec3f> forces_pos;
  float mass;

  Physics() : Physics(1.f) {}
  Physics(float mass)
      : velocity(), acceleration(), forces_dir(), forces_pos(), mass(mass) {}
};

class PhysicsSolver {
 public:
  void update(Transform* tf, Physics* phy, WorldPhysics const& world);

 private:
};

class PhysicsSystem : public System {
 public:
  static constexpr const char* INPUT_PROP = "physics";
  static constexpr const char* OUTPUT_PROP = "transform";

  PhysicsSystem(WorldPhysics const& world) : _solver(), _world(world) {}

 private:
  PhysicsSolver _solver;
  WorldPhysics _world;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
  Result init(const Ptr<Memory>& board) override;
  void release() override;
};

}  // namespace arty

#endif  // PHYSICS_SYSTEM_HPP
