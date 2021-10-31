#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include "Image.h"
#include <stb_image.h>
#include <stdexcept>

Image::Image() {
	stbi_set_flip_vertically_on_load(true);
	w = 0;
	h = 0;
	channels = 0;
	data = new uint8_t[0];
}

Image::Image(const char* filename) {
	stbi_set_flip_vertically_on_load(true);
	if (read(filename)) {
		size = w * h * channels;
	}
	else {
		throw std::invalid_argument("File not found");
	}
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
	stbi_set_flip_vertically_on_load(true);
	size = w * h * channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
	stbi_set_flip_vertically_on_load(true);
	memcpy(data, img.data, img.size);
}

Image::~Image() {
	stbi_image_free(data);
}

bool Image::read(const char* filename) {
	data = stbi_load(filename, &w, &h, &channels, 0);
	return data != NULL;
}
