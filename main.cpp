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
void drawCubeSimple() {
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
void drawFace(GLdouble topLeft[], GLdouble topRight[], GLdouble bottomRight[], GLdouble bottomLeft[], GLfloat color[]){

	glColor3fv(color);

	glVertex3dv(topRight);
	glVertex3dv(topLeft);
	glVertex3dv(bottomLeft);
	glVertex3dv(bottomRight);


}

float violet[] = { 1.0f, 0, 1.0f };
float blue[] = { 0.0f, 0.0f, 1.0f };
float green[] = { 0.0f, 1.0f, 0.0f };
float red[] = { 1.0f, 0.0f, 0.0f };
float orange[] = { 1.0f, 0.5f, 0.0f };
float yellow[] = { 1.0f, 1.0f, 0.0f };



void drawCube(double x, double y, double z, double size, std::vector<bool> faces) {
	GLdouble tfl[] = { x,y,z };
	GLdouble tfr[] = { x+size,y,z };
	
	GLdouble tbl[] = { x,y,z-size };
	GLdouble tbr[] = { x+size,y,z-size };

	GLdouble bfl[] = { x,y-size,z };
	GLdouble bfr[] = { x+size,y-size,z };

	GLdouble bbl[] = { x,y-size,z-size };
	GLdouble bbr[] = { x+size,y-size,z-size };


	glBegin(GL_QUADS);

	//top face
	if (faces[0]) drawFace(tbl, tbr, tfr, tfl, green);

	//bottom
	if (faces[1]) drawFace(bbl, bbr, bfr, bfl, orange);

	//front face
	if (faces[2]) drawFace(tfl, tfr, bfr, bfl, red);

	//back face
	if (faces[3]) drawFace(tbr, tbl, bbl, bbr, yellow);

	//left face
	if (faces[4]) drawFace(tbl, tfl, bfl,bbl,blue);

	//right face
	if (faces[5]) drawFace(tfr, tbr, bbr, bfr, violet);

	glEnd();
	
}

void drawSponge(double x, double y, double z, double size, int r, std::vector<bool> faces) {
	if (!r) {
		drawCube(x, y, z, size,faces);
	}
	else {
		r = r - 1;
		double s = size / 3;

		double layer_depth = z;
		//front cubes
		
			// top layer
			double up = y;
			drawSponge(x, up, layer_depth, s, r, faces);
			drawSponge(x+s, up, layer_depth, s, r, faces);
			drawSponge(x+2*s, up, layer_depth, s, r, faces);

			
			// middle layer
			up = y - s;
			drawSponge(x+2*s, up, layer_depth, s, r, faces);
			drawSponge(x, up, layer_depth, s, r, faces);

			// bottom layer
			up = y - 2 * s;
			drawSponge(x, up, layer_depth, s, r, faces);
			drawSponge(x+s, up, layer_depth, s, r, faces);
			drawSponge(x+2*s, up, layer_depth, s, r, faces);

		//connectors
		layer_depth = z - s;

			// top
			up = y;
			drawSponge(x, up, layer_depth, s, r, faces);
			drawSponge(x+2*s, up, layer_depth, s, r, faces);

			// bottom
			up = y - 2 * s;
			drawSponge(x, up, layer_depth, s, r, faces);
			drawSponge(x+2*s, up, layer_depth, s, r, faces);

		//back cubes
		layer_depth = z - 2 * s;

			// top layer
			up = y;
			drawSponge(x, up, layer_depth, s, r, faces);
			drawSponge(x+s, up, layer_depth, s, r, faces);
			drawSponge(x+2*s, up, layer_depth, s, r, faces);

			// mid layer
			up = y - s;
			drawSponge(x+2*s, up, layer_depth, s, r, faces);
			drawSponge(x, up, layer_depth, s, r, faces);

			// bottom layer
			up = y - 2 * s;
			drawSponge(x, up, layer_depth, s, r, faces);
			drawSponge(x + s, up, layer_depth, s, r, faces);
			drawSponge(x + 2 * s, up, layer_depth, s, r, faces);
	}
}

void drawSpongeCulled(double x, double y, double z, double size, int r, std::vector<bool> faces) {
	if (!r) {
		drawCube(x, y, z, size, faces);
	}
	else {

		r = r - 1;
		double s = size / 3;
		double layer_depth = z;
		//front cubes

			// top layer
			double up = y;
			std::vector<bool> f1 = {faces[0]&&true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x, up, layer_depth, s, r, f1);
			std::vector<bool> f2 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + s, up, layer_depth, s, r, f2);
			std::vector<bool> f3 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + 2 * s, up, layer_depth, s, r, f3);


			// middle layer
			up = y - s;
			std::vector<bool> f4 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + 2 * s, up, layer_depth, s, r, f4);
			std::vector<bool> f5 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x, up, layer_depth, s, r, f5);

			// bottom layer
			up = y - 2 * s;
			std::vector<bool> f6 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x, up, layer_depth, s, r, f6);
			std::vector<bool> f7 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + s, up, layer_depth, s, r, f7);
			std::vector<bool> f8 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + 2 * s, up, layer_depth, s, r, f8);

		//connectors
		layer_depth = z - s;

			// top
			up = y;

			std::vector<bool> f10 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x, up, layer_depth, s, r, f10);
			

			std::vector<bool> f11 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + 2 * s, up, layer_depth, s, r, f11);

			// bottom
			up = y - 2 * s;

			std::vector<bool> f12 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x, up, layer_depth, s, r, f12);

			std::vector<bool> f13 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + 2 * s, up, layer_depth, s, r, f13);

		//back cubes
		layer_depth = z - 2 * s;

			// top layer
			up = y;

			std::vector<bool> f14 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x, up, layer_depth, s, r, f14);

			std::vector<bool> f15 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + s, up, layer_depth, s, r, f15);

			std::vector<bool> f16 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + 2 * s, up, layer_depth, s, r, f16);

			// mid layer
			up = y - s;

			std::vector<bool> f17 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + 2 * s, up, layer_depth, s, r, f17);

			std::vector<bool> f18 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x, up, layer_depth, s, r, f18);

			// bottom layer
			up = y - 2 * s;

			std::vector<bool> f19 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x, up, layer_depth, s, r, f19);

			std::vector<bool> f20 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + s, up, layer_depth, s, r, f20);

			std::vector<bool> f21 = { faces[0] && true, faces[1] && true, faces[2] && true, faces[3] && true, faces[4] && true, faces[5] && true };
			drawSpongeCulled(x + 2 * s, up, layer_depth, s, r, f21);
	}
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
  glScaled(.2,.2,.2);
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
		//drawTri(t);
	}


    
    /* Another way to iterate through elements in list
    for (auto t : tris) {
      glVertex2f(t[0].x, t[0].y);
      glVertex2f(t[1].x, t[1].y);
      glVertex2f(t[2].x, t[2].y);
    }
    * */
    
    glEnd();
	//drawCubeSimple();
	std::vector<bool> faces = { true, false, false, false, false, false };
	drawSponge(-2.5, 2.5, 2.5, 5, 1, faces);
	
	glRotatef(spin, 1.0,1.0,1.0);
	//Swap current scene with next scene
    glfwSwapBuffers(window);

    glfwPollEvents(); // will process event queue and carry on
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
