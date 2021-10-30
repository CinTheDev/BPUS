#include "Object_Manager.h"

std::vector<Object*> Obj_M::remove_obj;
std::vector<Object*> Obj_M::objects;

void Obj_M::create(Object* obj) {
	// Put new object in list
	objects.push_back(obj);
	obj->init();
}

Object* Obj_M::create(Vector2 position, Vector2 size, Texture* texture) {
	Object* obj = new Object(position, texture, size);
	create(obj);
	return obj;
}

void Obj_M::remove(Object* obj) {
	// Flaggs item to remove
	remove_obj.push_back(obj);
}

void Obj_M::destroy(Object* obj, int index) {
	// Removes item from list
	remove_obj.erase(remove_obj.begin() + index);

	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i] == obj) {
			objects.erase(objects.begin() + i);
			break;
		}
	}

	obj->~Object();
}

void Obj_M::objects_update(UpdateArguments args) {
	// Call update for every object
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->update(args);
	}

	// Destroy every object that has been flagged
	for (unsigned int i = 0; i < remove_obj.size(); i++) {
		Object* o = remove_obj[i];
		destroy(o, i);
	}
}
