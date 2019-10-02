#include <array>
#include <iostream>
#include <vector>

//#include <gl/glu.h>
#include <math.h>
//
#include "glad/glad.h"

//
#define GLFW_INCLUDE_GLU 
#include "GLFW/glfw3.h"

#define RAD 57.29577951
#define DegToRad(angle)	((angle)/RAD)

//using namespace std;	//Uncomment to remove "std::" in front of functions

float violet[] = { 1.0f, 0, 1.0f };
float blue[] = { 0.0f, 0.0f, 1.0f };
float green[] = { 0.0f, 1.0f, 0.0f };
float red[] = { 1.0f, 0.0f, 0.0f };
float orange[] = { 1.0f, 0.5f, 0.0f };
float yellow[] = { 1.0f, 1.0f, 0.0f };

struct V3f	// declaration and initializatoin of the "V3f" structure
{
	float x, y, z;
	V3f(float x1, float y1, float z1)
	{
		x = x1; y = y1; z = z1;
	}
	V3f()
	{
		x = 0; y = 0; z = 0;
	}
};

V3f operator+(V3f a, V3f b)			// vector addition
{
	return V3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

V3f operator-(V3f a) 				// changing sign of a vector
{
	return V3f(-a.x, -a.y, -a.z);
}

V3f operator-(V3f a, V3f b)			// vectior subtraction
{
	return V3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

V3f operator*(float c, V3f a)		// scalar multiplied by a vector
{
	return V3f(c * a.x, c * a.y, c * a.z);
}

V3f operator*(V3f a, float c)		// vector multiplied by a scalar
{
	return V3f(c * a.x, c * a.y, c * a.z);
}

float operator*(V3f a, V3f b)		// dot product
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

V3f operator%(V3f a, V3f b)			// cross product
{
	V3f c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = -a.x * b.z + a.z * b.x;
	c.z = a.x * b.y - a.y * b.x;
	return c;
}

float VecLength(V3f v)				// vector length
{
	return sqrt(v * v);
}

V3f VecNormalize(V3f v)				// vector normalization
{
	return (1.0 / VecLength(v) * v);
}

struct Vec3d {
	//public members by default
	Vec3d(GLdouble a[3]) : x(a[0]), y(a[1]), z(a[2]) {}
	Vec3d() = default;
	Vec3d(GLdouble x, GLdouble y, GLdouble z) : x(x), y(y) , z(z) {}
	Vec3d(float a[3]) : x(a[0]), y(a[1]), z(a[2]) {}
	GLdouble x = 0.f;	
	GLdouble y = 0.f;
	GLdouble z = 0.f;
};

Vec3d add(Vec3d a, Vec3d b) { return Vec3d(a.x + b.x, a.y + b.y, a.z + b.z); }
Vec3d operator+(Vec3d a, Vec3d b) { return Vec3d(a.x + b.x, a.y + b.y,a.z + b.z); }

class Triangle {
	//Private members by default
	Vec3d m_verts[3];	
	
public:
	Vec3d m_color;
	Triangle() = default;
	Triangle(Vec3d a, Vec3d b, Vec3d c, Vec3d color= yellow) {
		m_verts[0] = a;
		m_verts[1] = b;
		m_verts[2] = c;
		m_color = color;
	}

	//functions to get/read value
	Vec3d a() const { return m_verts[0]; }
	Vec3d b() const { return m_verts[1]; }
	Vec3d c() const { return m_verts[2]; }

	Vec3d operator[](int index) const { return m_verts[index]; }

	//functions to set value
	Vec3d &a() { return m_verts[0]; }
	Vec3d &b() { return m_verts[1]; }
	Vec3d &c() { return m_verts[2]; }

	Vec3d &operator[](int index) { return m_verts[index]; }	
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
void drawFaceTris(GLdouble topLeft[], GLdouble topRight[], GLdouble bottomRight[], GLdouble bottomLeft[], GLfloat color[], std::vector<Triangle> &tris) {


	Triangle tl = Triangle(topRight,topLeft,bottomLeft,color);
	Triangle br = Triangle(topRight, bottomLeft, bottomRight, color);
	tris.push_back(tl);
	tris.push_back(br);

}





void drawCube(double x, double y, double z, double size, std::vector<bool> faces) {
	GLdouble tfl[] = { x,y,z };
	GLdouble tfr[] = { x+size,y,z };
	
	GLdouble tbl[] = { x,y,z-size };
	GLdouble tbr[] = { x+size,y,z-size };

	GLdouble bfl[] = { x,y-size,z };
	GLdouble bfr[] = { x+size,y-size,z };

	GLdouble bbl[] = { x,y-size,z-size };
	GLdouble bbr[] = { x+size,y-size,z-size };


	

	//top face
	if (faces[0]) drawFace(tbl, tbr, tfr, tfl, green);

	//bottom
	if (faces[1]) drawFace(bbr, bbl, bfl, bfr, orange);

	//front face
	if (faces[2]) drawFace(tfl, tfr, bfr, bfl, red);

	//back face
	if (faces[3]) drawFace(tbr, tbl, bbl, bbr, yellow);

	//left face
	if (faces[4]) drawFace(tbl, tfl, bfl,bbl,blue);

	//right face
	if (faces[5]) drawFace(tfr, tbr, bbr, bfr, violet);

}

void drawCubeTris(double x, double y, double z, double size, std::vector<bool> faces, std::vector<Triangle> &tris) {
	GLdouble tfl[] = { x,y,z };
	GLdouble tfr[] = { x + size,y,z };

	GLdouble tbl[] = { x,y,z - size };
	GLdouble tbr[] = { x + size,y,z - size };

	GLdouble bfl[] = { x,y - size,z };
	GLdouble bfr[] = { x + size,y - size,z };

	GLdouble bbl[] = { x,y - size,z - size };
	GLdouble bbr[] = { x + size,y - size,z - size };

	GLdouble zero[] = { 0,0,0 };



	//top face
	if (faces[0]) drawFaceTris(tbl, tbr, tfr, tfl, green, tris);

	//bottom
	if (faces[1]) drawFaceTris(bbr, bbl, bfl, bfr, orange, tris);

	//front face
	if (faces[2]) drawFaceTris(tfl, tfr, bfr, bfl, red, tris);

	//back face
	if (faces[3]) drawFaceTris(tbr, tbl, bbl, bbr, yellow, tris);

	//left face
	if (faces[4]) drawFaceTris(tbl, tfl, bfl, bbl, blue, tris);

	//right face
	if (faces[5]) drawFaceTris(tfr, tbr, bbr, bfr, violet, tris);

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

void drawSpongeTris(double x, double y, double z, double size, int r, std::vector<bool> faces, std::vector<Triangle> &tris) {
	if (!r) {
		drawCubeTris(x, y, z, size, faces, tris);
	}
	else {
		r = r - 1;
		double s = size / 3;

		double layer_depth = z;
		//front cubes

			// top layer
		double up = y;
		drawSpongeTris(x, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + s, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + 2 * s, up, layer_depth, s, r, faces, tris);


		// middle layer
		up = y - s;
		drawSpongeTris(x + 2 * s, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x, up, layer_depth, s, r, faces, tris);

		// bottom layer
		up = y - 2 * s;
		drawSpongeTris(x, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + s, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + 2 * s, up, layer_depth, s, r, faces, tris);

		//connectors
		layer_depth = z - s;

		// top
		up = y;
		drawSpongeTris(x, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + 2 * s, up, layer_depth, s, r, faces, tris);

		// bottom
		up = y - 2 * s;
		drawSpongeTris(x, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + 2 * s, up, layer_depth, s, r, faces, tris);

		//back cubes
		layer_depth = z - 2 * s;

		// top layer
		up = y;
		drawSpongeTris(x, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + s, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + 2 * s, up, layer_depth, s, r, faces, tris);

		// mid layer
		up = y - s;
		drawSpongeTris(x + 2 * s, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x, up, layer_depth, s, r, faces, tris);

		// bottom layer
		up = y - 2 * s;
		drawSpongeTris(x, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + s, up, layer_depth, s, r, faces, tris);
		drawSpongeTris(x + 2 * s, up, layer_depth, s, r, faces, tris);
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

int recursionLevel = 0;
bool updateMesh = true;
float spin = 1.0f;
float spin2 = 1.0f;

float scale = 1.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS && recursionLevel <=3) {
		recursionLevel++;
		updateMesh = true;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS && recursionLevel>=1) {
		recursionLevel--;
		updateMesh = true;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS ) {
		spin--;
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		spin++;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		spin2--;
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		spin2++;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		scale+=0.1f;
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		scale -= 0.1f;
	}
		
}
double xPrev = 0;
double yPrev = 0;
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (xPrev == 0 || yPrev == 0) {
		xPrev = xpos;
		yPrev = ypos;
		return;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
		return;
	}
	else {
		spin -= xpos - xPrev;
		double s = fmod(spin, 360);
		spin2 -= (ypos - yPrev);
		if (spin2 < -90)
		{
			spin2 = -90;
		}
		else if (spin2 > 90) {
			spin2 = 90;
		}
		
		xPrev = xpos;
		yPrev = ypos;
	}
}
int m_width = 800;
int m_height = 800;
bool updateWindow = true;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	m_width = width;
	m_height = height;
	updateWindow = true;
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
  window = glfwCreateWindow(m_height, m_width, "Tut03", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  //Set context to window
  glfwMakeContextCurrent(window);
  
  //Load up 
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glfwSwapInterval(1); // vsync
  
  

  glTranslatef(0, 0, 0);
  //
  std::vector<bool> faces = { true, true, true, true, true, true };
  std::vector<Triangle> tris = {};
  //drawSpongeTris(-2.5, 2.5, 2.5, 5, recursionLevel, faces, tris);
  std::cout << tris.size() << "\n";
  // Rendering loop
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  while (!glfwWindowShouldClose(window)) {

	  if (updateMesh) {
		  tris = {};
		  drawSpongeTris(-0.5, 0.5, 0.5, 1, recursionLevel, faces, tris);
		  updateMesh = false;
	  }
	  //spin = spin - .5; // inc for spin
	  if (spin < 360){
		  //spin = spin + 360;
	  }
	  if (updateWindow) {
		  glViewport(0, 0, m_width, m_height);
		  updateWindow = false;
	  }
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LESS);
	//glu::gluLookAt(0,0,-5,0,0,0,0,1,0);
	glBegin(GL_TRIANGLES);
	
    for (auto t : tris) {
		
		glColor3d(t.m_color.x, t.m_color.y, t.m_color.z);
		  glVertex3d(t[0].x, t[0].y, t[0].z);
		  glVertex3d(t[1].x, t[1].y, t[1].z);
		  glVertex3d(t[2].x, t[2].y, t[2].z);
    }
	
	//glBegin(GL_QUADS);


	//drawSponge(-2.5, 2.5, 2.5, 5, 5, faces);
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPushMatrix();
	//glRotatef(90, 1, 0, 0.00);
	glRotatef(spin, 0,1.0,0);
	glRotatef(spin2, 1, 0, 0);
	glScaled(scale, scale, scale);
	//glRotatef(spin3, 0, 0, 1);
	//Swap current scene with next scene
    glfwSwapBuffers(window);

    glfwPollEvents(); // will process event queue and carry on
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
