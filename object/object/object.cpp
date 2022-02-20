#include "object.h"

object::object(Vector2 pos, texture* img, Vector2 s, float r) {
    position = pos;
    image = img;
    rotation = r;
    size = s;
    z = 0;
}

object::object(Vector2 pos, texture* img, Vector2 s) : object(pos, img, s, 0) {}

object::object(Vector2 pos, texture* img) : object(pos, img, Vector2(1, 1)) {}

object::object(Vector2 pos) : object(pos, NULL) {}

object::~object() {
    position.~Vector2();
}

bool object::operator==(const object& o) {
    return this == &o;
}

bool object::operator!=(const object& o) {
    return this != &o;
}

void object::addComponent(component* component) {
    components.push_back(component);
    component->init();
}

void object::updateComponents(updateArguments args) {
    for (int i = 0; i < components.size(); i++) {
        if (!components[i]->enabled) continue;
        components[i]->update(args);
    }
}

void object::objectInit() {
    init();
}
void object::objectUpdate(updateArguments args) {
    updateComponents(args);

    update(args);
}

void object::init() {}
void object::update(updateArguments args) {}
