#pragma once

#include <vector>
#include "../object/object.h"
#include "../component_standard.cpp"

class obj_m {
private:
    static std::vector<object*> remove_obj;

    static void clear();

    static void lstupdate();

public:
    static std::vector<object*> objects;

    static std::vector<comp::collider_circle*> circle_colliders;
    static std::vector<comp::collider_line*> line_colliders;
    static std::vector<comp::collider_rect*> rect_colliders;

    static void create(object* obj);

    static object* create(Vector2 position, Vector2 size, texture* texture);

    static void remove(object* obj);

    static void registerCircle(comp::collider_circle* coll);
    static void registerLine(comp::collider_line* coll);
    static void registerRect(comp::collider_rect* coll);

    static void removeCircle(comp::collider_circle* coll);
    static void removeLine(comp::collider_line* coll);
    static void removeRect(comp::collider_rect* coll);

    static void objects_update(updateArguments args);

    static void stop();
};
