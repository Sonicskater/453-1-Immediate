#include <array>
#include <iostream>
#include <vector>

//#include <gl/glu.h>
#include <math.h>
//
#include "glad/glad.h"

//

#include "GLFW/glfw3.h"
#include <cmath>
//#include "common_matrices.cpp"


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

void drawFaceTris(GLdouble topLeft[], GLdouble topRight[], GLdouble bottomRight[], GLdouble bottomLeft[], GLfloat color[], std::vector<Triangle> &tris) {


	Triangle tl = Triangle(topRight,topLeft,bottomLeft,color);
	Triangle br = Triangle(topRight, bottomLeft, bottomRight, color);
	tris.push_back(tl);
	tris.push_back(br);

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

int recursionLevel = 0;
bool updateMesh = true;
float spin = 1.0f;
float spin2 = 1.0f;

float scale = 1.0f;

bool per = false;
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
		per = !per;
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
void draw(std::vector<Triangle> tris);

GLfloat* symmetricFrustumProjectionGl(float r, float t, float n, float f) {
	float a00 = n / r;
	float a11 = n / t;
	float a22 = -(f + n) / (f - n);
	float a23 = -2.f * f * n / (f - n);

	GLfloat symmetricFrustum[] = {
		a00, 0.f, 0.f,  0.f, // 0 1 2 3
		0.f, a11, 0.f,  0.f, // 4 5 6 7
		0.f, 0.f, a22,  a23, // 8 9 10 11
		0.f, 0.f, -1.f, 0.f  // 12 13 14 15
	};
	return symmetricFrustum;
}
GLfloat* perspectiveProjectionGl(float fovDegrees, float aspectRatio, float zNear,
	float zFar) {
	float top = std::tan(fovDegrees * (M_PI / 180.f) * (1.f / 2.f)) * zNear;
	float right = top * aspectRatio;

	return symmetricFrustumProjectionGl(right, top, zNear, zFar);
}
std::vector<Triangle> triangles = {};
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	m_width = width;
	m_height = height;
	updateWindow = true;
	glViewport(0, 0, m_width, m_height);
	draw(triangles);
}

void setupWindow(GLFWwindow *window) {
	//Set context to window
	glfwMakeContextCurrent(window);

	//Load up 
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(1); // vsync

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}
GLFWwindow* window;
void printGlVersion() {
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
	std::cout << "GLFW Version: " << glfwMajor << "." << glfwMinor << "." << glfwRevision << std::endl;
}

void glSetup() {

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
}

void glPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW;
	GLdouble fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;

	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void applyTransform() {


	glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();
	//glPushMatrix();
	glLoadIdentity();
	if (per) {
		glTranslatef(0, 0, -2);
	}
	glRotatef(spin, 0, 1.0, 0);
	glRotatef(spin2, 1, 0, 0);
	glScaled(scale, scale, scale);
	glMatrixMode(GL_PROJECTION);
	auto d = perspectiveProjectionGl(90, m_width / m_height, 0.1, 15);
	//const GLfloat* g = d;
	//glPopMatrix();
	//glPushMatrix();
	glLoadIdentity();
	if (per) {
		glPerspective(90, m_width / m_height, 0.1, 5);
	}
	//draw(triangles);
	glMatrixMode(GL_MODELVIEW);
	
	
}

void draw(std::vector<Triangle> tris) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	for (auto t : tris) {

		glColor3d(t.m_color.x, t.m_color.y, t.m_color.z);
			glVertex3d(t[0].x, t[0].y, t[0].z);
			glVertex3d(t[1].x, t[1].y, t[1].z);
			glVertex3d(t[2].x, t[2].y, t[2].z);
		
	}
	glEnd();


	//Swap current scene with next scene
	glfwSwapBuffers(window);

}


int main() {
   window = nullptr;

  // Close if OpenGL is not initialized
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  // Currently installed version of OpenGL
  printGlVersion();

  //Assign OpenGL version for program
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  //Create window
  window = glfwCreateWindow(m_height, m_width, "Tut03", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  setupWindow(window);

  glSetup();

  std::vector<bool> faces = { true, true, true, true, true, true };


  // Rendering loop
  while (!glfwWindowShouldClose(window)) {

	  if (updateMesh) {
		  triangles = {};
		  drawSpongeTris(-0.5, 0.5, 0.5, 1, recursionLevel, faces, triangles);
		  updateMesh = false;
	  }
	  if (updateWindow) {

		  updateWindow = false;
	  }
	  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	  draw(triangles);
	  applyTransform();


      glfwPollEvents(); // will process event queue and carry on
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
