#pragma once

#include <vector>
#include "../../renderer/texture/texture.h"
#include "../component/component.h"

struct updateArguments {
    GLFWwindow* window;

    float deltatime;
};

class object {
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
    ~object();

    bool operator==(const object& o);
    bool operator!=(const object& o);

    void addComponent(component* comp);
    void updateComponents();

    virtual void init();
    virtual void update(updateArguments args);
};
