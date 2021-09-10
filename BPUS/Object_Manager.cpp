#include "Object_Manager.h"

std::vector<Object*> Obj_M::remove_obj;
std::vector<Object*> Obj_M::objects;

void Obj_M::create(Object* obj) {
	objects.push_back(obj);
	obj->init();
}

void Obj_M::remove(Object* obj) {
	remove_obj.push_back(obj);
}

void Obj_M::destroy(Object* obj, int index) {
	remove_obj.erase(remove_obj.begin() + index);

	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] == obj) {
			objects.erase(objects.begin() + i);
			break;
		}
	}

	obj->~Object();
}

void Obj_M::objects_update(Input* input, float dt) {
	// Call update for every object
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->update(input, dt);
	}

	// Destroy every object that has been flagged
	for (int i = 0; i < remove_obj.size(); i++) {
		Object* o = remove_obj[i];
		destroy(o, i);
	}
}
