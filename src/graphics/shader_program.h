#pragma once

#include <string>
#include <glad/glad.h>

namespace wing_ding {
	class shader_program {
	public:
		shader_program();
		~shader_program();
		shader_program(const shader_program&) = delete;
		shader_program& operator=(const shader_program&) = delete;

		void attach_shader(GLint shader_type, const std::string& source);
		void link_program();
		void use();

		GLint get_uniform_location(const std::string& name);
		void set_uniform1i(const std::string& name, GLint value);

	private:
		GLuint m_program_handle = 0;
		GLuint m_vertex_handle = 0, m_fragment_handle = 0;
	};
}