#pragma once

#include "vec3f.hpp"

namespace geometry {

class Triangle {
public:
  Triangle() = default;
  Triangle(math::Vec3f a, math::Vec3f b, math::Vec3f c);

  math::Vec3f const &a() const;
  math::Vec3f const &b() const;
  math::Vec3f const &c() const;

  math::Vec3f const &operator[](int index) const;

  math::Vec3f &a();
  math::Vec3f &b();
  math::Vec3f &c();

  math::Vec3f &operator[](int index);

private:
  math::Vec3f m_vertices[3];
};

Triangle scale(Triangle t, float s);
Triangle translate(Triangle t, math::Vec3f v);

} // namespace geometry
