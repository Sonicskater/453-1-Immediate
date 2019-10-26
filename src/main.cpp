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
#include <shader_tools.hpp>
#include <glm\glm.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>


#define RAD 57.29577951
#define DegToRad(angle)	((angle)/RAD)

//using namespace std;	//Uncomment to remove "std::" in front of functions

glm::vec3 violet = { 1.0f, 0, 1.0f };
glm::vec3 blue = { 0.0f, 0.0f, 1.0f };
glm::vec3 green = { 0.0f, 1.0f, 0.0f };
glm::vec3 red = { 1.0f, 0.0f, 0.0f };
glm::vec3 orange = { 1.0f, 0.5f, 0.0f };
glm::vec3 yellow = { 1.0f, 1.0f, 0.0f };

glm::mat4 transform = glm::mat4(1.0f);
glm::mat4 view;


struct Vertex {
	Vertex() = default;
	Vertex(glm::dvec3 position, glm::vec3 color) : position(position), color(color) {}

	glm::dvec3 position;
	glm::dvec3 color;
};


struct Triangle {
	//Private members by default
	//Vec3d m_verts[3];

	
	public:
	
		glm::dvec3 a, b, c;
		glm::dvec3 m_color;
	Triangle() = default;
	Triangle(glm::dvec3 m_a, glm::dvec3 m_b, glm::dvec3 m_c, glm::dvec3 color = yellow) {
		a = m_a;
		b = m_b;
		c = m_c;
		m_color = color;
	}

	//functions to get/read value

};

void drawFaceTris(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomRight, glm::dvec3 bottomLeft, glm::vec3 color, std::vector<Triangle> &tris) {

	Triangle tl = Triangle(topRight,topLeft,bottomLeft,color);
	Triangle br = Triangle(topRight, bottomLeft, bottomRight, color);
	tris.push_back(tl);
	tris.push_back(br);

}

void drawCubeTris(double x, double y, double z, double size, std::vector<bool> faces, std::vector<Triangle> &tris) {

	glm::dvec3 tfl = { x,y,z };
	glm::dvec3 tfr = { x + size,y,z };

	glm::dvec3 tbl = { x,y,z - size };
	glm::dvec3 tbr = { x + size,y,z - size };

	glm::dvec3 bfl = { x,y - size,z };
	glm::dvec3 bfr = { x + size,y - size,z };

	glm::dvec3 bbl = { x,y - size,z - size };
	glm::dvec3 bbr = { x + size,y - size,z - size };

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
float phi = 1.0f;
float theta = 1.0f;

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
float m_width = 800;
float m_height = 800;
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
		glm::mat4 cubeInv = glm::inverse(transform);
		glm::vec4 rX = cubeInv * view * glm::vec4(0.f, -1.f, 0.f, 1.f);
		glm::vec4 rY = cubeInv * view * glm::vec4(-1.f, 0.f, 0.f, 1.f);
		transform = glm::rotate(transform, (m_width / 2 - (float)xpos) * 0.001f , glm::vec3(rX.x, rX.y, rX.z));
		transform = glm::rotate(transform, (m_height / 2 - (float)ypos)* 0.001f, glm::vec3(rY.x, rY.y, rY.z));
	}
}

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

GLfloat* perspectiveProjectionGl(float fovDegrees, float aspectRatio, float zNear, float zFar) 
{
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
	glRotatef(phi, 0, 1.0, 0);
	glRotatef(theta, 1, 0, 0);
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

	std::cout << sizeof(Vertex) << "," << sizeof(glm::dvec3) << ","  << "\n";


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

	// bind Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	clear();

	//bind vertex array and buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// configure vertex arrays
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

	bool oldPer = !per;
	bool draw = true;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 projection = glm::mat4(1.0f);


	while (!glfwWindowShouldClose(window)) {

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		if (updateMesh || vecs.size() == 0) {

			//generate mesh
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

			//Load vertices to gpu
			glBindVertexArray(0);
			glBufferData(GL_ARRAY_BUFFER, vecs.size() * sizeof(Vertex), vecs.data(), GL_STATIC_DRAW);
		}

		clear();
		glPolygonMode(GL_FRONT, GL_POLYGON);

		float radius = 10.0f;
		float camX = sin(DegToRad(phi)) * cos(DegToRad(theta)) * scale;
		float camY = sin(DegToRad(phi)) * sin(DegToRad(theta)) * scale;
		float camZ = cos(DegToRad(phi)) * scale;

		//std::cout << phi << "," << theta << "\n";

		//transform = glm::rotate(transform, glm::radians(-azimuth), glm::vec3(1, 0, 0));


		if (per) {
			projection = glm::perspective(glm::radians(90.0f), m_width / m_height, 0.1f, 100.0f);
		}
		else {
			projection = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.0f);
		}

		if (per != oldPer) {
			GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
			oldPer = per;
		}

		 view = glm::lookAt(
			glm::vec3(
				0,
				0,
				3),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
		);




		GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));



		glUseProgram(shaderProgram);

		GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		if (draw) {
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vecs.size());
		}

		glfwSwapBuffers(window);
		glfwPollEvents(); // will process event queue and carry on  
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

