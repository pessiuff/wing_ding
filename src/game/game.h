#pragma once

#include "state_manager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace wing_ding {
	class game {
	public:
		game() = default;
		~game() = default;

		int run();

	private:
		GLFWwindow* m_window = nullptr;
		state_manager m_state_manager;
	};
}