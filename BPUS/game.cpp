#include "Object_behavior.cpp"

Image test("test2.jpg");
Object* obj;
Object* obj2;

internal void
init() {
	obj = new Object(Vector2(0, 0), &test, 1.f, &Obj_INIT::empty, &Obj_UPD::baseobject);
	obj2 = new Object(Vector2(500, 0), &test, 1.f, &Obj_INIT::empty, &Obj_UPD::empty);
	Obj_M::create(obj);
	Obj_M::create(obj2);
}

internal void
simulate(Input* input, float dt) {
	draw_background(0x000110);

	Obj_M::objects_update(input, dt);

	draw_objects();
}