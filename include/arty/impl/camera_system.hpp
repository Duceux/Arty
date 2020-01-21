#ifndef CAMERA_SYSTEM_HPP
#define CAMERA_SYSTEM_HPP

#include <arty/core/math.hpp>
#include <arty/core/system.hpp>
#include <arty/core/window.hpp>

namespace arty {

struct Camera {
  Mat4x4f projection;
  Mat4x4f view;
};

class FixedCameraSystem : public System {
 public:
  FixedCameraSystem(Ptr<Window> const& w);

  Result process(Ptr<Memory> const& board) override;

 private:
  Ptr<Window> _window;
  Mat4x4f _view;
  Mat4x4f _projection;
  Mat4x4f _camFromWorld;
  float _fov;
};

}  // namespace arty

#endif  // CAMERA_SYSTEM_HPP
