#define isdown(b) input -> buttons[b].isdown
#define pressed(b) (input -> buttons[b].isdown && input -> buttons[b].changed)
#define released(b) (!input -> buttons[b].isdown && input -> buttons[b].changed)

#pragma once
namespace Obj{
#pragma region Declarations

	/*
	Class syntax for copy paste:

	class [Name] : public Object {
		using Object::Object;
		// Variables here

		void init() override;

		void update(Input* input, double dt) override;
	};

	{...}

	inline void [Name]::init() {}
	inline void [Name]::update(Input* input, double dt) {}

	*/

	class Empty : public Object {
		using Object::Object;

		void init() override;

		void update(Input* input, double dt) override;
	};

	class Baseobject : public Object {
		using Object::Object;

		void init() override;

		void update(Input* input, double dt) override;
	};

	class Bullet : public Object {
		using Object::Object;

		int i = 0;

		void init() override;

		void update(Input* input, double dt) override;
	};

#pragma endregion
#pragma region Functions

	inline void Empty::init() {}
	inline void Empty::update(Input* input, double dt) {}

	inline void Baseobject::init() {}
	inline void Baseobject::update(Input* input, double dt) {
		if (isdown(BUTTON_UP)) campos.y += 50. * dt;
		if (isdown(BUTTON_DOWN)) campos.y -= 50. * dt;
		if (isdown(BUTTON_LEFT)) campos.x -= 50. * dt;
		if (isdown(BUTTON_RIGHT)) campos.x += 50. * dt;

		if (isdown(BUTTON_W)) position.y += 200. * dt;
		if (isdown(BUTTON_A)) position.x -= 200. * dt;
		if (isdown(BUTTON_S)) position.y -= 200. * dt;
		if (isdown(BUTTON_D)) position.x += 200. * dt;

		if (pressed(BUTTON_F)) {
			Object* obj = new Bullet(position, image, 0.3f);
			Obj_M::create(obj);
		}
	}

	inline void Bullet::init() {}
	inline void Bullet::update(Input* input, double dt) {
		if (i > 100) {
			Obj_M::remove(this);
			return;
		}
		position.y += 20. * dt;
		i++;
	}

#pragma endregion
}
