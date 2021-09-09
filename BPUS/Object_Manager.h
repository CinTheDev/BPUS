#include <vector>
#include "Object.h"

#pragma once
static class Obj_M
{
public:
	static std::vector<Object*> objects;

	static void create(Object* obj);

	static void destroy(Object* obj);

	static void objects_update(Input* input, float dt);
};

