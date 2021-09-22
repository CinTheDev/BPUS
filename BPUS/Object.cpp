#include "Object.h"

Object::Object(Vector2 pos, Image* img, float s, float r) {
	position = pos;
	image = img;
	rotation = r;
	size = s;
	z = 0;
	centerPivot();
}

Object::Object(Vector2 pos, Image* img, float s) : Object(pos, img, s, 0) {
}

Object::Object(Vector2 pos, Image* img) : Object(pos, img, 1.f) {
}

Object::Object(Vector2 pos) : Object(pos, new Image()) {
}

Object::Object() : Object(Vector2(0, 0)) {
}

Object::~Object() {
	position.~Vector2();
}

bool Object::operator==(const Object& o) {
	return this == &o;
}
bool Object::operator!=(const Object& o) {
	return this != &o;
}

void Object::centerPivot() {
	pivot = Vector2(image->h * size / 2, image->w * size / 2);
}

void Object::init() {}

void Object::update(Input* input, double dt) {}
