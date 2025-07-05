#include "batch_renderer.h"

#include <print>

namespace wing_ding {
	batch_renderer::batch_renderer() {
		// Generate and verify VAO.
		glGenVertexArrays(1, &m_vao_handle);
		if (m_vao_handle == 0) {
			std::println("[batch_renderer::batch_renderer] [error] VAO creation failed.");
			return;
		}
		glBindVertexArray(m_vao_handle);

		// Generate, verify VBO and setup attribute pointers.
		glGenBuffers(1, &m_vbo_handle);
		if (m_vbo_handle == 0) {
			std::println("[batch_renderer::batch_renderer] [error] VBO creation failed.");
			glBindVertexArray(0);
			return;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handle);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_attributes), (void*) 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_attributes), (void*) offsetof(vertex_attributes, m_texture_coordinates));
		glEnableVertexAttribArray(1);

		// Generate and verify EBO.
		glGenBuffers(1, &m_ebo_handle);
		if (m_ebo_handle == 0) {
			std::println("[batch_renderer::batch_renderer] [error] EBO creation failed.");
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			return;
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_handle);

		// Unbind them because no need anymore.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		// Initialize indices.
		for (unsigned int i = 0; i < globals::g_max_batch_quads; i++) {
			unsigned int offset = i * 4;

			m_indices[i * 6 + 0] = offset + 0;
			m_indices[i * 6 + 1] = offset + 1;
			m_indices[i * 6 + 2] = offset + 2;

			m_indices[i * 6 + 3] = offset + 1;
			m_indices[i * 6 + 4] = offset + 3;
			m_indices[i * 6 + 5] = offset + 2;
		}

		// Setup shader program.
		m_shader_program.attach_shader(GL_VERTEX_SHADER, globals::g_vertex_source);
		m_shader_program.attach_shader(GL_FRAGMENT_SHADER, globals::g_fragment_source);
		m_shader_program.link_program();
	}

	batch_renderer::~batch_renderer() {
		if (m_vao_handle != 0) {
			glDeleteVertexArrays(1, &m_vao_handle);
		}
		if (m_vbo_handle != 0) {
			glDeleteBuffers(1, &m_vbo_handle);
		}
		if (m_ebo_handle != 0) {
			glDeleteBuffers(1, &m_ebo_handle);
		}
	}

	void batch_renderer::draw(texture* source_texture, glm::vec4 source_rect, glm::vec4 dest_rect) {
		// Check if texture changed or max quad count reached.
		if (m_current_texture != source_texture || (m_current_vertices_index >= (globals::g_max_batch_quads * 4) - 4)) {
			flush();
			m_current_texture = source_texture;
		}

		if (m_current_texture == nullptr) {
			m_current_texture = source_texture;
		}

		// Normalize source coordinates.
		float srcX = source_rect.x / source_texture->get_image_width();
		float srcY = source_rect.y / source_texture->get_image_height();
		float srcW = source_rect.z / source_texture->get_image_width();
		float srcH = source_rect.w / source_texture->get_image_height();

		// Normalize destination coordinates.
		float ndcX = (dest_rect.x / globals::g_window_width) * 2.0f - 1.0f;
		float ndcY = (dest_rect.y + dest_rect.w / globals::g_window_height) * 2.0f - 1.0f;
		float ndcW = (dest_rect.z / globals::g_window_width) * 2.0f;
		float ndcH = (dest_rect.w / globals::g_window_height) * 2.0f;

		// Add the four vertices for this textured quad.
		// Top-left
		m_vertices[m_current_vertices_index++] = {
			glm::vec2(ndcX, ndcY),
			glm::vec2(srcX, srcY)
		};

		// Top-right
		m_vertices[m_current_vertices_index++] = {
			glm::vec2(ndcX + ndcW, ndcY),
			glm::vec2(srcX + srcW, srcY)
		};

		// Bottom-left
		m_vertices[m_current_vertices_index++] = {
			glm::vec2(ndcX, ndcY - ndcH),
			glm::vec2(srcX, srcY + srcH)
		};

		// Bottom-right
		m_vertices[m_current_vertices_index++] = {
			glm::vec2(ndcX + ndcW, ndcY - ndcH),
			glm::vec2(srcX + srcW, srcY + srcH)
		};
	}

	void batch_renderer::flush() {
		// Check for invalid states.
		if (m_current_vertices_index == 0 || m_current_texture == nullptr)
			return;

		glBindVertexArray(m_vao_handle);

		// Setup vertices.
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_attributes) * m_current_vertices_index, m_vertices, GL_STREAM_DRAW);

		// Setup indices.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ((m_current_vertices_index / 4) * 6), m_indices, GL_STREAM_DRAW);

		// Bind texture.
		m_current_texture->bind();

		// Use the shader and set uniform values.
		m_shader_program.use();
		m_shader_program.set_uniform1i("textureSampler", 0);

		// Draw
		glDrawElements(GL_TRIANGLES, (m_current_vertices_index / 4) * 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		// Reset batch state.
		m_current_vertices_index = 0;
		m_current_texture = nullptr;
	}
}