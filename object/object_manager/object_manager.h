#pragma once

#include <vector>
#include "../object/object.h"
#include "../component_standard.cpp"

class obj_m {
private:
    static std::vector<object*> remove_obj;

    static void destroy(object* obj, int index);

    static void lstupdate();

public:
    static std::vector<object*> objects;

    static std::vector<comp::collider*> colliders;

    static void create(object* obj);

    static object* create(Vector2 position, Vector2 size, texture* texture);

    static void remove(object* obj);

    static void registerCollider(comp::collider* coll);

    static void removeCollider(comp::collider* coll);

    template<class T>
    static std::vector<T*> getCollider() {
        const std::type_info* class_type = &typeid(T);
        std::vector<T*> result = std::vector<T*>();
        for (auto& c : obj_m::colliders) {
            if (typeid(*c) == *class_type) {
                result.push_back((T*)c);
            }
        }
        return result;
    }

    static void objects_update(updateArguments args);

    static void stop();
};
