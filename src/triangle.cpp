//
//  triangle.cpp
//  Triangle
//  CollisionExample
//
//  Created by Andrew Owens
//  Copyright (c) 2018 ___ALGORITHMIC_BOTANY___. All rights reserved.
//

#include "triangle.hpp"

using namespace math;

namespace geometry {

Triangle::Triangle(Vec3f a, Vec3f b, Vec3f c) : m_vertices{a, b, c} {}

math::Vec3f const &Triangle::a() const { return m_vertices[0]; }
math::Vec3f const &Triangle::b() const { return m_vertices[1]; }
math::Vec3f const &Triangle::c() const { return m_vertices[2]; }

math::Vec3f const &Triangle::operator[](int index) const {
  return m_vertices[index];
}

math::Vec3f &Triangle::a() { return m_vertices[0]; };
math::Vec3f &Triangle::b() { return m_vertices[1]; };
math::Vec3f &Triangle::c() { return m_vertices[2]; };

math::Vec3f &Triangle::operator[](int index) { return m_vertices[index]; }

// Free functions (also part of the interface of Triangle)

Triangle scale(Triangle t, float s) {
  t.a() *= s;
  t.b() *= s;
  t.c() *= s;

  return t;
}

Triangle translate(Triangle t, Vec3f v) {
  t.a() += v;
  t.b() += v;
  t.c() += v;

  return t;
}

} // namespace geometry
