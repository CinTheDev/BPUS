#pragma once

//#include "utils.cpp"
#include "../../renderer/texture/texture.h"

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

    object(Vector2 pos, texture* img, Vector2 s, float r);
    object(Vector2 pos, texture* img, Vector2 s);
    object(Vector2 pos, texture* img);
    object(Vector2 pos);
    ~object();

    bool operator==(const object& o);
    bool operator!=(const object& o);

    virtual void init();
    virtual void update(updateArguments args);
};
