#pragma once

#define isdown(b) input -> buttons[b].isdown
#define pressed(b) (input -> buttons[b].isdown && input -> buttons[b].changed)
#define released(b) (!input -> buttons[b].isdown && input -> buttons[b].changed)

namespace Obj{
#pragma region Declarations

	/*
	* Define functions inside class, but when needed outside.
	Class syntax for copy paste:

	class Name : public Object {
		using Object::Object;
		// Variables here

		void init() override;

		void update(Input* input, double dt) override;
	};

	{...}

	void Name::init() {}
	void Name::update(Input* input, double dt) {}

	*/

	class Empty : public Object {
		using Object::Object;

		void init() override {}

		void update(Input* input, double dt) override {}
	};

	class Camera : public Object {
		using Object::Object;

	private:
		float zoom = 1;

	public:
		void update(Input* input, double dt) override {
			if (isdown(BUTTON_UP)) position.y += 50. * dt;
			if (isdown(BUTTON_DOWN)) position.y -= 50. * dt;
			if (isdown(BUTTON_LEFT)) position.x -= 50. * dt;
			if (isdown(BUTTON_RIGHT)) position.x += 50. * dt;

			if (isdown(BUTTON_SHIFT)) setZoom(zoom + 1 * dt);
			if (isdown(BUTTON_CTRL)) setZoom(zoom - 1 * dt);
		}

		// Setter
		void setZoom(float value) {
			zoom = clamp(0.f, value, INFINITY);
		}
		// Getter
		float getZoom() {
			return zoom;
		}

		Vector2Int middleOfScreen() {
			return Vector2Int(renderState.width >> 1, renderState.height >> 1);
		}
	};

	class Baseobject : public Object {
		using Object::Object;

		void update(Input* input, double dt) override {
			if (isdown(BUTTON_W)) position.y += 200. * dt;
			if (isdown(BUTTON_A)) position.x -= 200. * dt;
			if (isdown(BUTTON_S)) position.y -= 200. * dt;
			if (isdown(BUTTON_D)) position.x += 200. * dt;

			if (isdown(BUTTON_Q)) rotation += 1 * dt;
			if (isdown(BUTTON_E)) rotation -= 1 * dt;

			if (isdown(BUTTON_NUMPAD_8)) size += 200 * dt;
			if (isdown(BUTTON_NUMPAD_2)) size -= 200 * dt;
		}
	};

#pragma endregion
#pragma region Functions

#pragma endregion
}
