#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

	// Register GLFW callbacks after we created the window and before the render loop
	// Register the resize callback
	glfwSetFramebufferSizeCallback(window, resizeViewport);

	// Initialize the render loop
	while (!glfwWindowShouldClose(window)) {
		// Handle input
		handleInput(window);

		// RENDERING LOGIC
		// Set the color which glClear will use
		// state-setting function
		glClearColor(0.2f,0.3f,0.3f, 1.0f);
		// Actually clear the screen's color buffer
		// state-using function
		glClear(GL_COLOR_BUFFER_BIT);

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