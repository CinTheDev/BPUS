#pragma once

#include <stdint.h>
#include <cstdio>
#include "../utils.cpp"

struct image {
    uint8_t* data = NULL;
    size_t size = 0;
    int w;
    int h;
    int channels;

    image();
    image(const char* filename);
    image(int w, int h, int channels);
    image(const image& img);
    ~image();

    bool read(const char* filename);
};
