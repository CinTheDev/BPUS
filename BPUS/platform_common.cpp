#pragma once
struct ButtonState {
	bool isdown;
	bool changed;
};

#pragma once
enum {
	BUTTON_UP,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_A,
	BUTTON_S,
	BUTTON_D,

	BUTTON_COUNT, // last item
};

#pragma once
struct Input {
	ButtonState buttons[BUTTON_COUNT];
};