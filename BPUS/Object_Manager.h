#include <vector>
#include "Object.h"

#pragma once
static class Obj_M
{
private:
	static std::vector<Object*> remove_obj;

	static void destroy(Object* obj, int index);

	static void lstupdate();
public:
	static std::vector<Object*> objects;

	static void create(Object* obj);

	static void remove(Object* obj);

	static void objects_update(Input* input, float dt);
};
