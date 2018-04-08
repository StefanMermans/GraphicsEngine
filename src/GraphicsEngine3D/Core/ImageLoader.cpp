#include "ImageLoader.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION

unsigned char* Core::ImageLoader::readImage(const std::string &path, int * width, int * height)
{
	int channels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageBuffer = stbi_load(path.c_str(), width, height, &channels, 4);

	if (channels != 4) {
		throw std::exception("Wrong number of channels loaded!");
		return NULL;
	}

	if (!imageBuffer) {
		throw std::exception("Failed to load image!");
		return NULL;
	}

	return imageBuffer;
}

void Core::ImageLoader::release(unsigned char * imageData) {
	stbi_image_free(imageData);
}
