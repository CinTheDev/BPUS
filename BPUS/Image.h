#pragma once

#include <stdint.h>
#include <cstdio>
#include "utils.cpp"

enum ImageType {
	PNG, BMP, JPG, TGA
};

struct Image {
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;

	Image();
	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image& img);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);

	ImageType getFileType(const char* filename);

	u32 getPixel(int x, int y, u32 color);
	u32 getPixel(Vector2Int p, u32 color);
};
