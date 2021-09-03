internal void
draw_background(u32 color) {
	unsigned int* pixel = (u32*)renderState.memory;
	for (int y = 0; y < renderState.height; y++) {
		for (int x = 0; x < renderState.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void
draw_rect_pixel(int x0, int y0, int x1, int y1, u32 color) {
	x0 = clamp(0, x0, renderState.width);
	x1 = clamp(0, x1, renderState.width);
	y0 = clamp(0, y0, renderState.height);
	y1 = clamp(0, y1, renderState.height);

	for (int y = y0; y < y1; y++) {
		u32* pixel = (u32*)renderState.memory + x0 + y * renderState.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

internal void
draw_tri_pixel(int x0, int y0, int x1, int y1, int x2, int y2, u32 color) {
	// For later
}

internal void
draw_cir_pixel() {
	// For later
}

global_variable float renderScale = 0.01f;

internal void
draw_rect(float x, float y, float halfsize_x, float halfsize_y, u32 color) {
	x *= renderState.height * renderScale;
	y *= renderState.height * renderScale;
	halfsize_x *= renderState.height * renderScale;
	halfsize_y *= renderState.height * renderScale;

	x += renderState.width / 2.f;
	y += renderState.height / 2.f;

	// Change to pixels
	int x0 = x - halfsize_x;
	int x1 = x + halfsize_x;
	int y0 = y - halfsize_y;
	int y1 = y + halfsize_y;

	draw_rect_pixel(x0, y0, x1, y1, color);
}

#include "Image.h"

internal void
draw_image(Image image, int offset_x, int offset_y, float scale) {
	for (int y = offset_y; y < floor((double)image.h * (double)scale) + offset_y; y++) {
		if (y >= renderState.height || y <= 0) continue;
		u32* pixel = (u32*)renderState.memory + offset_x + y * renderState.width;
		for (int x = offset_x; x < floor((double)image.w * (double)scale) + offset_x; x++) {
			if (x >= renderState.width || x <= 0) *pixel++;
			else *pixel++ = image.getPixel(floor((x - offset_x) / scale), floor((y - offset_y) / scale));
		}
	}
}