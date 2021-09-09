#include <stdint.h>
#include <cstdio>

#pragma once
enum ImageType {
	PNG, BMP, JPG, TGA
};

#pragma once
struct Image {
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;

	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image& img);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);

	ImageType getFileType(const char* filename);

	unsigned int getPixel(int x, int y);

	Image& grayscale();
};