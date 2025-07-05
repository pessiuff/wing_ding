#pragma once

#include <string>

namespace wing_ding {
	class image_loader {
	public:
		// Loads only RGBA images.
		static unsigned char* load_image_rgba(const std::string& file_name, int* width, int* height);

		// Don't forget to call or get doomed.
		static void free_image(unsigned char* image_data_ptr);

	private:
		image_loader();
	};
}