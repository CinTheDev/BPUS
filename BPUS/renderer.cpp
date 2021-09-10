#include "Object_Manager.h"

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
draw_rect_pixel(Vector2Int p0, Vector2Int p1, u32 color) {
	p0.x = clamp(0, p0.x, renderState.width);
	p1.x = clamp(0, p1.x, renderState.width);
	p0.y = clamp(0, p0.y, renderState.height);
	p1.y = clamp(0, p1.y, renderState.height);

	for (int y = p0.y; y < p1.y; y++) {
		u32* pixel = (u32*)renderState.memory + p0.x + y * renderState.width;
		for (int x = p0.x; x < p1.x; x++) {
			*pixel++ = color;
		}
	}
}

internal void
draw_cir_pixel(Vector2Int p0, Vector2Int p1, u32 color) {
	// For later
}

internal void
draw_tri_pixel(Vector2Int p0, Vector2Int p1, Vector2Int p2, u32 color) {
	// For later
}

internal void
draw_image_pixel(Image* image, Vector2Int offset, float scale) {
	for (int y = offset.y; y < floor((double)image->h * (double)scale) + offset.y; y++) {
		if (y >= renderState.height || y <= 0) continue;
		u32* pixel = (u32*)renderState.memory + offset.x + y * renderState.width;
		for (int x = offset.x; x < floor((double)image->w * (double)scale) + offset.x; x++) {
			if (x >= renderState.width || x <= 0) *pixel++;
			else *pixel++ = image->getPixel(floor((x - offset.x) / scale), floor((y - offset.y) / scale));
		}
	}
}

global_variable float renderScale = 0.01f;
Vector2 campos(0, 0);

internal void
draw_rect(Vector2 p, Vector2 size, u32 color) {
	p -= campos;
	Vector2Int p0(floor(p.x), floor(p.y));
	Vector2Int p1(floor(p.x + size.x), floor(p.y + size.y));

	draw_rect_pixel(p0, p1, color);
}

internal void
draw_cir(Vector2 p, Vector2 size, u32 color) {
	p -= campos;
	Vector2Int p0(floor(p.x), floor(p.y));
	Vector2Int p1(floor(p.x + size.x), floor(p.y + size.y));

	draw_cir_pixel(p0, p1, color);
}

internal void
draw_tri(Vector2 p0, Vector2 p1, Vector2 p2, u32 color) {
	p0, p1, p2 -= campos;
	Vector2Int p0_1(floor(p0.x), floor(p0.y));
	Vector2Int p1_1(floor(p1.x), floor(p1.y));
	Vector2Int p2_1(floor(p2.x), floor(p2.y));

	draw_tri_pixel(p0_1, p1_1, p2_1, color);
}

internal void
draw_image(Image* image, Vector2 p, float scale) {
	p -= campos;
	draw_image_pixel(image, Vector2Int((int)floor(p.x), (int)floor(p.y)), scale);
}

internal void
draw_objects() {
	for (unsigned int i = 0; i < Obj_M::objects.size(); i++) {
		Object* o = Obj_M::objects[i];
		draw_image(o->image, o->position, o->size);
	}
}
