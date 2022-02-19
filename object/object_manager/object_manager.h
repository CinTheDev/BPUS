#pragma once

#include <vector>
#include "../object/object.h"

class obj_m {
private:
    static std::vector<object*> remove_obj;

    static void destroy(object* obj, int index);

    static void lstupdate();

public:
    static std::vector<object*> objects;

    static void create(object* obj);

    static object* create(Vector2 position, Vector2 size, texture* texture);

    static void remove(object* obj);

    static void objects_update(updateArguments args);

    static void stop();
};
