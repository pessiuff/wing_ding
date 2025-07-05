#include "image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace wing_ding {
	unsigned char* image_loader::load_image_rgba(const std::string& file_name, int* width, int* height) {
		int channel_count = 0;
		return stbi_load(file_name.c_str(), width, height, &channel_count, 4);
	}

	void image_loader::free_image(unsigned char* image_data_ptr)
	{
		if (image_data_ptr == nullptr)
			return;

		stbi_image_free(image_data_ptr);
	}
}