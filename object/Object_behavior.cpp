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
			if (glfwGetKey(args.window, GLFW_KEY_I) == GLFW_PRESS) position.y += 2. * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_K) == GLFW_PRESS) position.y -= 2. * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_J) == GLFW_PRESS) position.x -= 2. * args.deltatime;
			if (glfwGetKey(args.window, GLFW_KEY_L) == GLFW_PRESS) position.x += 2. * args.deltatime;

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

	class Player_1 : public object {
		using object::object;

		comp::dynamics* rb;
		const double jumpforce = 10;

		void init() {
			rb = getComponent<comp::dynamics>();
		}

		void update(updateArguments args) {
			if (glfwGetKey(args.window, GLFW_KEY_W) == GLFW_PRESS) rb->addForce(Vector2(0, 1) * jumpforce * args.deltatime);
			if (glfwGetKey(args.window, GLFW_KEY_A) == GLFW_PRESS) rb->addForce(Vector2(-0.5, 0) * jumpforce * args.deltatime);
			if (glfwGetKey(args.window, GLFW_KEY_S) == GLFW_PRESS) rb->addForce(Vector2(0, -0.5) * jumpforce * args.deltatime);
			if (glfwGetKey(args.window, GLFW_KEY_D) == GLFW_PRESS) rb->addForce(Vector2(0.5, 0) * jumpforce * args.deltatime);
		}
	};
	class Player_2 : public object {
		using object::object;

		comp::dynamics* rb;
		const double jumpforce = 10;

		void init() {
			rb = getComponent<comp::dynamics>();
		}

		void update(updateArguments args) {
			if (glfwGetKey(args.window, GLFW_KEY_UP) == GLFW_PRESS) rb->addForce(Vector2(0, 1) * jumpforce * args.deltatime);
			if (glfwGetKey(args.window, GLFW_KEY_LEFT) == GLFW_PRESS) rb->addForce(Vector2(-0.5, 0) * jumpforce * args.deltatime);
			if (glfwGetKey(args.window, GLFW_KEY_DOWN) == GLFW_PRESS) rb->addForce(Vector2(0, -0.5) * jumpforce * args.deltatime);
			if (glfwGetKey(args.window, GLFW_KEY_RIGHT) == GLFW_PRESS) rb->addForce(Vector2(0.5, 0) * jumpforce * args.deltatime);
		}
	};

	class Basketball : public object {
		using object::object;

		void init() {
			// Some cool stuff
		}

		void update(updateArguments args) {
			// More cool stuff
		}
	};

#pragma endregion
#pragma region Functions

#pragma endregion
}
