#pragma once

namespace obj {
#pragma region Declarations

	/*
	* Define functions inside class, but when needed outside.
	Class syntax for copy paste:

	class Name : public Object {
		using Object::Object;
		// Variables here

		void init() override {}

		void update(UpdateArguments args) override {}
	};

	{...}

	void Name::init() {}
	void Name::update(UpdateArguments args) {}

	*/

	class Camera : public object {
		using object::object;

	private:
		GLFWwindow* window;
		float zoom = 1;

	public:
		void update(updateArguments args) override {
			if (glfwGetKey(args.window, GLFW_KEY_UP) == GLFW_PRESS) position.y += 2. * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_DOWN) == GLFW_PRESS) position.y -= 2. * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_LEFT) == GLFW_PRESS) position.x -= 2. * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_RIGHT) == GLFW_PRESS) position.x += 2. * args.deltatime;

			if (glfwGetKey(args.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) setZoom(zoom - 1 * args.deltatime);
			if (glfwGetKey(args.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) setZoom(zoom + 1 * args.deltatime);
		}

		void setWindow(GLFWwindow* window) {
			this->window = window;
		}
		GLFWwindow* getWindow() {
			return window;
		}

		// Setter
		void setZoom(float value) {
			zoom = clamp(0.1f, value, INFINITY);
		}
		// Getter
		float getZoom() {
			return zoom;
		}

		Vector2Int getWindowDimensions() {
			Vector2Int dim;
			glfwGetWindowSize(window, &dim.x, &dim.y);
			return dim;
		}
	};

	class Baseobject : public object {
		using object::object;

		void update(updateArguments args) override {
			if (glfwGetKey(args.window, GLFW_KEY_W) == GLFW_PRESS) position.y += 1. * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_A) == GLFW_PRESS) position.x -= 1. * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_S) == GLFW_PRESS) position.y -= 1. * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_D) == GLFW_PRESS) position.x += 1. * args.deltatime;

			if (glfwGetKey(args.window, GLFW_KEY_Q) == GLFW_PRESS) rotation += 1 * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_E) == GLFW_PRESS) rotation -= 1 * args.deltatime;
		}
	};

#pragma endregion
#pragma region Functions

#pragma endregion
}
