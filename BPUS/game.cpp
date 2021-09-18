Image test("test2.jpg");
Image alphatest("test3.png");

#include "Object_behavior.cpp"

Object* obj;
Object* obj2;

internal void
init() {
	obj = new Obj::Empty(Vector2(0, 0), &test, 1.f);
	obj->z = 1;
	obj2 = new Obj::Baseobject(Vector2(500, 0), &alphatest, 1.f);
	obj2->z = 0;
	Obj_M::create(obj);
	Obj_M::create(obj2);
}

internal void
simulate(Input* input, float dt) {
	draw_background(0xAAABBA);

	Obj_M::objects_update(input, dt);

	draw_rect(Vector2(300, 100), Vector2(100, 300), 0xAA0055);
	draw_tri(Vector2(100, 100), Vector2(200, 150), Vector2(180, 250), 0x00AA55);
	draw_cir(Vector2(300, 100), 50, 0x55AA00);
	//draw_oval(Vector2(400, 100), Vector2Int(50, 100), 0x5500AA);

	draw_objects();
}
