#ifndef IMAGE_LOADER_H
#define IMAGE_LAODER_H

#include <string>

namespace Core {
	class ImageLoader {
	public:
		static unsigned char* readImage(const std::string &path, int * width, int * height);
		static void release(unsigned char * imageData);
	};
};

#endif // !IMAGE_LOADER_H
