#include "game.h"

#include "../globals.h"

#include <print>

namespace wing_ding {
	int game::run() {
		// Initialize and verify GLFW.
		if (glfwInit() != GLFW_TRUE) {
			std::println("[game::run] There was a problem while initializing GLFW!");
			return 1;
		}

		// Setup window hints.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// Create a window and verify it.
		m_window = glfwCreateWindow(globals::g_window_width, globals::g_window_height, globals::g_window_name.c_str(), nullptr, nullptr);
		if (m_window == nullptr) {
			std::println("[game::run] [error] There was a problem while creating a GLFW window!");
			return 1;
		}

		// Load OpenGL.
		glfwMakeContextCurrent(m_window);
		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 1) {
			std::println("[game::run] [error] There was a problem while loading OpenGL!");
			return 1;
		}

		// Setup one time OpenGL settings.
		glViewport(0, 0, globals::g_window_width, globals::g_window_height);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		while (!glfwWindowShouldClose(m_window)) {
			// Process pending events.
			glfwPollEvents();

			// Update the game depending on the state.
			m_state_manager.update();

			// Clear color before drawing anything.
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw/render the game depending on the state.
			m_state_manager.draw();

			// Swap front & back buffers.
			glfwSwapBuffers(m_window);
		}

		// Complete shutdown.
		glfwTerminate();
		return 0;
	}
}