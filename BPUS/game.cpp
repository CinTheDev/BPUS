#define isdown(b) input -> buttons[b].isdown
#define pressed(b) (input -> buttons[b].isdown && input -> buttons[b].changed)
#define released(b) (!input -> buttons[b].isdown && input -> buttons[b].changed)

Image test("test.jpg");

internal void
init() {

}

float scale = 1.f;
Vector2 pos(0, 0);

internal void
simulate(Input* input, float dt) {
	draw_background(0x000110);

	if (isdown(BUTTON_UP)) scale *= 1.01f;
	if (isdown(BUTTON_DOWN)) scale *= 0.99f;

	if (isdown(BUTTON_W)) pos.y += 200. * (double)dt;
	if (isdown(BUTTON_A)) pos.x -= 200. * (double)dt;
	if (isdown(BUTTON_S)) pos.y -= 200. * (double)dt;
	if (isdown(BUTTON_D)) pos.x += 200. * (double)dt;

	draw_image(test, floor(pos.x), floor(pos.y), scale);
}