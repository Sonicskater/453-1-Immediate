#include "sierpinski.hpp"

// don't put 'using namespace' in .cpp files
// exceptions (in main.cpp or function definitions)
using namespace geometry;
using namespace math;

namespace fractal {

SierpinskiOutput SierpinskiStep(Triangle t) {
  SierpinskiOutput triangles;

  //TODO 
  
  return triangles;
}

std::vector<Triangle> SierpinskiFractal(Triangle base, int depth) {
  std::vector<Triangle> triangles, tmp;
  triangles.push_back(base);

  //TODO

  return triangles;
}

} // namespace fractal
