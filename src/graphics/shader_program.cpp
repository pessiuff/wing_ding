#include "shader_program.h"

#include <print>
#include <vector>

namespace wing_ding {

	shader_program::shader_program() {
		m_program_handle = glCreateProgram();
	}

	shader_program::~shader_program() {
		if (m_program_handle != 0) {
			glDeleteProgram(m_program_handle);
			m_program_handle = 0;
		}
	}

	void shader_program::attach_shader(GLint shader_type, const std::string& source) {
		// Make sure the shader program exists.
		if (m_program_handle == 0) {
			std::println("[shader_program::attach_shader] [error] Program handle was `0` while trying to execute the function.");
			return;
		}

		// Try creating a shader.
		GLuint shader_handle = glCreateShader(shader_type);
		if (shader_handle == 0) {
			std::println("[shader_program::attach_shader] [error] Shader creation has failed.");
			return;
		}

		// Check shader type to see if the same time has already been attached before.
		switch (shader_type) {
		case GL_VERTEX_SHADER:
			if (m_vertex_handle != 0) {
				glDeleteShader(shader_handle);
				std::println("[shader_program::attach_shader] [error] Tried attaching an already existing shader type (GL_VERTEX_SHADER).");
				return;
			}

			break;
		case GL_FRAGMENT_SHADER:
			if (m_fragment_handle != 0) {
				glDeleteShader(shader_handle);
				std::println("[shader_program::attach_shader] [error] Tried attaching an already existing shader type (GL_FRAGMENT_SHADER).");
				return;
			}

			break;
		default:
			glDeleteShader(shader_handle);
			std::println("[shader_program::attach_shader] [error] Tried attaching an invalid/unsupported shader type.");
			return;
		}

		// Set the shader source.
		const char* source_c_str = source.c_str();
		glShaderSource(shader_handle, 1, &source_c_str, nullptr);

		// Try compiling and check for errors.
		glCompileShader(shader_handle);
		GLint compile_status = GL_FALSE;
		glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status);
		if (compile_status == GL_FALSE) {
			GLint info_log_length = 0;
			glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length > 0) {
				std::vector<char> info_log_buffer(info_log_length);
				glGetShaderInfoLog(shader_handle, info_log_length, nullptr, info_log_buffer.data());
				std::println("[shader_program::attach_shader] [error] Error while compiling shader: [\n{}]", info_log_buffer.data());
			}
			else {
				std::println("[shader_program::attach_shader] [error] Unknown shader compilation error.");
			}

			glDeleteShader(shader_handle);
			return;
		}

		// Set corresponding shader handles.
		if (shader_type == GL_VERTEX_SHADER)
			m_vertex_handle = shader_handle;
		else if (shader_type == GL_FRAGMENT_SHADER)
			m_fragment_handle = shader_handle;

		// Finally, attach the shader to the program.
		glAttachShader(m_program_handle, shader_handle);
	}

	void shader_program::link_program() {
		// Make sure the shader program exists.
		if (m_program_handle == 0) {
			std::println("[shader_program::link_program] [error] Program handle was `0` while trying to execute the function.");
			return;
		}

		// Try linking and check for errors.
		glLinkProgram(m_program_handle);
		GLint link_status = GL_FALSE;
		glGetProgramiv(m_program_handle, GL_LINK_STATUS, &link_status);
		if (link_status == GL_FALSE) {
			GLint info_log_length = 0;
			glGetProgramiv(m_program_handle, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length > 0) {
				std::vector<char> info_log_buffer(info_log_length);
				glGetProgramInfoLog(m_program_handle, info_log_length, nullptr, info_log_buffer.data());
				std::println("[shader_program::link_program] [error] Error while linking program: [\n{}]", info_log_buffer.data());
			}
			else {
				std::println("[shader_program::link_program] [error] Unknown linking error.");
			}

			glDeleteProgram(m_program_handle);
			m_program_handle = 0;
			return;
		}

		// Finally, detach & delete shaders after successfully linking to free memory.
		if (m_vertex_handle != 0) {
			glDetachShader(m_program_handle, m_vertex_handle);
			glDeleteShader(m_vertex_handle);
			m_vertex_handle = 0;
		}

		if (m_fragment_handle != 0) {
			glDetachShader(m_program_handle, m_fragment_handle);
			glDeleteShader(m_fragment_handle);
			m_fragment_handle = 0;
		}
	}

	void shader_program::use() {
		glUseProgram(m_program_handle);
	}

	GLint shader_program::get_uniform_location(const std::string& name) {
		if (m_program_handle == 0)
			return -1;

		return glGetUniformLocation(m_program_handle, name.c_str());
	}

	void shader_program::set_uniform1i(const std::string& name, GLint value) {
		GLint location = get_uniform_location(name);
		if (location == -1)
			return;

		glUniform1i(location, value);
	}
}
