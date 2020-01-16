#ifndef HITBOX_RENDERING_SYSTEM_HPP
#define HITBOX_RENDERING_SYSTEM_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/system.hpp>

namespace arty {

class IShapeRenderer {
 public:
  virtual ~IShapeRenderer() {}
  virtual Result init() = 0;
  virtual void draw(const Entity &e, const Box &s, const Mat4x4f &model,
                    const Mat4x4f &view, const Mat4x4f &proj) = 0;
  virtual void release() = 0;
};

class HitBoxRenderingSystem : public System {
 public:
  static constexpr const char *DRAW_PROP = "hitbox";

 public:
  HitBoxRenderingSystem(Ptr<IShapeRenderer> rend) : _renderer(rend) {}

 private:
  Ptr<IShapeRenderer> _renderer;
  // System interface
 public:
  Result process(const Ptr<Blackboard> &board) override;
  Result init(const Ptr<Blackboard> &board) override;
  void release() override;
};

}  // namespace arty

#endif  // HITBOX_RENDERING_SYSTEM_HPP
