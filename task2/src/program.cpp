// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "sceneGraph.hpp"
#include "vao.hpp"
#include <stdbool.h>
#include <iostream>

bool left_control_down = false;

void runProgram(GLFWwindow* window)
{

	// Set GLFW callback mechanism(s)
	glfwSetKeyCallback(window, keyboardCallback);

	// Enable depth (Z) buffer (accept "closest" fragment)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Configure miscellaneous OpenGL settings
	glEnable(GL_CULL_FACE);

	// Set default colour after clearing the colour buffer
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Set provoking vertex convention
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);


	Model* models[2];
	uint models_i = 0;

	float square_vertices[] = {
		 1.0,  1.0,
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0, -1.0,
	};

	uint square_indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	float board_color[] = { 0.2, 0.7, 0.3, 1.0 };

	Model board = createFlatModel(square_vertices, square_indices, board_color, 2, 0.3f, false);
    board.transform = glm::translate(board.transform, glm::vec3(0.0f, 0.0f, -5.0f));
	models[models_i++] = &board;

    float square_colors[][4] = { 
    	{ 0.7, 0.1, 0.1, 1.0 },
		{ 0.1, 0.1, 0.7, 1.0 },
	};

   //  for (int i = 0; i < 5; i++) {
   //  	for (int j = 0; j < 8; j++) {
			// Model square = createFlatModel(square_vertices, square_indices, square_colors[(i + j) % 2], 2, 0.0f, false);
			// square.transform = glm::translate(square.transform, glm::vec3(j, i, -4.7f));
			// models[models_i++] = &square;
   //  	}
   //  }
	if (true) {
		Model board2 = createFlatModel(square_vertices, square_indices, square_colors[0], 2, 0.0f, false);
		board2.transform = glm::translate(board2.transform, glm::vec3(0.0f, 0.0f, -4.7f));
		models[models_i++] = &board2;
	}


    Gloom::Shader prog;
    prog.makeBasicShader(
        "../../shaders/light.vert",
        "../../shaders/light.frag"
    );
    prog.activate();

    uint model_location = glGetUniformLocation(prog.get(), "model");
    
    uint camera_location = glGetUniformLocation(prog.get(), "camera");
    glm::mat4 camera = glm::perspective(glm::radians(45.0f), 5.0f/4.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(camera_location, 1, GL_FALSE, glm::value_ptr(camera));
	
    uint light_direction_location = glGetUniformLocation(prog.get(), "lightDirection");
	glm::vec3 light_direction(0.0f, 0.0f, -1.0f);
    glUniform3fv(light_direction_location, 1, glm::value_ptr(light_direction));

	// double deltaSeconds;

	// Rendering Loop
	while (!glfwWindowShouldClose(window)) {
		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		// deltaSeconds = getTimeDeltaSeconds();
	    // board.transform = glm::rotate(board.transform, -0.5f * (float)deltaSeconds, glm::vec3(1.0f, 0.0f, 0.0f));
    	
    	for (uint i = 0; i < sizeof(models)/sizeof(Model*); i++) {
	    	renderModel(*models[i], model_location);	
    	}

		// Handle other events
		glfwPollEvents();

		// Flip buffers
		glfwSwapBuffers(window);
	}

	std::cout << '\n';
}


void keyboardCallback(GLFWwindow* window, int key, int scancode,
	int action, int mods)
{
	// Store if left control is pressed
	if (key == GLFW_KEY_LEFT_CONTROL) {
		left_control_down = action != GLFW_RELEASE;
	}
	// Use escape or ctrl+w for terminating the GLFW window
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_W:
			if (!left_control_down) break;
			case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}
}
