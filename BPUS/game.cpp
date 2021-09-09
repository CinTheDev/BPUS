#define isdown(b) input -> buttons[b].isdown
#define pressed(b) (input -> buttons[b].isdown && input -> buttons[b].changed)
#define released(b) (!input -> buttons[b].isdown && input -> buttons[b].changed)

Image test("test2.jpg");
Object* obj;
Object* obj2;

internal void
init() {
	obj = new Object(Vector2(0, 0), &test, 1);
	obj2 = new Object(Vector2(500, 0), &test, 1);
	Obj_M::create(obj);
	Obj_M::create(obj2);
}

internal void
simulate(Input* input, float dt) {
	draw_background(0x000110);

	/*if (isdown(BUTTON_UP)) scale *= 1.01f;
	if (isdown(BUTTON_DOWN)) scale *= 0.99f;

	if (isdown(BUTTON_W)) pos.y += 200. * (double)dt;
	if (isdown(BUTTON_A)) pos.x -= 200. * (double)dt;
	if (isdown(BUTTON_S)) pos.y -= 200. * (double)dt;
	if (isdown(BUTTON_D)) pos.x += 200. * (double)dt;

	draw_image(test, floor(pos.x), floor(pos.y), scale);*/

	draw_objects();
}