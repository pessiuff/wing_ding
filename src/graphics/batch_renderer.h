#pragma once

#include "shader_program.h"
#include "texture.h"
#include "../globals.h"

#include <glad/glad.h>
#include <vec2.hpp>
#include <vec4.hpp>
#include <vector>

namespace wing_ding {
	struct vertex_attributes {
		glm::vec2 m_position;
		glm::vec2 m_texture_coordinates;

		vertex_attributes() : m_position(0.0f, 0.0f), m_texture_coordinates(0.0f, 0.0f) {
		
		}

		vertex_attributes(glm::vec2 position, glm::vec2 texture_coordinates) {
			m_position = position;
			m_texture_coordinates = texture_coordinates;
		}
	};

	class batch_renderer {
	public:
		batch_renderer();
		~batch_renderer();

		void draw(texture* source_texture, glm::vec4 source_rect, glm::vec4 dest_rect);
		void flush();

	private:
		GLuint m_vao_handle = 0, m_vbo_handle = 0, m_ebo_handle = 0;
		shader_program m_shader_program;
		texture* m_current_texture = nullptr;
		unsigned int m_current_vertices_index = 0;
		vertex_attributes m_vertices[globals::g_max_batch_quads * 4] = {};
		unsigned int m_indices[globals::g_max_batch_quads * 6] = {};
	};
}