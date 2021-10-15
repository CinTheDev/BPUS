#include "Object_Manager.h"
#include "Object_behavior.cpp"

Obj::Camera* camera;

#pragma region Screen functions

static void
draw_background(u32 color) {
	unsigned int* pixel = (u32*)renderState.memory;
	for (int y = 0; y < renderState.height; y++) {
		for (int x = 0; x < renderState.width; x++) {
			*pixel++ = color;
		}
	}
}

bool outside_screen(Vector2 p, Vector2 size) {
	if (p.x > renderState.width || p.x + size.x < 0) return true;
	if (p.y > renderState.height || p.y + size.y < 0) return true;
	return false;
}

bool outside_screen(Vector2Int p) {
	if (p.x <= 0 || p.x >= renderState.width) return true;
	if (p.y <= 0 || p.y >= renderState.height) return true;
	return false;
}

#pragma endregion

#pragma region Pixel renderers

static void
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

static void
draw_cir_pixel(Vector2Int p, int radius, u32 color) {
	for (int y = p.y - radius; y < p.y + radius; y++) {
		if (y >= renderState.height || y <= 0) continue;
		u32* pixel = (u32*)renderState.memory + (p.x - radius) + y * renderState.width;
		for (int x = p.x - radius; x < p.x + radius; x++) {
			Vector2Int l = Vector2Int(x, y) - p;
			if (l.sqrlen() > radius*radius || (x >= renderState.width || x <= 0)) pixel++;
			else *pixel++ = color;
		}
	}
}

// Formula from https://math.stackexchange.com/questions/432902/how-to-get-the-radius-of-an-ellipse-at-a-specific-angle-by-knowing-its-semi-majo
// Also performance is terrible
bool inside_oval(Vector2Int l, Vector2Int axis) {
	double angle = atan2(l.y, l.x);
	double nom = (double)axis.x * (double)axis.y;
	double den = sqrt(pow(axis.x, 2) * pow(sin(angle), 2) + pow(axis.y, 2) * pow(cos(angle), 2));
	double r = nom / den;
	return l.sqrlen() <= r*r;
}

static void
draw_oval_pixel(Vector2Int p, Vector2Int radius, u32 color) {
	for (int y = p.y - radius.y; y < p.y + radius.y; y++) {
		if (y >= renderState.height || y <= 0) continue;
		u32* pixel = (u32*)renderState.memory + p.x + y * renderState.width;
		for (int x = p.x - radius.x; x < p.x + radius.x; x++) {
			Vector2Int l = Vector2Int(x, y) - p;
			if (inside_oval(l, radius) && !(x >= renderState.width || x <= 0)) *pixel++ = color;
			else pixel++;
		}
	}
}

// sign_tri and draw_tri_pixel from https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
int sign_tri(Vector2Int p0, Vector2Int p1, Vector2Int p2) {
	return (p0.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p0.y - p2.y);
}

static void
draw_tri_pixel(Vector2Int p0, Vector2Int p1, Vector2Int p2, u32 color) {
	int a[] = {p0.x, p1.x, p2.x};
	int b[] = { p0.y, p1.y, p2.y };
	int sx = min_array(a, sizeof(a) / sizeof(*a)), bx = max_array(a, sizeof(a) / sizeof(*a));
	int sy = min_array(b, sizeof(b) / sizeof(*b)), by = max_array(b, sizeof(b) / sizeof(*b));

	int d1, d2, d3;
	bool has_neg, has_pos;

	for (int y = sy; y < by; y++) {
		if (y >= renderState.height || y <= 0) continue;
		u32* pixel = (u32*)renderState.memory + sx + y * renderState.width;
		for (int x = sx; x < bx; x++) {
			d1 = sign_tri(Vector2Int(x, y), p0, p1);
			d2 = sign_tri(Vector2Int(x, y), p1, p2);
			d3 = sign_tri(Vector2Int(x, y), p2, p0);

			has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
			has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

			if (!(has_neg && has_pos) && !(x >= renderState.width || x <= 0)) *pixel++ = color;
			else pixel++;
		}
	}
}

static void
draw_image_pixel(Image* image, Vector2Int offset, float scale, float rotation, Vector2Int pivot) {
	// Define and round image sizes
	int height = floor((double)image->h * (double)scale);
	int width = floor((double)image->w * (double)scale);
	for (int y = offset.y; y < height + offset.y; y++) {
		// Get pointer to buffer
		u32* pixel = (u32*)renderState.memory + offset.x + y * renderState.width;
		for (int x = offset.x; x < width + offset.x; x++) {
			// If the pixel is outside buffer
			if (outside_screen(Vector2Int(x, y))) pixel++;
			else {
				// Rotate position backwards to get color
				Vector2 rot = Vector2(x, y) - pivot.todouble();
				rot = rot.rotate(-rotation);
				Vector2 point = pivot.todouble() + rot;
				Vector2Int pointint(floor(point.x), floor(point.y));
				// If rotated vector is outside image
				if (pointint.x - offset.x <= 0 || pointint.x - offset.x >= width
					|| pointint.y - offset.y <= 0 || pointint.y - offset.y >= height) {
					pixel++;
					continue;
				}
				// Color the pixel and increment
				*pixel++ = image->getPixel(floor((pointint.x - offset.x) / scale), floor((pointint.y - offset.y) / scale), *pixel);
			}
		}
	}
}

#pragma endregion

#pragma region Shapes

static Vector2Int
camOperations(Vector2 point) {
	point -= camera->position;
	Vector2 point_diff = (point - camera->middleOfScreen().todouble()) * camera->getZoom();
	point = camera->middleOfScreen().todouble() + point_diff;

	return Vector2Int((int)floor(point.x), (int)floor(point.y));
}

static void
draw_rect(Vector2 p, Vector2 size, u32 color) {
	Vector2Int p0 = camOperations(p);
	Vector2Int p1 = camOperations(p + size);

	draw_rect_pixel(p0, p1, color);
}

static void
draw_cir(Vector2 p, int radius, u32 color) {
	Vector2Int p0 = camOperations(p);

	draw_cir_pixel(p0, radius * camera->getZoom(), color);
}

static void
draw_oval(Vector2 p, Vector2Int size, u32 color) {
	// Didn't feel like implementing camera operations
	p -= camera->position;
	Vector2Int p0((int)floor(p.x), (int)floor(p.y));

	draw_oval_pixel(p0, size, color);
}

static void
draw_tri(Vector2 p0, Vector2 p1, Vector2 p2, u32 color) {
	Vector2Int p0_1 = camOperations(p0);
	Vector2Int p1_1 = camOperations(p1);
	Vector2Int p2_1 = camOperations(p2);

	draw_tri_pixel(p0_1, p1_1, p2_1, color);
}

#pragma endregion

#pragma region Text

#include "font.cpp"

static void
draw_text(Vector2 pos, const char* text, float scale) {
	float org_y = pos.y;
	float org_org_x = pos.x;

	while (*text) {
		const char** a_letter = letters[*text - ' '];
		float org_x = pos.x;
		int width = 0;

		if (*text == 10) {
			pos.x = org_org_x - scale * 2;
			org_x = pos.x;
			org_y -= scale * 10.f;
			pos.y = org_y;
		}
		else {
			for (int i = 0; i < 7; i++) {
				int _width = 0;
				const char* row = a_letter[i];
				while (*row) {
					if (*row == '0') {
						draw_rect(Vector2(pos.x, pos.y), Vector2(scale, scale), 0xFFFFFF);
					}
					pos.x += scale;
					_width++;
					row++;
				}
				pos.y -= scale;
				width = max(width, _width);
				pos.x = org_x;
			}
		}
		text++;
		pos.x += width * scale + scale * 1.75f;
		pos.y = org_y;
	}
}

#pragma endregion

#pragma region Images

static void
draw_image(Image* image, Vector2 p, float scale, float rotation, Vector2 pivot) {

	Vector2Int pos = camOperations(p);
	Vector2Int pivint = camOperations(pivot);

	if (outside_screen(pos.todouble(), Vector2(image->w * scale * camera->getZoom(), image->h * scale * camera->getZoom()))) return;
	
	draw_image_pixel(image, pos, scale * camera->getZoom(), rotation, pivint);
}

static void
draw_image(Object o) {
	draw_image(o.image, o.position, o.size, o.rotation, o.getPivAbs());
}

#pragma endregion

#pragma region Convinience

static void
draw_objects() {
	for (unsigned int i = 0; i < Obj_M::objects.size(); i++) {
		Object o = *Obj_M::objects[i];
		draw_image(o);
	}
}

static void
render() {
	draw_background(0x000011);

	draw_rect(Vector2(300, 100), Vector2(100, 300), 0xAA0055);
	draw_tri(Vector2(100, 100), Vector2(200, 150), Vector2(180, 250), 0x00AA55);
	draw_cir(Vector2(300, 100), 50, 0x55AA00);
	draw_text(Vector2(500, 200), "The Quick Brown Fox Jumps Over The Lazy Dog\nTHE QUICK BROWN FOX JUMPS OVER THE LAZY DOG\nthe quick brown fox jumps over the lazy dog", 2.5f);

	draw_objects();
}

#pragma endregion
