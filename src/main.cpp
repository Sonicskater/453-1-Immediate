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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "vec3f.hpp"
#include "vec2f.hpp"

#include "buffer_object.hpp"
#include "vertex_array_object.hpp"
#include "vbo_tools.hpp"
#include <program.hpp>
#include <shader_file_io.hpp>
#include <texture.hpp>
#include <image.hpp>



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

ImGuiIO* io;

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
	//Bail and ignore input if
	if (io->WantCaptureMouse) {
		return;
	}
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
		//transform = glm::rotate(transform, (m_width / 2 - (float)xpos) * 0.001f , glm::vec3(rX.x, rX.y, rX.z));
		//transform = glm::rotate(transform, (m_height / 2 - (float)ypos)* 0.001f, glm::vec3(rY.x, rY.y, rY.z));
	}
}

bool updateWindow = true;
void clear();
void loadMesh(int mesh, geometry::OBJMesh& meshData, GLuint& totalIndices, opengl::VertexArrayObject& vao, opengl::BufferObject& indexBuffer, opengl::BufferObject& vertexBuffer);
int main();

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

void clear()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// user defined alias
opengl::Program createShaderProgram(std::string const& vertexShaderFile,
	std::string const& fragmentShaderFile) {
	using namespace opengl;
	auto vertexShaderSource = loadShaderStringFromFile(vertexShaderFile);
	auto fragmentShaderSource = loadShaderStringFromFile(fragmentShaderFile);

	std::cout << "[Log] compiling program " << vertexShaderFile << ' '
		<< fragmentShaderFile << '\n';
	return opengl::makeProgram(vertexShaderSource, fragmentShaderSource);
}

opengl::Program createShaderProgram(std::string const& vertexShaderFile,
	std::string const& geometryShaderFile,
	std::string const& fragmentShaderFile) {
	using namespace opengl;
	auto vertexShaderSource = loadShaderStringFromFile(vertexShaderFile);
	auto geometryShaderSource = loadShaderStringFromFile(geometryShaderFile);
	auto fragmentShaderSource = loadShaderStringFromFile(fragmentShaderFile);

	std::cout << "[Log] compiling program " << vertexShaderFile << ' '
		<< geometryShaderFile << ' ' << fragmentShaderFile << '\n';
	return opengl::makeProgram(vertexShaderSource, geometryShaderSource,
		fragmentShaderSource);
}

bool wf = true;
bool pers = true;
bool tex = false;

void toggleWF() {
	wf = !wf;
	std::cout << "toggled wf" << "\n";
}

void toggleTex() {
	if (tex) {
		tex = 0.f;
	}
	else {
		tex = 1.f;
	}
	std::cout << "toggled wf" << "\n";
}

void togglePer() {
	pers = !pers;
	std::cout << "toggled per" << "\n";
}
float scaleFactor;

char* texPath = "";



void loadMesh(int mesh, geometry::OBJMesh& meshData, GLuint& totalIndices, opengl::VertexArrayObject& vao, opengl::BufferObject& indexBuffer, opengl::BufferObject& vertexBuffer, opengl::Texture& texture)
{
	std::string filePath = "";

	if (updateMesh) {
		switch (mesh)
		{
		case 1:
			filePath = "./ico_sphere_w_normals.obj";
			scaleFactor = 1.f;
			break;
		case 2:
			filePath = "./spot_triangulated.obj";
			texPath = "./spot_texture.png";
			scaleFactor = 1.f;
			break;
		case 3:
			filePath = "./teapot_triangulated.obj";
			scaleFactor = 0.5f;
			break;
		case 4:
			filePath = "./Nefertiti_Low.obj";
			texPath = "./COLOR_Low.jpg";
			scaleFactor = 0.2f;
			break;
		case 5:
			filePath = "./Nefertiti_High.obj";
			texPath = "./COLOR_High.jpg";
			scaleFactor = 0.2f;
			break;

		}
	}

	if (!geometry::loadOBJMeshFromFile(filePath, meshData)) {
		std::cerr << "[Error] Cannot load .obj file located at: " << filePath;
	}

	//std::cout << meshData.triangles.size() << "," << meshData.vertices.size() << "," << meshData.normals.size() << "\n";

	//Calculate vertex normals if they are not included in .obj file
	if (meshData.normals.size() == 0) {

		std::cout << "Missing Normals\n";

		geometry::Normals normals = geometry::calculateVertexNormals(meshData.triangles, meshData.vertices);

		//std::cout << normals.size() << "\n";

		//meshData.normals = normals;
		opengl::VBOData_VerticesNormals vboData = opengl::makeConsistentVertexNormalIndices(meshData, normals);//, normals);

		//std::cout << meshData.normals.size() << "\n";

		totalIndices =
			opengl::setup_vao_and_buffers(vao, indexBuffer, vertexBuffer, vboData);

		//std::cout << totalIndices;
	}
	//Pass in vertex normals if they are already included in .obj file
	else {
		if (meshData.textureCoords.empty()) {
			std::cout << "Normals Present \n";
			opengl::VBOData_VerticesNormals vboData = opengl::makeConsistentVertexNormalIndices(meshData);

			totalIndices =
				opengl::setup_vao_and_buffers(vao, indexBuffer, vertexBuffer, vboData);
			//std::cout << totalIndices;
		}
		else {
			auto vboData = opengl::makeConsistentVertexTextureCoordNormalIndices(meshData);

			totalIndices =
				opengl::setup_vao_and_buffers(vao, indexBuffer, vertexBuffer, vboData);

			{
				auto image = raster::read_image_from_file(texPath);

				glActiveTexture(GL_TEXTURE0);
				texture.bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				auto channels = GL_RGBA;
				if (image.channels() == 3)
					channels = GL_RGB;

				glTexImage2D(GL_TEXTURE_2D,  //target
					0,                 		//level
					channels,         		//internalformat
					image.width(),    		//width
					image.height(),   		//height
					0,                		//border
					channels,           	//format
					GL_UNSIGNED_BYTE, 		//type
					image.data());			//data

				glGenerateMipmap(GL_TEXTURE_2D);


			}
		}
	}
}

void bindTexture(opengl::Texture const& texture, opengl::Program const &program, GLchar const * s, GLuint activeTex) {
	assert(activeTex < 32);
	glActiveTexture(GL_TEXTURE0 + activeTex); // Activate texture in position "activeTex"
	texture.bind();		//Bind texture object to active texture

	//Set bound texture to sampler2D uniform in shader program in position "activeTex" 
	program.use();
	glUniform1i(program.uniformLocation(s), activeTex);
}

float yaw;
float pitch;
float roll;

float x;
float y;
float z;

float lightx=1;
float lighty;
float lightz=1;



int oldMesh;
int shadingMode = 1;



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
	layout (location = 1) in vec3 normal;

	out Data
	{
		vec3 position;
		vec3 normal;
	} data;

	uniform mat4 transform;
	uniform mat4 projection;
	uniform mat4 view;

	void main()
	{
		//move vertices in model space to world space
		data.position = vec3(transform * vec4(position, 1.0));
		data.normal = normalize(mat3(transform) * normal);
	
		gl_Position = projection * view * vec4(data.position, 1.0);
	}

	)vs";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader, NULL);
	glCompileShader(vertexShader);

	openGL::checkShaderCompileStatus(vertexShader);

	const char* fragment_shader = R"fs(

	#version 330 core
	out vec4 fragColor;

	in Data {
		vec3 position;
		vec3 normal;
	} data;

	void main()
	{
		fragColor = vec4(data.normal, 1.0);
	}

	)fs";

	//std::cout << sizeof(Vertex) << "," << sizeof(glm::dvec3) << ","  << "\n";



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

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	char* buf = new char[10];

	float f = 0;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;

	

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	char* frameRate = "0";
	int mesh = 1;

	auto vao = opengl::makeVertexArrayObject();
	auto indexBuffer = opengl::makeBufferObject();
	auto vertexBuffer = opengl::makeBufferObject();

	GLuint totalIndices = 0;
	geometry::OBJMesh meshData;
	opengl::Texture& texture = opengl::generateTexture();
	loadMesh(mesh, meshData, totalIndices, vao, indexBuffer, vertexBuffer, texture);

	math::Vec3f lightPosition = math::Vec3f(1, 0, 1);

	auto depthShader = createShaderProgram("./depth_vs.glsl",
		"./depth_fs.glsl");
	auto normalShader = createShaderProgram("./normal_vs.glsl",
		"./normal_fs.glsl");
	auto phongSolidShader = createShaderProgram("./phong_vs.glsl",
		"./phong_fs.glsl");

	auto gouraudSolidShader = createShaderProgram("./gouraud_vs.glsl",
		"./gouraud_fs.glsl");

	auto flatSolidShader = createShaderProgram("./flat_vs.glsl",
		"./flat_fs.glsl");

	auto phongTexShader = createShaderProgram("./phong_tex_vs.glsl",
		"./phong_tex_fs.glsl");

	auto gouraudTexShader = createShaderProgram("./gouraud_tex_vs.glsl",
		"./gouraud_tex_fs.glsl");

	auto flatTexShader = createShaderProgram("./flat_tex_vs.glsl",
		"./flat_tex_fs.glsl");

	normalShader.use();

	phongSolidShader.use();
	opengl::setUniformVec3f(phongSolidShader.uniformLocation("lightPos"), lightPosition);
	opengl::setUniformVec3f(phongSolidShader.uniformLocation("lookPos"), math::Vec3f(0, 0, 3));
	opengl::setUniform1f(phongSolidShader.uniformLocation("ambient"), 0.1);

	gouraudSolidShader.use();
	opengl::setUniformVec3f(gouraudSolidShader.uniformLocation("lightPos"), lightPosition);
	opengl::setUniformVec3f(gouraudSolidShader.uniformLocation("lookPos"), math::Vec3f(0, 0, 3));
	opengl::setUniform1f(gouraudSolidShader.uniformLocation("ambient"), 0.1);





	// due to strange behaviour regarding the geometry shader, i used alternative method specified here:
	// https://gamedev.stackexchange.com/questions/154854/how-do-i-implement-flat-shading-in-glsl
	flatSolidShader.use();
	opengl::setUniformVec3f(flatSolidShader.uniformLocation("lightPos"), lightPosition);
	opengl::setUniformVec3f(flatSolidShader.uniformLocation("lookPos"), math::Vec3f(0, 0, 3));
	opengl::setUniform1f(flatSolidShader.uniformLocation("ambient"), 0.1);


	// Same as above 3, but modified for textures. Wanted to use a bool, got same odd behaviour as with geometry shaders.

	// due to strange behaviour regarding the geometry shader, i used alternative method specified here:
	// https://gamedev.stackexchange.com/questions/154854/how-do-i-implement-flat-shading-in-glsl
	flatTexShader.use();
	opengl::setUniformVec3f(flatTexShader.uniformLocation("lightPos"), lightPosition);
	opengl::setUniformVec3f(flatTexShader.uniformLocation("lookPos"), math::Vec3f(0, 0, 3));
	opengl::setUniform1f(flatTexShader.uniformLocation("ambient"), 0.1);

	phongTexShader.use();
	opengl::setUniformVec3f(phongTexShader.uniformLocation("lightPos"), lightPosition);
	opengl::setUniformVec3f(phongTexShader.uniformLocation("lookPos"), math::Vec3f(0, 0, 3));
	opengl::setUniform1f(phongTexShader.uniformLocation("ambient"), 0.1);

	gouraudTexShader.use();
	opengl::setUniformVec3f(gouraudTexShader.uniformLocation("lightPos"), lightPosition);
	opengl::setUniformVec3f(gouraudTexShader.uniformLocation("lookPos"), math::Vec3f(0, 0, 3));
	opengl::setUniform1f(gouraudTexShader.uniformLocation("ambient"), 0.1);

	depthShader.use();
	opengl::setUniformVec3f(depthShader.uniformLocation("lightPosition"), lightPosition);



	view = glm::lookAt(
		glm::vec3(0, 0, 3),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	opengl::Program* program = &normalShader;
	while (!glfwWindowShouldClose(window)) {


		glfwPollEvents(); // will process event queue and carry on 
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);

		if (oldMesh != mesh) {
			totalIndices = 0;
			meshData;
			texture = opengl::generateTexture();
			loadMesh(mesh, meshData, totalIndices, vao, indexBuffer, vertexBuffer, texture);
			oldMesh = mesh;
		}

		if (wf) { 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		switch (shadingMode) {
		case 1:
			depthShader.use();
			program = &depthShader;
			break;
		case 2:
			normalShader.use();
			program = &normalShader;
			break;

		case 3:
			if (tex) {
				flatTexShader.use();
				program = &flatTexShader;
			}
			else {
				flatSolidShader.use();
				program = &flatSolidShader;
			}
			break;
		case 4:
			if (tex) {
				gouraudTexShader.use();
				program = &gouraudTexShader;
			}
			else {
				gouraudSolidShader.use();
				program = &gouraudSolidShader;
			}

			break;
		case 5:
			if (tex) {
				phongTexShader.use();
				program = &phongTexShader;
			}
			else {
				phongSolidShader.use();
				program = &phongSolidShader;
			}
			break;

		}

		if (pers) {
			projection = glm::perspective(glm::radians(90.0f), m_width / m_height, 0.1f, 100.0f);
		}
		else {
			projection = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.0f);
		}
		if (tex) {
			bindTexture(tex, *program, "texture", 0);
		}
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		program->use();

		transform = glm::mat4(1.0f);

		transform = glm::scale(transform, glm::vec3(scaleFactor,scaleFactor,scaleFactor));

		transform = glm::translate(transform, glm::vec3(x, y, z));

		transform = glm::rotate(transform, yaw, glm::vec3(0,1,0));
		transform = glm::rotate(transform, roll, glm::vec3(0, 0, 1));
		transform = glm::rotate(transform, pitch, glm::vec3(1, 0, 0));




		vao.bind();
		//transform = glm::transpose(projection);

		lightPosition = math::Vec3f(lightx, lighty, lightz);

		//opengl::setUniform1f(program->uniformLocation("tex"), tex);

		opengl::setUniformVec3f(program->uniformLocation("lightPos"), lightPosition);
		opengl::setUniformMat4f(program->uniformLocation("model"), transform, false);
		opengl::setUniformMat4f(program->uniformLocation("view"), view, false);
		opengl::setUniformMat4f(program->uniformLocation("projection"), projection, false);

		//std::cout << totalIndices << "\n";

		glDrawElements(GL_TRIANGLES,
			totalIndices,    // # of triangles * 3
			GL_UNSIGNED_INT, // type of indices
			(void*)0        // offset
		);

		ImGui::StyleColorsDark();
		//imgui 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Test");
		if (ImGui::Button("Toggle Wireframe"))
			toggleWF();
		if (ImGui::Button("Toggle Perspective"))
			togglePer();
		if (ImGui::Button("Toggle Texture"))
			toggleTex();
		ImGui::SliderFloat("Yaw", &yaw, -3.2f, 3.2f);
		ImGui::SliderFloat("Pitch", &pitch, -3.2f, 3.2f);
		ImGui::SliderFloat("Roll", &roll, -3.2f, 3.2f);

		ImGui::SliderFloat("X", &x, -5.f, 5.f);
		ImGui::SliderFloat("Y", &y, -5.f, 5.f);
		ImGui::SliderFloat("Z", &z, -5.f, 5.f);
		ImGui::SliderInt("Model", &mesh, 1, 5);
		ImGui::SliderInt("Shading", &shadingMode, 1, 5);

		ImGui::Text("test");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();

		ImGui::Begin("Light");

		ImGui::SliderFloat("X", &lightx, -5.f, 5.f);
		ImGui::SliderFloat("Y", &lighty, -5.f, 5.f);
		ImGui::SliderFloat("Z", &lightz, -5.f, 5.f);

		ImGui::End();



		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		//refresh framebuffer
		glfwSwapBuffers(window);

		
		 
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

