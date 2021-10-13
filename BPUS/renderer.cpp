#include "Object_Manager.h"

Object* camera;

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

bool outside_screen(Vector2 p) {
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
	int sy = min3(p0.y, p1.y, p2.y), by = max3(p0.y, p1.y, p2.y);
	int sx = min3(p0.x, p1.x, p2.x), bx = max3(p0.x, p1.x, p2.x);

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

void set_pixel(Vector2Int pos, Vector2Int pix, Image* img, float rotation, Vector2Int pivot) {
	Vector2 rot = pos.todouble() - pivot.todouble();
	rot.rotate(rotation);
	Vector2 point = pivot.todouble() + rot;
	Vector2Int pointint(floor(point.x), floor(point.y));

	if (outside_screen(pointint.todouble())) return;

	u32* pixel = (u32*)renderState.memory + pointint.x + pointint.y * renderState.width;
	*pixel = img->getPixel(pix.x, pix.y, *pixel);
}

static void
draw_image_pixel(Image* image, Vector2Int offset, float scale, float rotation, Vector2Int pivot) {
	int height = floor((double)image->h * (double)scale);
	int width = floor((double)image->w * (double)scale);
	for (int y = offset.y; y < height + offset.y; y++) {

		for (int x = offset.x; x < width + offset.x; x++) {
			int px = floor((x - offset.x) / scale);
			int py = floor((y - offset.y) / scale);

			set_pixel(Vector2Int(x, y), Vector2Int(px, py), image, rotation, pivot);
		}
	}
}

#pragma endregion

#pragma region Shapes

static void
draw_rect(Vector2 p, Vector2 size, u32 color) {
	p -= camera->position;
	Vector2Int p0((int)floor(p.x), (int)floor(p.y));
	Vector2Int p1((int)floor(p.x + size.x), (int)floor(p.y + size.y));

	draw_rect_pixel(p0, p1, color);
}

static void
draw_cir(Vector2 p, int radius, u32 color) {
	p -= camera->position;
	Vector2Int p0((int)floor(p.x), (int)floor(p.y));

	draw_cir_pixel(p0, radius, color);
}

static void
draw_oval(Vector2 p, Vector2Int size, u32 color) {
	p -= camera->position;
	Vector2Int p0((int)floor(p.x), (int)floor(p.y));

	draw_oval_pixel(p0, size, color);
}

static void
draw_tri(Vector2 p0, Vector2 p1, Vector2 p2, u32 color) {
	p0 -= camera->position;
	p1 -= camera->position;
	p2 -= camera->position;
	Vector2Int p0_1((int)floor(p0.x), (int)floor(p0.y));
	Vector2Int p1_1((int)floor(p1.x), (int)floor(p1.y));
	Vector2Int p2_1((int)floor(p2.x), (int)floor(p2.y));

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
	p -= camera->position;
	if (outside_screen(p, Vector2(image->w * scale, image->h * scale))) return;
	Vector2Int pivint((int)floor(pivot.x), (int)floor(pivot.y));
	draw_image_pixel(image, Vector2Int((int)floor(p.x), (int)floor(p.y)), scale, rotation, pivint);
}

static void
draw_image(Object o) {
	o.position -= camera->position;
	if (outside_screen(o.position, Vector2(o.image->w * o.size, o.image->h * o.size))) return;
	Vector2Int pivint((int)floor(o.getPivAbs().x), (int)floor(o.getPivAbs().y));
	draw_image_pixel(o.image, Vector2Int((int)floor(o.position.x), (int)floor(o.position.y)), o.size, o.rotation, pivint);
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
