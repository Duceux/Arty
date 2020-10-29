#ifndef CAMERA_SYSTEM_HPP
#define CAMERA_SYSTEM_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/math.hpp>
#include <arty/core/system.hpp>
#include <arty/core/window.hpp>
#include <cmath>
#include <limits>

namespace arty {

class Camera {
 public:
  using number_type = float;
  using mat_type = Mat<number_type, 4, 4>;
  using point_type = Vec3<number_type>;
  using vector_type = Vec4<number_type>;
  using pixel_type = Vec2<number_type>;
  using ray_type = Line3<number_type>;

  mat_type const& projection() const { return _projection; }
  mat_type view() const { return _inv_rot * _inv_tran; }
  mat_type transform() const { return (-_inv_tran) * _inv_rot.transpose(); }
  point_type position() const {
    return transform().block<number_type, 3, 1>(0, 3);
  }

  pixel_type worldToPixel(point_type const& pt) const;

  void perspective(number_type const& fov, number_type const& aspect,
                   number_type const& znear, number_type const& zfar);

  void lookAt(point_type const& eye, point_type const& target,
              point_type const& updir);

  ray_type raycast(pixel_type const& pixel) {
    vector_type mpp(pixel, -1.f, 1.f);
    auto dir = transform() * mpp;
    auto ori = point_type(-_inv_tran(0, 3), -_inv_tran(1, 3), -_inv_tran(2, 3));
    return ray_type(ori, point_type(dir.x(), dir.y(), dir.z()));
  }

  number_type distanceTo(point_type const& pt) {
    auto cam = point_type(-_inv_tran(0, 3), -_inv_tran(1, 3), -_inv_tran(2, 3));
    return (cam - pt).norm();
  }

 private:
  Mat4x4f _projection;
  Mat4x4f _inv_rot;
  Mat4x4f _inv_tran;
};  // namespace arty

class FixedCameraSystem : public System {
 public:
  FixedCameraSystem(Ptr<Window> const& w);

  Result process(Ptr<Memory> const& board) override;

  void setEye(Vec3f const& eye) { _eye = eye; }

  void setTarget(Vec3f const& target) { _target = target; }

  void setUpdir(Vec3f const& updir) { _updir = updir; }

 private:
  Ptr<Window> _window;
  Mat4x4f _view;
  Mat4x4f _projection;
  Mat4x4f _camFromWorld;
  float _fov;
  Vec3f _eye;
  Vec3f _target;
  Vec3f _updir;
};

}  // namespace arty

#endif  // CAMERA_SYSTEM_HPP
