#pragma once

#include <vector>

#include "triangle.hpp"
#include "vec3f.hpp"

namespace fractal {

struct SierpinskiOutput {
  geometry::Triangle a, b, c;
};

SierpinskiOutput SierpinskiStep(geometry::Triangle t);

std::vector<geometry::Triangle> SierpinskiFractal(geometry::Triangle base,
                                                  int depth);
} // namespace fractal
