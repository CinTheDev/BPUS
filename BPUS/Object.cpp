#include "Object.h"

Object::Object(Vector2 pos, Texture* img, Vector2 s, float r) {
	position = pos;
	image = img;
	rotation = r;
	size = s;
	z = 0;
}

Object::Object(Vector2 pos, Texture* img, Vector2 s) : Object(pos, img, s, 0) {
}

Object::Object(Vector2 pos, Texture* img) : Object(pos, img, Vector2(1, 1)) {
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

void Object::init() {}

void Object::update(UpdateArguments args) {}
