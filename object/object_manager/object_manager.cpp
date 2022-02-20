#include "object_manager.h"

std::vector<object*> obj_m::remove_obj;
std::vector<object*> obj_m::objects;

void obj_m::create(object* obj) {
    // Put new object in list
    objects.push_back(obj);
    obj->objectInit();
    lstupdate();
}

object* obj_m::create(Vector2 position, Vector2 size, texture* texture) {
    object* obj = new object(position, texture, size);
    create(obj);
    return obj;
}

void obj_m::remove(object* obj) {
    // Flag item to remove
    remove_obj.push_back(obj);
}

void obj_m::destroy(object* obj, int index) {
    remove_obj.erase(remove_obj.begin() + index);

    // Remove object from actual objects list
    for (unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i] == obj) {
            objects.erase(objects.begin() + i);
            break;
        }
    }

    obj->~object();
}

void obj_m::objects_update(updateArguments args) {
    // Call update for every object
    for (unsigned int i = 0; i < objects.size(); i++) {
        objects[i]->objectUpdate(args);
    }

    // Destroy every object that has been flagged
    for (unsigned int i = 0; i < remove_obj.size(); i++) {
        destroy(remove_obj[i], i);
    }
}

void obj_m::stop() {
    remove_obj.clear();
    objects.clear();
}

#include <algorithm>

bool srt(const object* a, const object* b) {
    if (a->z > b->z) return true;
    return false;
}

void obj_m::lstupdate() {
    // Sorts the list every time it is changed
    std::stable_sort(objects.begin(), objects.end(), srt);
}
