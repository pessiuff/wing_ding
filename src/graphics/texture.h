#pragma once

#include <string>
#include <glad/glad.h>

namespace wing_ding {
	class texture {
	public:
		explicit texture(const std::string& file_name);
		~texture();

		void bind();

		[[nodiscard]] int get_image_width();
		[[nodiscard]] int get_image_height();

	private:
		int m_image_width = 0, m_image_height = 0;
		GLuint m_texture_handle = 0;
	};
}