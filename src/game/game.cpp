#include "game.h"

#include "../globals.h"

#include <print>
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/backends/imgui_impl_glfw.h"
#include "../../vendor/imgui/backends/imgui_impl_opengl3.h"

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
		m_window = glfwCreateWindow(720, 1280, globals::g_window_name.c_str(), nullptr, nullptr);
		if (m_window == nullptr) {
			std::println("[game::run] There was a problem while creating a GLFW window!");
			return 1;
		}

		// Load OpenGL for the game.
		glfwMakeContextCurrent(m_window);
		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 1) {
			std::println("[game::run] There was a problem while loading OpenGL!");
			return 1;
		}
		
		// Setup ImGui.
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

		// Setup one time OpenGL settings.
		glViewport(0, 0, 720, 1280);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		while (!glfwWindowShouldClose(m_window)) {
			// Process pending events.
			glfwPollEvents();

			// Update the game depending on the state.
			m_state_manager.update();

			// Sleep ImGui if minimized.
			if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
			{
				ImGui_ImplGlfw_Sleep(10);
				continue;
			}

			// Clear color before drawing anything.
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw/render the game depending on the state.
			m_state_manager.draw();

			// Start a new ImGui frame.
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Draw debug window.
			{
				ImGui::Begin("Debug Window");

				const std::string state_string = m_state_manager.get_state() == state::pre ? "pre"
					: m_state_manager.get_state() == state::during ? "during"
					: m_state_manager.get_state() == state::post ? "post"
					: "unknown";
				ImGui::Text("Current game state: %s", state_string);
				if (ImGui::Button("Pre")) {
					m_state_manager.change_state(state::pre);
				}

				ImGui::SameLine();
				if (ImGui::Button("During")) {
					m_state_manager.change_state(state::during);
				}

				ImGui::SameLine();
				if (ImGui::Button("Post")) {
					m_state_manager.change_state(state::post);
				}

				ImGui::SameLine();
				ImGui::Text("Change game state");

				ImGui::End();
			}

			// Render the current ImGui frame.
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Swap front & back buffers.
			glfwSwapBuffers(m_window);
		}

		// Complete shutdown.
		glfwTerminate();
		return 0;
	}
}