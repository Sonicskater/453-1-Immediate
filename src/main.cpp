#include <array>
#include <iostream>
#include <vector>

//#include <gl/glu.h>
#include <math.h>
//
#include "glad/glad.h"

//

#include "GLFW/glfw3.h"
#include <string>
#include <vec3f.hpp>
#include <mat4f.hpp>
#include <common_matrices.hpp>
#include <shader_tools.hpp>
#include <glm\glm.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>



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

struct Vertex {
	Vertex() = default;
	Vertex(Vec3d position, Vec3d color) : position(position), color(color) {}

	Vec3d position;
	Vec3d color;
};

Vec3d add(Vec3d a, Vec3d b) { return Vec3d(a.x + b.x, a.y + b.y, a.z + b.z); }
Vec3d operator+(Vec3d a, Vec3d b) { return Vec3d(a.x + b.x, a.y + b.y,a.z + b.z); }

struct Triangle {
	//Private members by default
	//Vec3d m_verts[3];

	
public:
	Vec3d a, b, c;
	Vec3d m_color;
	Triangle() = default;
	Triangle(Vec3d m_a, Vec3d m_b, Vec3d m_c, Vec3d color= yellow) {
		a = m_a;
		b = m_b;
		c = m_c;
		m_color = color;
	}

	//functions to get/read value

};

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
		scale-=0.1f;
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		scale += 0.1f;
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
		spin -= 0.1 * (xpos - xPrev);
		//double s = fmod(spin, 360);
		spin2 -= 0.1 * (ypos - yPrev);
		if (spin2 < -89)
		{
			spin2 = -89;
		}
		else if (spin2 > 89) {
			spin2 = 89;
		}
		
		xPrev = xpos;
		yPrev = ypos;
	}
}
float m_width = 800;
float m_height = 800;
bool updateWindow = true;
void clear();
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
std::vector<Vertex> vecs = {};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	m_width = width;
	m_height = height;
	updateWindow = true;
	glViewport(0, 0, m_width, m_height);
	//draw(triangles);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	
	scale += (yoffset*.5);
	
	scale = std::max(scale,1.f);

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
	glfwSetScrollCallback(window, scroll_callback);
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

void clear()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void prepMesh(std::vector<bool>& faces, unsigned int VBO);




int main() {
   window = nullptr;

  // Close if OpenGL is not initialized
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  // Currently installed version of OpenGL
  printGlVersion();

  //Assign OpenGL version for program
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  //Create window
  window = glfwCreateWindow(m_height, m_width, "Tut03", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  setupWindow(window);

  glSetup();

  std::vector<bool> faces = { true, true, true, true, true, true };

  //setup vertex buffer
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
  //setup vertex shader
  const char* vertex_shader = R"vs(

			#version 330 core
      layout (location = 0) in vec3 position;
      layout (location = 1) in vec3 color;

      struct Data
      {
        vec3 color;
      };

      out Data data;

		uniform mat4 transform;
		uniform mat4 projection;
		uniform mat4 view;

			void main()
			{
        gl_Position =  projection  * view * transform * vec4(position, 1.);

        data.color = color;
			}

			)vs";

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertex_shader, NULL);
  glCompileShader(vertexShader);

  openGL::checkShaderCompileStatus(vertexShader);

  const char* fragment_shader = R"fs(

			#version 330 core

      struct Data
      {
        vec3 color;
      };

      in Data data;

      out vec4 fragmentColor;
			void main()
			{
        fragmentColor = vec4(data.color, 1.f);
			}

			)fs";

  //setup fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
  glCompileShader(fragmentShader);

  //setup and compile shader pipeline
  GLint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  //set current shader program
  glUseProgram(shaderProgram);

  //cleanup now uselss shader code objects.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);


  // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
  

  float vertices[] = {
-0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
 0.0f,  0.5f, 0.0f
  };

  // 1. bind Vertex Array Object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  // Rendering loop

  clear();
  prepMesh(faces, VBO);

  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
  while (!glfwWindowShouldClose(window)) {


	  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	  if (updateMesh || vecs.size() == 0) {
		  triangles = {};
		  drawSpongeTris(-0.5, 0.5, 0.5, 1, recursionLevel, faces, triangles);
		  updateMesh = false;
		  std::cout << "made thing";
		  vecs.clear();
		  for (auto t : triangles) {
			  vecs.push_back(Vertex(t.a, t.m_color));
			  vecs.push_back(Vertex(t.b, t.m_color));
			  vecs.push_back(Vertex(t.c, t.m_color));
		  }
		  std::cout << vecs.size() << "\n";
	  }
	  //glFrontFace(GL_FRONT_AND_BACK);
	  glBindVertexArray(VAO);
	  glBindBuffer(GL_ARRAY_BUFFER, VBO);

	  // 3. then set our vertex attributes pointers
	  glEnableVertexAttribArray(0);          // match layout # in shader
	  glVertexAttribPointer(                 //
		  0,                                 // attribute layout # (in shader)
		  3,                                 // number of coordinates per vertex
		  GL_DOUBLE,                          // type
		  GL_FALSE,                          // normalized?
		  sizeof(Vertex),                    // stride
		  (void*)offsetof(Vertex, position) // array buffer offset
	  );

	  glEnableVertexAttribArray(1);         // match layout # in shader
	  glVertexAttribPointer(                //
		  1,                                // attribute layout # (in shader)
		  3,                                // number of coordinates per vertex
		  GL_DOUBLE,                         // type
		  GL_FALSE,                         // normalized?
		  sizeof(Vertex),                   // stride
		  (void*)(offsetof(Vertex, color)) // array buffer offset
	  );


	  glBindVertexArray(0);

	  glBufferData(GL_ARRAY_BUFFER, vecs.size() * sizeof(Vertex), vecs.data(), GL_STATIC_DRAW);
	  clear();
	  //draw

	  //glBindVertexArray(0);

	  //std::cout << "yeet";

	  if (updateWindow) {

		  updateWindow = false;
	  }
	  glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);

	  //draw(triangles);


	  float radius = 10.0f;
	  float camX = sin(spin * 0.001) * scale;
	  float camZ = cos(spin * 0.001) * scale;

	  //applyTransform();
	  //math::Mat4f transformold = math::identity();
	  glm::mat4 transform = glm::mat4(1.0f);
	  //transform = transform * math::translateMatrix(0,0,-1);

	  transform = glm::rotate(transform, glm::radians(spin2), glm::vec3(1, 0, 0));
	  transform = glm::rotate(transform, glm::radians(spin), glm::vec3(0, 1, 0));
	  //transform = transform * math::rotateAboutXMatrix(spin2);
	  //transform = transform * math::uniformScaleMatrix(scale);
	  //transform = glm::scale(transform, glm::vec3(scale, scale, scale));
	  glm::mat4 test = glm::mat4();
	  glm::mat4 projection = glm::mat4(1.0f);
	  //projection = glm::ortho(0.0f, m_width, 0.0f, m_height, 0.1f, 100.0f);
	  if (per) {
	  projection = glm::perspective(glm::radians(90.0f), m_width / m_height, 0.1f, 100.0f);
		}// *math::perspectiveProjection(90, 1, 0.1, 1000);
		  //projection = projection * math::orthographicProjection(0, m_width, m_height, 0, 0, 1000);
		  //auto projection = perspectiveProjectionGl(45, 1, 0.1, 10);
		  glm::mat4 view = glm::lookAt(
			  glm::vec3(
				  camX,
				  0 ,
				  camZ),
			  glm::vec3(0,0,0),
			  glm::vec3(0,1,0)
		  );
		  //view = math::lookAtMatrix({ camX, 0 ,camZ}, { 0,0,0 }, { 0,1,0 });
		  //view = view * math::translateMatrix(0, 0, -10);


		  glUseProgram(shaderProgram);

		  glMatrixMode(GL_MODELVIEW);
		  GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
		  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));


		  GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		  glMatrixMode(GL_PROJECTION);

		  GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		  glBindVertexArray(VAO);
		  glDrawArrays(GL_TRIANGLES, 0, vecs.size());
		  glfwSwapBuffers(window);
		  glfwPollEvents(); // will process event queue and carry on
	  
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}

void prepMesh(std::vector<bool>& faces, unsigned int VBO)
{
	triangles = {};
	drawSpongeTris(-0.5, 0.5, 0.5, 1, recursionLevel, faces, triangles);
	updateMesh = false;
	std::cout << "made thing";
	vecs.clear();
	for (auto t : triangles) {

	}
	std::cout << vecs.size() << "\n";
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vecs.size() * sizeof(Vec3d), vecs.data(), GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vec3d), (void*)0);
	glEnableVertexAttribArray(0);
}
