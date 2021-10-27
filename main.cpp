#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"

using namespace std;

// settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

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
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);

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
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

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

	// Enable Z-Buffer
	glEnable(GL_DEPTH_TEST);

	// BUILD VERTEX AND FRAGMENT SHADERS
	Shader shader("shader.vert", "shader.frag");
	Shader lightShader("light.vert", "light.frag");

	// Define position coordinates and texture coordinates of the vertices a cube
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Define the positions of multiple cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(-2.0f, -5.0f, 15.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Define the position of the light source cube
	glm::vec3 lightPostion(0.0f, 0.0f, 0.0f);

	// CREATE A BOX
	// Bind Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Copy our vertices array in a buffer for OpenGL to use
	// Create a vertex buffer object to store the vertices in an opengl object and reference it by id
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// OpenGL has many types of buffer objects and the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy the previously defined vertex data into the buffer's memory
	// Store the vertex data within memory on the graphics card as managed by a vertex buffer object named VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set the vertex attributes pointers
	// Tell OpenGL how it should interpret the vertex data
	// We have to manually specify what part of our input data goes to which vertex attribute in the vertex shader

	// Define the layout of the position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// Enable the position vertex attribute with glEnableVertexAttribArray giving the vertex attribute location as its argument (layout (location=0))
	glEnableVertexAttribArray(0);

	// Define the layout of the texture coordinates attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// Enable the texture coordinates attribute
	glEnableVertexAttribArray(1);

	// Another way to fetch the location of the vertex atrribute in a shader
	// cout << glGetAttribLocation(shaderProgram, "aPos") << endl;

	// Unbind vertex array
	//glBindVertexArray(0);


	// GENERATING FIRST TEXTURE
	unsigned int texture1;
	glGenTextures(1, &texture1);
	// Just like other objects we need to bind it so any subsequent texture commands will configure the currently bound texture
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set the texture wrapping and filtering options on the currently bound texture object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// IMAGE LOADING
	// Flip y axis of image so it is loaded correctly
	stbi_set_flip_vertically_on_load(true);

	// Load first image
	// We need the images width and height for generating textures later on
	int width, height, numberOfColorChannels;
	unsigned char* image1 = stbi_load("container.jpg", &width, &height, &numberOfColorChannels, 0);
	if (image1) {
		// Actually generate texture with the previously loaded image
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		cout << "Failed to load texture" << endl;
	}
	// Free the image memory
	stbi_image_free(image1);

	// GENERATE SECOND TEXTURE
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load second image
	unsigned char* image2 = stbi_load("awesomeface.png", &width, &height, &numberOfColorChannels, 0);
	if (image2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(image2);

	// Activate the shader before setting texture uniforms
	shader.use();
	// Tell OpenGL to which texture unit each shader sampler belongs to
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);


	// CREATE A LIGHT SOURCE CUBE
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// use the already created cube buffer for the light
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Initialize the render loop
	while (!glfwWindowShouldClose(window)) {
		// INPUT
		handleInput(window);

		// RENDERING LOGIC
		// Set the color which glClear will (state-setting function)
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Actually clear the screen's color and depth buffer (state-using function)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TEXTURES
		// Activate the texture unit first before binding texture
		// Most graphic drivers set default texture unit to 0 and you can skip this step if you only want to assign 1 texture
		glActiveTexture(GL_TEXTURE0);
		// Bind the texture and it will automatically assign it to the fragment shader's sampler
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		// RENDER CUBES

		// Activate shader programm object for the cubes
		// Every shader and rendering call after glUseProgram will now use this program object (and thus the shaders)
		shader.use();
		// Setup object & lighting colors
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		// TRANSFORMATIONS
		// Create Model Matrix to transform the model's local space to world space
		glm::mat4 model = glm::mat4(1.0f);
		// Create View Matrix to transform world space to view (camera) space
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		// Create Porjection Matrixs to transform view space to clip space
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		// Send the matrices to the shader
		glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++) {
			// Reset model identity matrix
			model = glm::mat4(1.0f);
			// Position cube
			model = glm::translate(model, cubePositions[i]);
			// Rotate cube
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f * (i + 1)), glm::vec3(1.0f, 0.3f, 0.5f));
			// Send model matrix to the shader
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// RENDER LIGHT SOURCE CUBE
		lightShader.use();
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		// Reset model identity matrix
		model = glm::mat4(1.0f);
		// Position the light source cube
		model = glm::translate(model, lightPostion);
		// Shrink the light source cube
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Swap the 2D color buffer
		// front buffer displays the rendered image
		// back buffer draws the next image
		glfwSwapBuffers(window);
		// Check if any events are triggered (e.g keyboard input or mouse movement events)
		// updates the window state and calls the corresponding callback functions.
		glfwPollEvents();
	}

	// optional: deallocate all ressources once they have outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// clean up all the GLFW resources and properly exit the application
	glfwTerminate();
	return 0;
}