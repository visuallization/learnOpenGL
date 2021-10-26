#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"

using namespace std;

void resizeViewport(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	// Initialize GLFW
	glfwInit();
	// Configure GLFW
	// Use OPENGL3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use the OPENGL core profile (no backwards compatible features)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create a new window object (OS dependent)
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		cout << "Failed to create GLFW window!" << endl;
		return -1;
	}
	// Make the context of the window the context on the current thread
	glfwMakeContextCurrent(window);

	// Before we call any OpenGL functions initialize GLAD
	// GLAD manages function pointers for OpenGL
	// We pass GLAD the function to load the address pof the OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD!" << endl;
		return -1;
	}

	// Create the OpenGL viewport
	glViewport(0, 0, 800, 600);

	// Get the amount of allowed vertex attributes in a shader which is defined by the hardware
	int numberOfVertexAttributes = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numberOfVertexAttributes);
	cout << "Maximum number of vertex attributes supported by hardware: " << numberOfVertexAttributes << endl;

	// Register GLFW callbacks after we created the window and before the render loop
	// Register the resize callback
	glfwSetFramebufferSizeCallback(window, resizeViewport);

	// Draw meshes in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Draw meshes in filled mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Define position coordinates and rgb values of the vertices of a triangle in Normalized Device Coordinates (NDC: -1, 1)
	float vertices[] = {
		// positions			// colors
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f    // top 
	};
	// Define drawing indeces to prevent defining redundant vertices
	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// 0. Bind Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 1. Copy our vertices array in a buffer for OpenGL to use
	// Create a vertex buffer object to store the vertices in an opengl object and reference it by id
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// OpenGL has many types of buffer objects and the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy the previously defined vertex data into the buffer's memory
	// Store the vertex data within memory on the graphics card as managed by a vertex buffer object named VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Bind Elemment Buffer Object (stores indices that OpenGL uses to decide what vertices to draw, helps to reduce the definition of redundant vertices, e.g for each adjustent triangle)
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 2. Set the vertex attributes pointers
	// Tell OpenGL how it should interpret the vertex data
	// We have to manually specify what part of our input data goes to which vertex attribute in the vertex shader
	// Define the layout of the position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// Enable the position vertex attribute with glEnableVertexAttribArray giving the vertex attribute location as its argument (layout (location=0))
	glEnableVertexAttribArray(0);

	// Define the layout of the color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Enable the color vertex attribute
	glEnableVertexAttribArray(1);

	// Another way to fetch the location of the vertex atrribute in a shader
	// cout << glGetAttribLocation(shaderProgram, "aPos") << endl;

	// Unbind vertex array
	glBindVertexArray(0);

	Shader shader("shader.vert", "shader.frag");

	// Initialize the render loop
	while (!glfwWindowShouldClose(window)) {
		// Handle input
		handleInput(window);

		// RENDERING LOGIC
		// Set the color which glClear will (state-setting function)
		glClearColor(0.2f,0.3f,0.3f, 1.0f);
		// Actually clear the screen's color (state-using function)
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate programm object
		// Every shader and rendering call after glUseProgram will now use this program object (and thus the shaders)
		shader.use();



		// Draw the rectangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Draw a rectangle using indices
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap the 2D color buffer
		// front buffer displays the rendered image
		// back buffer draws the next image
		glfwSwapBuffers(window);
		// Check if any events are triggered (e.g keyboard input or mouse movement events)
		// updates the window state and calls the corresponding callback functions.
		glfwPollEvents();
	}

	// clean up all the GLFW resources and properly exit the application
	glfwTerminate();
	return 0;
}