#pragma once

namespace Obj{
#pragma region Declarations

	/*
	* Define functions inside class, but when needed outside.
	Class syntax for copy paste:

	class Name : public Object {
		using Object::Object;
		// Variables here

		void init() override;

		void update(UpdateArguments args) override;
	};

	{...}

	void Name::init() {}
	void Name::update(UpdateArguments args) {}

	*/

	class Empty : public Object {
		using Object::Object;

		void init() override {}

		void update(UpdateArguments args) override {}
	};

	class Camera : public Object {
		using Object::Object;

	private:
		float zoom = 8;

	public:
		void update(UpdateArguments args) override {
			if (glfwGetKey(args.window, GLFW_KEY_UP) == GLFW_PRESS) position.y += 2. * args.deltaTime;
			if (glfwGetKey(args.window, GLFW_KEY_DOWN) == GLFW_PRESS) position.y -= 2. * args.deltaTime;
			if (glfwGetKey(args.window, GLFW_KEY_LEFT) == GLFW_PRESS) position.x -= 2. * args.deltaTime;
			if (glfwGetKey(args.window, GLFW_KEY_RIGHT) == GLFW_PRESS) position.x += 2. * args.deltaTime;

			if (glfwGetKey(args.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) setZoom(zoom - 5 * args.deltaTime);
			if (glfwGetKey(args.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) setZoom(zoom + 5 * args.deltaTime);
		}

		// Setter
		void setZoom(float value) {
			zoom = clamp(0.f, value, INFINITY);
		}
		// Getter
		float getZoom() {
			return zoom;
		}

		//Vector2Int middleOfScreen() {
			// Both values are bitshifted left, this halfs the values.
		//	return Vector2Int(renderState.width >> 1, renderState.height >> 1);
		//}

		//double unitInPixel() {
			//return (int)floor(units * zoom);
		//	return renderState.height / zoom;
		//}
	};

	class Baseobject : public Object {
		using Object::Object;

		void update(UpdateArguments args) override {
			if (glfwGetKey(args.window, GLFW_KEY_W) == GLFW_PRESS) position.y += 1. * args.deltaTime;
			if (glfwGetKey(args.window, GLFW_KEY_A) == GLFW_PRESS) position.x -= 1. * args.deltaTime;
			if (glfwGetKey(args.window, GLFW_KEY_S) == GLFW_PRESS) position.y -= 1. * args.deltaTime;
			if (glfwGetKey(args.window, GLFW_KEY_D) == GLFW_PRESS) position.x += 1. * args.deltaTime;

			if (glfwGetKey(args.window, GLFW_KEY_Q) == GLFW_PRESS) rotation += 1 * args.deltaTime;
			if (glfwGetKey(args.window, GLFW_KEY_E) == GLFW_PRESS) rotation -= 1 * args.deltaTime;
			
			//if (isdown(BUTTON_NUMPAD_8)) size += 1 * dt;
			//if (isdown(BUTTON_NUMPAD_2)) size -= 1 * dt;
		}
	};

#pragma endregion
#pragma region Functions

#pragma endregion
}
