#pragma once

#include <vector>
#include "Object.h"

static class Obj_M
{
private:
	static std::vector<Object*> remove_obj;

	static void destroy(Object* obj, int index);

	static void lstupdate();

public:
	static std::vector<Object*> objects;

	static void create(Object* obj);

	static Object* create(Vector2 position, Vector2 size, Texture* texture);

	static void remove(Object* obj);

	static void objects_update(UpdateArguments args);
};
