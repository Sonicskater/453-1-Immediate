#include <array>
#include <iostream>
#include <vector>

//
#include "glad/glad.h"
//
#include "GLFW/glfw3.h"

//using namespace std;	//Uncomment to remove "std::" in front of functions

struct Vec2f {
	//public members by default
	Vec2f() = default;
	Vec2f(float x, float y) : x(x), y(y) {}
	
	float x = 0.f;	
	float y = 0.f;
};

Vec2f add(Vec2f a, Vec2f b) { return Vec2f(a.x + b.x, a.y + b.y); }
Vec2f operator+(Vec2f a, Vec2f b) { return Vec2f(a.x + b.x, a.y + b.y); }

std::ostream &operator<<(std::ostream &out, Vec2f const &a) {
	return out << a.x << ' ' << a.y;
}

class Triangle {
	//Private members by default
	Vec2f m_verts[3];	

public:
	Triangle() = default;
	Triangle(Vec2f a, Vec2f b, Vec2f c) {
		m_verts[0] = a;
		m_verts[1] = b;
		m_verts[2] = c;
	}

	//functions to get/read value
	Vec2f a() const { return m_verts[0]; }
	Vec2f b() const { return m_verts[1]; }
	Vec2f c() const { return m_verts[2]; }

	Vec2f operator[](int index) const { return m_verts[index]; }

	//functions to set value
	Vec2f &a() { return m_verts[0]; }
	Vec2f &b() { return m_verts[1]; }
	Vec2f &c() { return m_verts[2]; }

	Vec2f &operator[](int index) { return m_verts[index]; }	
};


int main() {
  GLFWwindow *window = nullptr;

  // Close if OpenGL is not initialized
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  
  // Currently installed version of OpenGL
  int glfwMajor, glfwMinor, glfwRevision;
  glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
  std::cout << "GLFW Version: " << glfwMajor << "." << glfwMinor << "."
       << glfwRevision << std::endl;

  //Assign OpenGL version for program
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  //Create window
  window = glfwCreateWindow(800, 800, "Tut03", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  //Set context to window
  glfwMakeContextCurrent(window);
  
  //Load up 
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glfwSwapInterval(1); // vsync
  
  //Set data
  
  // NDC [-1, 1]
  Triangle baseTriangle(Vec2f(-1, -1), // explicit
                          {1, -1},       // implied
                          {0, 1});
                          
  std::vector<Triangle> tris;
  tris.push_back(baseTriangle);

  // Rendering loop
  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    for(int i = 0; i < tris.size(); i++) {
		Triangle t = tris[i];
		glVertex2f(t[0].x, t[0].y);
		glVertex2f(t[1].x, t[1].y);
		glVertex2f(t[2].x, t[2].y);
	}
    
    /* Another way to iterate through elements in list
    for (auto t : tris) {
      glVertex2f(t[0].x, t[0].y);
      glVertex2f(t[1].x, t[1].y);
      glVertex2f(t[2].x, t[2].y);
    }
    * */
    
    glEnd();

	//Swap current scene with next scene
    glfwSwapBuffers(window);

    glfwPollEvents(); // will process event queue and carry on
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
