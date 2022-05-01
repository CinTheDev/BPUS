#pragma once

#include <vector>
#include "../../renderer/texture/texture.h"

struct updateArguments {
    GLFWwindow* window;

    float deltatime;
};

#include "../component.h"

class object {
private:
    void objectInit();
    void objectUpdate(updateArguments args);
    
    friend class obj_m;

public:
    Vector2 position;
    Vector2 size;
    float rotation;

    texture* image;
    int z;

    std::vector<component*> components;

    object(Vector2 pos, texture* img, Vector2 s, float r);
    object(Vector2 pos, texture* img, Vector2 s);
    object(Vector2 pos, texture* img);
    object(Vector2 pos);
    object();
    ~object();

    bool operator==(const object& o);
    bool operator!=(const object& o);

    void addComponent(component* comp);
    void updateComponents(updateArguments args);

    // Template functions cannot be defined in the .cpp file, so I did it here
    template<typename T>
    T* getComponent() {
        return getComponent<T>(true);
    }
    template<typename T>
    T* getComponent(bool warning) {
        for (auto& i : components) {
            // Check if component has the given type or inherits from it
            if (dynamic_cast<T*>(i)) return (T*)i;
        }

        if (warning) std::cout << "\033[33mWARNING: Component " << typeid(T).name() << " not found.\033[m" << std::endl;
        return nullptr;
    }

    virtual void init();
    virtual void update(updateArguments args);
};
