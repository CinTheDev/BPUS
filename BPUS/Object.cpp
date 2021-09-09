#include "Object.h"

Object::Object() {
	position = Vector2(0, 0);
	size = 0;
}

Object::Object(Vector2 pos, Image* img, int s) {
	position = pos;
	image = img;
	size = s;
}

Object::~Object() {
	
}