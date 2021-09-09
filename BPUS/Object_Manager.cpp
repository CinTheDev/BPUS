#include "Object_Manager.h"

std::vector<Object*> Obj_M::objects;

void Obj_M::create(Object* obj) {
	objects.push_back(obj);
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