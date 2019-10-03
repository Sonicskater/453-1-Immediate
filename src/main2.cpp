#include <array>
#include <iostream>
#include <vector>

// glad beforw glfw
#include "glad/glad.h"
//
#include "GLFW/glfw3.h"

#include "common_matrices.hpp"
#include "cube.hpp"
#include "mat4f.hpp"
#include "shader_tools.hpp"
#include "sierpinski.hpp"
#include "triangle.hpp"
#include "vec3f.hpp"

using namespace math;
using namespace geometry;

// GLOBAL Variables
GLuint g_vaoID = 0;
GLuint g_vertexBufferID = 0;
GLuint g_programID = 0;
math::Mat4f g_M = math::identity();
int g_totalVertices = 0;
//

// function declaration
using namespace std;

void setFrameBufferSize(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void setKeyboard(GLFWwindow *window, int key, int scancode, int action,
								 int mods) {
	if (GLFW_KEY_LEFT == key) {
		if (GLFW_PRESS == action){}
	} else if (GLFW_KEY_RIGHT == key) {
		if (GLFW_PRESS == action){}
	}
}

// user defined alias
GLuint createShaderProgram() {
	auto vertexShaderSource = //
			R"vs(

			#version 330 core
      layout (location = 0) in vec3 position;
      layout (location = 1) in vec3 color;

      struct Data
      {
        vec3 color;
      };

      out Data data;

      uniform mat4 MVP;

			void main()
			{
        gl_Position = MVP * vec4(position, 1.);

        data.color = color;
			}

			)vs";

	auto fragmentShaderSource = //
			R"fs(

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

	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &vertexShaderSource, NULL);
	glCompileShader(vsID);

	openGL::checkShaderCompileStatus(vsID);

	GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fsID);

	openGL::checkShaderCompileStatus(fsID);

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vsID);
	glAttachShader(programID, fsID);
	glLinkProgram(programID);

	openGL::checkProgramLinkStatus(programID);

	glDetachShader(programID, vsID);
	glDetachShader(programID, fsID);

	glDeleteShader(vsID);
	glDeleteShader(fsID);

	return programID;
}

bool generateOpenGL_IDs() {
	g_programID = createShaderProgram();

	glGenVertexArrays(1, &g_vaoID);
	glGenBuffers(1, &g_vertexBufferID);

	return true;
}

void deleteOpenGL_IDs() {
	glDeleteVertexArrays(1, &g_vaoID);
	glDeleteBuffers(1, &g_vertexBufferID);
	glDeleteProgram(g_programID);

	g_vaoID = 0;
	g_vertexBufferID = 0;
	g_programID = 0;
}

namespace openGL {

struct Vertex {
	Vertex() = default;
	Vertex(Vec3f position, Vec3f color) : position(position), color(color) {}

	Vec3f position;
	Vec3f color;
};

} // namespace openGL


bool loadSierpinskiGeometryToGPU(std::vector<Triangle> const &tris) {

	// each vertex has a (byte) layout
	// |------------------|
	// | Vec3f (position) |
	// |------------------|
	// | Vec3f (color)    |
	// |------------------|
	//
	// The contiguous memory of the std::vector vertices is laid out
	//
	// .----------.
	// | position |
	// |----------|  > vertex 0
	// | color		|
	// |----------|
	// | position |
	// |----------|  > vertex 1
	// | color		|
	// |----------|
	// 		...

	using namespace openGL;
	std::vector<Vertex> vertices;
	vertices.reserve(tris.size() * 3);

	for (int i = 0; i < tris.size(); ++i) {
		auto const &t = tris[i];

		Vertex vert;
		// vertex a position, red color
		vert.position = t.a();
		vert.color = Vec3f(1.f, 0.f, 0.f);
		vertices.push_back(vert);

		// vertex b position, green color
		vert.position = t.b();
		vert.color = Vec3f(0.f, 1.f, 0.f);
		vertices.push_back(vert);

		// vertex c position, blue color
		vert.position = t.c();
		vert.color = Vec3f(0.f, 0.f, 1.f);
		vertices.push_back(vert);
	}

	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferID);
	glBufferData(
			GL_ARRAY_BUFFER,                  // destination
			sizeof(Vertex) * vertices.size(), // size (bytes) of memcopy to GPU
			vertices.data(),                  // pointer to data (contiguous)
			GL_STATIC_DRAW                    // usage patter of the GPU buffer
	);
	
	g_totalVertices = vertices.size();

	return true;
}

void setupVAO() {
	using namespace openGL;

	glBindVertexArray(g_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferID);

	// .----------.
	// | position | 3 floats (3 * 4 bytes = 12 bytes)
	// |----------|
	// | color    | 3 floats (3 * 4 bytes = 12 bytes)
	// |----------|
	// | position |
	// |----------|
	// | color    |
	// |----------|
	//
	// sizeof(Vertex) = 24 bytes
	// offsetof(Vertex, position) = 0 bytes
	// offsetof(Vertex, color) = 12 bytes

	// set up position input into vertex shader
	glEnableVertexAttribArray(0);          // match layout # in shader
	glVertexAttribPointer(                 //
			0,                                 // attribute layout # (in shader)
			3,                                 // number of coordinates per vertex
			GL_FLOAT,                          // type
			GL_FALSE,                          // normalized?
			sizeof(Vertex),                    // stride
			(void *)offsetof(Vertex, position) // array buffer offset
	);

	// set up color input into vertex shader
	glEnableVertexAttribArray(1);         // match layout # in shader
	glVertexAttribPointer(                //
			1,                                // attribute layout # (in shader)
			3,                                // number of coordinates per vertex
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			sizeof(Vertex),                   // stride
			(void *)(offsetof(Vertex, color)) // array buffer offset
	);

	glBindVertexArray(0);
}

int main2() {
	GLFWwindow *window = nullptr;

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// print version
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
	cout << "GLFW Version: " << glfwMajor << "." << glfwMinor << "."
			 << glfwRevision << endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(1000,              // width
														1000,              // height
														"Tut05_OpenGL3.3", // title
														NULL,              // unused
														NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(1); // vsync
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	// setup callbacks
	glfwSetFramebufferSizeCallback(window, setFrameBufferSize);
	glfwSetKeyCallback(window, setKeyboard);

	// setup
	generateOpenGL_IDs();
	setupVAO();

	// Triangle in Normalized Ddevic Coordinates (NDC) [-1,1]x[-1,1]
	Triangle baseTriangle({-1.f, -1.f, 0.f}, //
												{1.f, -1.f, 0.f},  //
												{0.f, 1.f, 0.f});

	int frame = 0;
	int depth = 0;

	g_M = rotateAboutXMatrix(5) * uniformScaleMatrix(0.5);
	
	auto tris = fractal::SierpinskiFractal(baseTriangle, depth);
	loadSierpinskiGeometryToGPU(tris);

	while (!glfwWindowShouldClose(window)) {
				
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		GLint mvpID = glGetUniformLocation(g_programID, "MVP");
		glUniformMatrix4fv(mvpID, 1, true, g_M.data());

		glUseProgram(g_programID);   // binds the shader program for use
		glBindVertexArray(g_vaoID);  // binds vertex buffer
		glDrawArrays(GL_TRIANGLES,   // type of drawing (rendered to back buffer)
								 0,              // offset into buffer
								 g_totalVertices // number of vertices in buffer
		);

		glfwSwapBuffers(window); // swaps back buffer to front for drawing to screen
		glfwPollEvents();        // will process event queue and carry on
	}

	// cleaup window, and glfw before exit
	deleteOpenGL_IDs();
	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
