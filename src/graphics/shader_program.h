#pragma once

#include <string>
#include <glad/glad.h>

namespace wing_ding {
	enum class shader_attach_status {
		success,
		invalid_program,
		invalid_shader,
		unsupported_shader,
		already_attached,
		compile_failure
	};

	enum class program_link_status {
		success,
		invalid_program,
		link_failure
	};

	class shader_program {
	public:
		shader_program();
		~shader_program();
		shader_program(const shader_program&) = delete;
		shader_program& operator=(const shader_program&) = delete;

		// `info_log` will only have a value in case of a compilation error.
		shader_attach_status attach_shader(GLint shader_type, const std::string& source, std::string& info_log);

		// `info_log` will only have a value in case of a linking error.
		program_link_status link_program(std::string& info_log);

		GLint get_uniform_location(const std::string& name);

		void use();

	private:
		GLuint m_program_handle = 0;
		GLuint m_vertex_handle = 0, m_fragment_handle = 0;
	};
}