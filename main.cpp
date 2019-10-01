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

void drawTri(Triangle t) {
	glVertex2f(t[0].x, t[0].y);
	glVertex2f(t[1].x, t[1].y);
	glVertex2f(t[2].x, t[2].y);
}
void idk() {
	glBegin(GL_QUADS);
	// Top face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, 1.0f, -1.0f);  // Top-right of top face
	glVertex3f(-1.0f, 1.0f, -1.0f);  // Top-left of top face
	glVertex3f(-1.0f, 1.0f, 1.0f);  // Bottom-left of top face
	glVertex3f(1.0f, 1.0f, 1.0f);  // Bottom-right of top face

	// Bottom face
	glColor3f(1.0f, 0.5f, 0.0f); // Orange
	glVertex3f(1.0f, -1.0f, -1.0f); // Top-right of bottom face
	glVertex3f(-1.0f, -1.0f, -1.0f); // Top-left of bottom face
	glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom-left of bottom face
	glVertex3f(1.0f, -1.0f, 1.0f); // Bottom-right of bottom face

	// Front face
	glColor3f(1.0f, 0.0f, 0.0f);  // Red
	glVertex3f(1.0f, 1.0f, 1.0f);  // Top-Right of front face
	glVertex3f(-1.0f, 1.0f, 1.0f);  // Top-left of front face
	glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom-left of front face
	glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom-right of front face

	// Back face
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f); // Bottom-Left of back face
	glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom-Right of back face
	glVertex3f(-1.0f, 1.0f, -1.0f); // Top-Right of back face
	glVertex3f(1.0f, 1.0f, -1.0f); // Top-Left of back face

	// Left face
	glColor3f(0.0f, 0.0f, 1.0f);  // Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);  // Top-Right of left face
	glVertex3f(-1.0f, 1.0f, -1.0f);  // Top-Left of left face
	glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom-Left of left face
	glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom-Right of left face

	// Right face
	glColor3f(1.0f, 0.0f, 1.0f);  // Violet
	glVertex3f(1.0f, 1.0f, 1.0f);  // Top-Right of left face
	glVertex3f(1.0f, 1.0f, -1.0f);  // Top-Left of left face
	glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom-Left of left face
	glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom-Right of left face
	glEnd();
}
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
  //tris.push_back(baseTriangle);
  tris.push_back(Triangle(Vec2f(-1, 1), {-1,-1}, {0,1}));
  
  float spin = 0.1f;
  glScaled(.1,.1,.1);
  glTranslatef(0, 0,0.0f);
  glRotatef(-10, 1, 0, 0.00);
  
  // Rendering loop
  while (!glfwWindowShouldClose(window)) {

	  //spin = spin - .5; // inc for spin
	  if (spin < 360)
	  {
		  //spin = spin + 360;
	  }
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < tris.size(); i++) {
		Triangle t = tris[i];
		drawTri(t);
	}


    
    /* Another way to iterate through elements in list
    for (auto t : tris) {
      glVertex2f(t[0].x, t[0].y);
      glVertex2f(t[1].x, t[1].y);
      glVertex2f(t[2].x, t[2].y);
    }
    * */
    
    glEnd();
	idk();
	
	glRotatef(spin, 1.0,1.0,1.0);
	//Swap current scene with next scene
    glfwSwapBuffers(window);

    glfwPollEvents(); // will process event queue and carry on
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
