struct ButtonState {
	bool isdown;
	bool changed;
};

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

struct Input {
	ButtonState buttons[BUTTON_COUNT];
};