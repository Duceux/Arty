#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <arty/core/math.hpp>
#include <vector>

namespace arty {

template <typename T, int Dim>
class Transform {
  static_assert(Dim > 1, "transform in small space doesn't make any sense");

 public:
  static constexpr int dimension = Dim;
  using value_type = T;
  using translation_type = Vec<T, Dim>;
  using rotation_type = Mat<T, Dim, Dim>;
  using matrix_type = Mat<T, Dim + 1, Dim + 1>;
  using self_type = Transform<T, Dim>;

  Transform() : _translation(), _rotation(rotation_type::identity()) {}
  Transform(translation_type const& pos)
      : _translation(pos), _rotation(rotation_type::identity()) {}
  Transform(translation_type const& pos, rotation_type const& rot)
      : _translation(pos), _rotation(rot) {}

  matrix_type toMat() const;

  void fromMat(matrix_type const& m);

  static self_type from(Mat4x4f const& m) {
    self_type tf;
    tf.fromMat(m);
    return tf;
  }

  translation_type operator*(translation_type const& p) const {
    return _rotation * p + _translation;
  }

  self_type operator*(self_type const& p) const {
    self_type toto(*this);
    toto *= p;
    return toto;
  }

  self_type& operator*=(self_type const& r) {
    fromMat(toMat() * r.toMat());
    return *this;
  }

  self_type& operator+=(translation_type const& r) {
    _translation += r;
    return *this;
  }

  translation_type const& translation() const { return _translation; }
  translation_type& translation() { return _translation; }
  rotation_type const& rotation() const { return _rotation; }
  rotation_type& rotation() { return _rotation; }

 private:
  translation_type _translation;
  rotation_type _rotation;
};
using Tf3f = Transform<float, 3>;

template <class T>
class Intersection {
 private:
  bool _exist;
  T _value;

 public:
  using value_type = T;

  Intersection() : Intersection(false) {}
  Intersection(bool e) : Intersection(e, value_type()) {}
  Intersection(T const& val) : Intersection(true, val) {}
  Intersection(bool e, T const& val) : _exist(e), _value(val) {}

  bool empty() const { return !_exist; }
  bool exist() const { return _exist; }
  T const& value() const { return _value; }
};

template <typename T, int Dim>
class Line {
 public:
  Line(Vec<T, Dim> const& a, Vec<T, Dim> const& b) : _ori(a), _dir(b - a) {
    _normSquared = _dir.normsqr();
  }

  Vec<T, Dim> project(Vec<T, Dim> const& p) {
    return _ori + _dir * dirCoeff(p);
  }

  T distanceSquaredTo(Vec<T, Dim> const& p) {
    return (project(p) - p).normsqr();
  }

  T distanceTo(Vec<T, Dim> const& p) { return std::sqrt(distanceSquaredTo(p)); }

  T dirCoeff(Vec<T, Dim> const& p) {
    return (p - _ori).dot(_dir) / _normSquared;
  }

  Vec<T, Dim> const& origin() const { return _ori; }
  Vec<T, Dim> const& direction() const { return _dir; }

 private:
  Vec<T, Dim> _ori;
  Vec<T, Dim> _dir;
  T _normSquared;
};

template <typename T>
using Line3 = Line<T, 3>;
using Line3f = Line3<float>;

template <typename T, int Dim>
class Edge {
 public:
  using vector_type = Vec<T, Dim>;

  Edge(Vec<T, Dim> const& a, Vec<T, Dim> const& b) : _a(a), _b(b) {}

  vector_type project(vector_type const& p);

  T distanceSquaredTo(Vec<T, Dim> const& p) {
    return (project(p) - p).normsqr();
  }

  T distanceTo(Vec<T, Dim> const& p) { return std::sqrt(distanceSquaredTo(p)); }

  Vec<T, Dim> const& p1() const { return _a; }
  Vec<T, Dim> const& p2() const { return _b; }

 private:
  Vec<T, Dim> _a;
  Vec<T, Dim> _b;
};

template <typename T>
using Edge3 = Edge<T, 3>;
using Edge3f = Edge3<float>;

template <typename T>
class Plane {
 public:
  Plane(Vec3<T> const& pt1, Vec3<T> const& pt2, Vec3<T> const& pt3)
      : _line(pt1, cross(pt2 - pt1, pt3 - pt1)) {}
  Plane(Vec3<T> const& ori, Vec3<T> const& dir) : _line(ori, dir.normalize()) {}

  Vec3<T> project(Vec3<T> const& p) {
    T coeff = _line.dirCoeff(p);
    return p - _line.direction() * coeff;
  }

  T distanceSquaredTo(Vec3<T> const& p) { return (project(p) - p).normsqr(); }

  T distanceTo(Vec3<T> const& p) { return std::sqrt(distanceSquaredTo(p)); }

  Vec3<T> const& origin() const { return _line.origin(); }
  Vec3<T> direction() const { return _line.direction(); }

  T sideOf(Vec3<T> const& p) {
    T coeff = _line.dirCoeff(p);
    return (T(0) < coeff) - (T(0) > coeff);
  }

 private:
  Line3<T> _line;
};
using Plane3f = Plane<float>;

template <typename T>
class Triangle {
 public:
  Triangle(Vec3<T> const& p1, Vec3<T> const& p2, Vec3<T> const& p3)
      : _p1(p1), _p2(p2), _p3(p3) {}

  Vec3<T> project(Vec3<T> const& p) {
    Edge3<T> e0(_p1, _p2);
    Vec3<T> proj0 = e0.project(p);
    T dist0 = (proj0 - p).normsqr();
    Edge3<T> e1(_p2, _p3);
    Vec3<T> proj1 = e1.project(p);
    T dist1 = (proj1 - p).normsqr();
    Edge3<T> e2(_p3, _p1);
    Vec3<T> proj2 = e2.project(p);
    T dist2 = (proj2 - p).normsqr();
    Plane<T> e3(_p1, _p2, _p3);
    Vec3<T> proj3 = e3.project(p);
    T dist3 = (proj3 - p).normsqr();
    std::vector<T> el({dist0, dist1, dist2, dist3});
    std::size_t argmin = std::distance(
        std::min_element(std::begin(el), std::end(el)), std::begin(el));
    if (argmin == 0) {
      return proj0;
    }
    if (argmin == 1) {
      return proj1;
    }
    if (argmin == 2) {
      return proj2;
    }
    return proj3;
  }

  T distanceSquaredTo(Vec3<T> const& p) { return (project(p) - p).normsqr(); }

  T distanceTo(Vec3<T> const& p) { return std::sqrt(distanceSquaredTo(p)); }

  Intersection<Vec3<T>> intersect(Edge3<T> const& e) const {
    Intersection<Vec3<T>> intersection;
    Plane<T> plane(_p1, _p2, _p3);
    T sign1 = plane.sideOf(e.p1());
    T sign2 = plane.sideOf(e.p2());
    if (sign1 == sign2) {
      return intersection;
    }
    Plane<T> plane2(e.p2(), _p1, _p2);
    T sign3 = plane2.sideOf(e.p1());
    Plane<T> plane3(e.p2(), _p2, _p3);
    T sign4 = plane3.sideOf(e.p1());
    Plane<T> plane4(e.p2(), _p3, _p1);
    T sign5 = plane3.sideOf(e.p1());
    if (sign3 != sign4 || sign4 != sign5) {
      return intersection;
    }
    Vec3<T> n = cross(_p2 - _p1, _p3 - _p1);
    T coeff = -e.p1().dot(n - _p1) / e.p1().dot(e.p2() - e.p1());
    intersection._value = e.p1() + (e.p1() - e.p2()) * coeff;
    intersection._exist = true;
    return intersection;
  }

 private:
  Vec3<T> _p1;
  Vec3<T> _p2;
  Vec3<T> _p3;
};

using Trianglef = Triangle<float>;

template <typename T, int Dim>
class Circle {
  static_assert(Dim > 1);

 public:
  using value_type = T;
  using vector_type = Vec<T, Dim>;
  using self_type = Circle<T, Dim>;

  Circle() = default;
  Circle(vector_type const& c, float radius)
      : _center(c), _sqrRadius(radius * radius) {}

  bool intersect(self_type const& other);

  bool contains(vector_type const& pt);

  vector_type const& center() const { return _center; }
  value_type const& sqrRadius() const { return _sqrRadius; }

 private:
  vector_type _center;
  float _sqrRadius;
};
using Sphere3f = Circle<float, 3>;

template <typename T, int Dim>
class AABox {
 public:
  using vector_type = Vec<T, Dim>;
  using self_type = AABox<T, Dim>;

  AABox() = default;
  AABox(vector_type const& center, vector_type const& halfLength)
      : _center(center), _halfLength(halfLength) {}

  vector_type const& center() const { return _center; }
  vector_type const& halfLength() const { return _halfLength; }

  bool intersect(AABox const& other) { return intersection(other).exist(); }

  vector_type max() const { return _center + _halfLength; }

  vector_type min() const { return _center - _halfLength; }

  Intersection<self_type> intersection(self_type const& other) const {
    vector_type pt_max = max().apply(
        other.max(), [](float l, float r) { return std::min(l, r); });
    vector_type pt_min = min().apply(
        other.min(), [](float l, float r) { return std::max(l, r); });
    vector_type volume = (pt_max - pt_min) * 0.5f;
    if (!volume.verify([](float e) { return e >= 0.f; })) {
      return false;
    }
    return self_type((pt_min + pt_max) * 0.5f, volume);
  }

  self_type move(Tf3f const& tf) const {
    return self_type(tf * _center, _halfLength);
  }

  static self_type unit() {
    return self_type(vector_type::zero(), vector_type::all(T(1)));
  }

 private:
  vector_type _center;
  vector_type _halfLength;
};
using AABox2f = AABox<float, 2>;
using AABox3f = AABox<float, 3>;

template <typename T, int Dim>
class OrientedBox {
 public:
  using transform_type = Transform<T, Dim>;
  using vector_type = Vec<T, Dim>;
  using self_type = AABox<T, Dim>;

  OrientedBox() = default;
  OrientedBox(transform_type const& center, vector_type const& halfLength)
      : _center(center), _halfLength(halfLength) {}

  transform_type const& center() const { return _center; }
  vector_type const& halfLength() const { return _halfLength; }

  bool intersect(self_type const& other) { return intersection(other).exist(); }

  vector_type max() const { return _center + _halfLength; }

  vector_type min() const { return _center - _halfLength; }

  Intersection<self_type> intersection(self_type const& other) const {
    vector_type pt_max = max().apply(
        other.max(), [](float l, float r) { return std::min(l, r); });
    vector_type pt_min = min().apply(
        other.min(), [](float l, float r) { return std::max(l, r); });
    vector_type volume = (pt_max - pt_min) * 0.5f;
    if (!volume.verify([](float e) { return e >= 0.f; })) {
      return false;
    }
    return self_type((pt_min + pt_max) * 0.5f, volume);
  }

  self_type move(transform_type const& tf) const {
    return self_type(tf * _center, _halfLength);
  }

  static self_type unit() {
    return self_type(transform_type(), vector_type::all(T(1)));
  }

 private:
  transform_type _center;
  vector_type _halfLength;
};
using OBB2f = OrientedBox<float, 2>;
using OBB3f = OrientedBox<float, 3>;

template <typename T, int Dim>
class Polygon {
 public:
  using vec_type = Vec<T, Dim>;
  using self_type = Polygon<T, Dim>;

 private:
  std::vector<vec_type> _pts;

 public:
  std::vector<vec_type> const& pts() const { return _pts; }

  static self_type edge(vec_type const& v1, vec_type const& v2) {
    self_type s;
    s._pts.push_back(v1);
    s._pts.push_back(v2);
    return s;
  }

  static self_type triangle(vec_type const& v1, vec_type const& v2,
                            vec_type const& v3) {
    self_type s;
    s._pts.push_back(v1);
    s._pts.push_back(v2);
    s._pts.push_back(v2);
    s._pts.push_back(v3);
    s._pts.push_back(v3);
    s._pts.push_back(v1);
    return s;
  }
};
using Polygon3f = Polygon<float, 3>;

namespace Geo {
// CONTAINS
template <typename T, int D>
static bool contains(AABox<T, D> const& box, Vec<T, D> const& pt) {
  bool sup = (pt - box.min()).verify([](T val) { return val >= 0; });
  if (!sup) {
    return false;
  }
  return (box.max() - pt).verify([](T val) { return val >= 0; });
}

// LINE VS PLANE
template <typename T>
static Intersection<Vec3<T>> intersect(Plane<T> const& p, Line3<T> const& l) {
  return Geo::intersect(l, p);
}

template <typename T>
static Intersection<Vec3<T>> intersect(Line3<T> const& l, Plane<T> const& p) {
  if (l.direction().dot(p.direction()) == T(0)) {
    return false;
  }
  T s = p.direction().dot(p.origin() - l.origin()) /
        p.direction().dot(l.direction());
  return l.origin() + l.direction() * s;
}

// LINE VS AABB
template <typename T>
static Intersection<Vec3<T>> intersect(AABox<T, 3> const& p,
                                       Line3<T> const& l) {
  return Geo::intersect(l, p);
}

template <typename T>
static Intersection<Vec3<T>> intersect(Line3<T> const& l,
                                       AABox<T, 3> const& b) {
  for (int i = -1; i <= 1; i += 2) {
    for (int j = 0; j < 3; ++j) {
      auto dir = Vec3<T>(b.halfLength().x() * i, T(0), T(0));
      if (j == 1) {
        dir = Vec3<T>(T(0), b.halfLength().y() * i, T(0));
      } else if (j == 2) {
        dir = Vec3<T>(T(0), T(0), b.halfLength().z() * i);
      }
      auto center = b.center() + dir;
      auto inter = Geo::intersect(l, Plane3f(center, dir));
      if (inter.empty()) {
        return false;
      }
      auto ctn = Geo::contains(b, inter.value());
      if (ctn) {
        return inter;
      }
    }
  }
  return false;
}

}  // namespace Geo

}  // namespace arty

#include <arty/core/details/geometry_impl.hpp>

#endif  // GEOMETRY_HPP
