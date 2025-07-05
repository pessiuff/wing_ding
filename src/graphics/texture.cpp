#include "texture.h"

#include "../utils/image_loader.h"

#include <print>

namespace wing_ding {
	texture::texture(const std::string& file_name) {
		// Try loading the image.
		unsigned char* m_image_data_ptr = image_loader::load_image_rgba(file_name, &m_image_width, &m_image_height);
		if (m_image_data_ptr == nullptr) {
			std::println("[texture::texture] [error] Couldn't load image named `{}`. Make sure that it exists and has 4 channels (r, g, b, a)!", file_name);
			return;
		}

		// Try creating a texture in GPU.
		glGenTextures(1, &m_texture_handle);
		if (m_texture_handle == 0) {
			std::println("[texture::texture] [error] Texture creation has failed.");
			return;
		}

		// Bind the texture to setup its properties.
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);

		// Setup texture data using the loaded image data and free the loaded image data afterwards.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_image_width, m_image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image_data_ptr);
		image_loader::free_image(m_image_data_ptr);

		// Setup texture parameters to be pixel perfect.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Unbind since we are done setting it up.
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	texture::~texture() {
		if (m_texture_handle != 0) {
			glDeleteTextures(1, &m_texture_handle);
		}
	}

	void texture::bind() {
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);
	}
}