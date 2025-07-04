#include "shader_program.h"

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

	shader_attach_status shader_program::attach_shader(GLint shader_type, const std::string& source, std::string& info_log) {
		// Make sure the shader program exists.
		if (m_program_handle == 0)
			return shader_attach_status::invalid_program;

		// Try creating a shader.
		GLuint shader_handle = glCreateShader(shader_type);
		if (shader_handle == 0)
			return shader_attach_status::invalid_shader;

		// Check shader type to see if the same time has already been attached before.
		switch (shader_type) {
		case GL_VERTEX_SHADER:
			if (m_vertex_handle != 0) {
				glDeleteShader(shader_handle);
				return shader_attach_status::already_attached;
			}

			break;
		case GL_FRAGMENT_SHADER:
			if (m_fragment_handle != 0) {
				glDeleteShader(shader_handle);
				return shader_attach_status::already_attached;
			}

			break;
		default:
			glDeleteShader(shader_handle);
			return shader_attach_status::unsupported_shader;
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
				info_log = info_log_buffer.data();
			}
			else {
				info_log = "Unknown compilation error.";
			}

			glDeleteShader(shader_handle);

			return shader_attach_status::compile_failure;
		}

		// Set corresponding shader handles.
		if (shader_type == GL_VERTEX_SHADER)
			m_vertex_handle = shader_handle;
		else if (shader_type == GL_FRAGMENT_SHADER)
			m_fragment_handle = shader_handle;

		// Finally, attach the shader to the program.
		glAttachShader(m_program_handle, shader_handle);

		return shader_attach_status::success;
	}

	program_link_status shader_program::link_program(std::string& info_log) {
		// Make sure the shader program exists.
		if (m_program_handle == 0)
			return program_link_status::invalid_program;

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
				info_log = info_log_buffer.data();
			}
			else {
				info_log = "Unknown linking error.";
			}

			glDeleteProgram(m_program_handle);
			m_program_handle = 0;

			return program_link_status::link_failure;
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

		return program_link_status::success;
	}

	GLint shader_program::get_uniform_location(const std::string& name) {
		if (m_program_handle == 0)
			return -1;

		return glGetUniformLocation(m_program_handle, name.c_str());
	}

	void shader_program::use() {
		glUseProgram(m_program_handle);
	}
}
