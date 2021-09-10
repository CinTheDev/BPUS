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

		void update(Input* input, float dt) override;
	};

	{...}

	inline void [Name]::init() {}
	inline void [Name]::update(Input* input, float dt) {}

	*/

	class Baseobject : public Object {
		using Object::Object;

		void init() override;

		void update(Input* input, float dt) override;
	};

	class Bullet : public Object {
		using Object::Object;

		int i = 0;

		void init() override;

		void update(Input* input, float dt) override;
	};

#pragma endregion
#pragma region Functions

	inline void Baseobject::init() {}
	inline void Baseobject::update(Input* input, float dt) {
		if (isdown(BUTTON_UP)) size *= 1.01f;
		if (isdown(BUTTON_DOWN)) size *= 0.99f;

		if (isdown(BUTTON_W)) position.y += 200. * (double)dt;
		if (isdown(BUTTON_A)) position.x -= 200. * (double)dt;
		if (isdown(BUTTON_S)) position.y -= 200. * (double)dt;
		if (isdown(BUTTON_D)) position.x += 200. * (double)dt;

		if (pressed(BUTTON_1)) {
			Object* obj = new Bullet(position, image, 0.3f);
			Obj_M::create(obj);
		}
	}

	inline void Bullet::init() {}
	inline void Bullet::update(Input* input, float dt) {
		if (i > 100) {
			Obj_M::remove(this);
			return;
		}
		position.y += 20.f * dt;
		i++;
	}

#pragma endregion
}