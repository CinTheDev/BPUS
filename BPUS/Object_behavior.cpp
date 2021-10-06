#pragma once

#define isdown(b) input -> buttons[b].isdown
#define pressed(b) (input -> buttons[b].isdown && input -> buttons[b].changed)
#define released(b) (!input -> buttons[b].isdown && input -> buttons[b].changed)

namespace Obj{
#pragma region Declarations

	/*
	Class syntax for copy paste:

	class Name : public Object {
		using Object::Object;
		// Variables here

		void init() override;

		void update(Input* input, double dt) override;
	};

	{...}

	inline void Name::init() {}
	inline void Name::update(Input* input, double dt) {}

	*/

	class Empty : public Object {
		using Object::Object;

		void init() override;

		void update(Input* input, double dt) override;
	};

	class Camera : public Object {
		using Object::Object;
		


		void init() override;

		void update(Input * input, double dt) override;
	};

	class Baseobject : public Object {
		using Object::Object;

		void init() override;

		void update(Input* input, double dt) override;
	};

#pragma endregion
#pragma region Functions

	inline void Empty::init() {}
	inline void Empty::update(Input* input, double dt) {}

	inline void Camera::init() {
	}
	inline void Camera::update(Input* input, double dt) {
		if (isdown(BUTTON_UP)) position.y += 50. * dt;
		if (isdown(BUTTON_DOWN)) position.y -= 50. * dt;
		if (isdown(BUTTON_LEFT)) position.x -= 50. * dt;
		if (isdown(BUTTON_RIGHT)) position.x += 50. * dt;
	}

	inline void Baseobject::init() {}
	inline void Baseobject::update(Input* input, double dt) {
		if (isdown(BUTTON_W)) position.y += 200. * dt;
		if (isdown(BUTTON_A)) position.x -= 200. * dt;
		if (isdown(BUTTON_S)) position.y -= 200. * dt;
		if (isdown(BUTTON_D)) position.x += 200. * dt;

		if (isdown(BUTTON_Q)) rotation += 1 * dt;
		if (isdown(BUTTON_E)) rotation -= 1 * dt;

		if (isdown(BUTTON_NUMPAD_8)) size += 2 * dt;
		if (isdown(BUTTON_NUMPAD_2)) size -= 2 * dt;
	}

#pragma endregion
}
