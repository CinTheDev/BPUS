#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <stdexcept>
#include "utils.cpp"

Image::Image() {
	w = 0;
	h = 0;
	channels = 0;
	data = new uint8_t[0];
}

Image::Image(const char* filename) {
	if (read(filename)) {
		size = w * h * channels;
	}
	else {
		throw std::invalid_argument("File not found");
	}
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
	size = w * h * channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
	memcpy(data, img.data, img.size);
}

Image::~Image() {
	stbi_image_free(data);
}

ImageType Image::getFileType(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if (ext != nullptr) {
		if (strcmp(ext, ".png") == 0) {
			return PNG;
		}
		else if (strcmp(ext, ".jpg") == 0) {
			return JPG;
		}
		else if (strcmp(ext, ".bmp") == 0) {
			return BMP;
		}
		else if (strcmp(ext, ".tga") == 0) {
			return TGA;
		}
	}

	throw std::invalid_argument("Invalid filetype");
}

bool Image::read(const char* filename) {
	data = stbi_load(filename, &w, &h, &channels, 0);
	return data != NULL;
}

bool Image::write(const char* filename) {
	ImageType type = getFileType(filename);
	int succes;

	switch (type)
	{
	case PNG:
		succes = stbi_write_png(filename, w, h, channels, data, w * channels);
		break;
	case BMP:
		succes = stbi_write_bmp(filename, w, h, channels, data);
		break;
	case JPG:
		succes = stbi_write_jpg(filename, w, h, channels, data, 100);
		break;
	case TGA:
		succes = stbi_write_tga(filename, w, h, channels, data);
		break;
	default:
		succes = 0;
		break;
	}

	return succes != 0;
}

unsigned int Image::getPixel(int x, int y, u32 color) {
	int index = size - w * channels - y * channels * w + x * channels;

	u32 rgb[] = {
		(u32)data[index],
		(u32)data[index + 1],
		(u32)data[index + 2],
	};

	if (channels > 3) {
		double a = 1 - ((double)data[index + 3] / 0xFF);
		u32 colors[3] = {};
		u32 temp = color;

		for (int i = 2; i >= 0; i--) {
			colors[i] = temp % 256;
			temp /= 256;
		}
		for (int i = 0; i < 3; i++) {
			int d = rgb[i] - colors[i];
			d = (int)floor((double)d * a);
			rgb[i] -= d;
		}
		
	}

	u32 result = (rgb[0] << 16) | (rgb[1] << 8) | (rgb[2] << 0);

	return result;
}
