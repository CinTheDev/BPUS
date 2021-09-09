#define isdown(b) input -> buttons[b].isdown
#define pressed(b) (input -> buttons[b].isdown && input -> buttons[b].changed)
#define released(b) (!input -> buttons[b].isdown && input -> buttons[b].changed)

#pragma once
internal struct Obj_INIT {
	internal void empty(Object* ths) {}
};

#pragma once
internal struct Obj_UPD {
	internal void empty(Object* ths, Input* input, float dt) {}

	internal void baseobject(Object* ths, Input* input, float dt) {
		if (isdown(BUTTON_UP)) ths->size *= 1.01f;
		if (isdown(BUTTON_DOWN)) ths->size *= 0.99f;

		if (isdown(BUTTON_W)) ths->position.y += 200. * (double)dt;
		if (isdown(BUTTON_A)) ths->position.x -= 200. * (double)dt;
		if (isdown(BUTTON_S)) ths->position.y -= 200. * (double)dt;
		if (isdown(BUTTON_D)) ths->position.x += 200. * (double)dt;
	}
};