// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "sceneGraph.hpp"
#include "vao.hpp"
#include <stdbool.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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

	float vertices[] = {
		 0.6,  0.6,  0.0,
		-0.2,  0.6,  0.0,
		-0.6, -0.6,  0.0,
		 0.2, -0.6,  0.0,
	};

	uint indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	float colors[] = {
		 0.3,  0.7,  0.2,  1.0,
		 0.3,  0.7,  0.2,  1.0,
		 0.3,  0.7,  0.2,  1.0,
		 0.3,  0.7,  0.2,  1.0,
	};

	uint askew     = createTrianglesVAO(vertices, indices, colors, 2);
	uint extrusion_triangles_num = 2;
	uint extrusion = createExtrusionVAO(vertices, indices, colors, &extrusion_triangles_num, 0.2f);

    Gloom::Shader prog;
    prog.makeBasicShader(
        "../../shaders/camera.vert",
        "../../shaders/color.frag"
    );
    prog.activate();

    uint transform_location = glGetUniformLocation(prog.get(), "transform");
	

    glm::mat4 camera;
    camera = glm::rotate(camera, 0.4f, glm::vec3(1.0f, 0.0f, 0.0f));
    camera = glm::translate(camera, glm::vec3(0.0f, 0.0f, 0.0f));
    // glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 5.0f/4.0f, 0.1f, 100.0f);
    // camera = perspective * camera;
    glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(camera));

	double deltaSeconds;

	// Rendering Loop
	while (!glfwWindowShouldClose(window)) {
		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		deltaSeconds = getTimeDeltaSeconds();

	    glBindVertexArray(extrusion);
        glDrawElements(GL_TRIANGLES, kIndicesPerTriangle * extrusion_triangles_num, GL_UNSIGNED_INT, 0);
        printGLError();

	    // glBindVertexArray(askew);
     //    glDrawElements(GL_TRIANGLES, kIndicesPerTriangle * 2, GL_UNSIGNED_INT, 0);
     //    printGLError();

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
