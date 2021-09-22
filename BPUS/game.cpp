Image empty(0, 0, 0);
Image test("test2.jpg");
Image alphatest("test3.png");

#include "Object_behavior.cpp"

Object* obj;
Object* obj2;

internal void
init() {
	// Camera
	camera = new Obj::Camera(Vector2(0, 0), &empty, 1.f);
	Obj_M::create(camera);

	//obj = new Obj::Empty(Vector2(0, 0), &test, 1.f);
	//obj->z = 1;
	obj2 = new Obj::Baseobject(Vector2(500, 0), &alphatest, 1.f);
	obj2->z = 0;
	//Obj_M::create(obj);
	Obj_M::create(obj2);
}

internal void
simulate(Input* input, float dt) {
	draw_background(0x000011);

	draw_rect(Vector2(300, 100), Vector2(100, 300), 0xAA0055);
	draw_tri(Vector2(100, 100), Vector2(200, 150), Vector2(180, 250), 0x00AA55);
	draw_cir(Vector2(300, 100), 50, 0x55AA00);
	draw_text(Vector2(500, 200), "The Quick Brown Fox Jumps Over The Lazy Dog\nTHE QUICK BROWN FOX JUMPS OVER THE LAZY DOG\nthe quick brown fox jumps over the lazy dog", 2.5f);

	// Debug
	std::string s = "x: " + std::to_string(camera->position.x) + "; y: " + std::to_string(camera->position.y);
	draw_text(camera->position + Vector2(5, 20), s.c_str(), 2.f);

	draw_objects();

	Obj_M::objects_update(input, dt);
}
