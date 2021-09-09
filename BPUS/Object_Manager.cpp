#include "Object_Manager.h"

std::vector<Object*> Obj_M::objects;

void Obj_M::create(Object* obj) {
	objects.push_back(obj);
	(*obj->init)(obj);
}

void Obj_M::destroy(Object* obj) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] == obj) {
			objects.erase(objects.begin() + i);
			break;
		}
	}
	obj->~Object();
}

void Obj_M::objects_update(Input* input, float dt) {
	for (int i = 0; i < objects.size(); i++) {
		(*objects[i]->update)(objects[i], input, dt);
	}
}