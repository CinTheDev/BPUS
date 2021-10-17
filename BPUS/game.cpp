class BPUS_Game{
	// Networking has been removed for now
	// https://www.youtube.com/watch?v=f_1lt9pfaEo has everything important
private:
#pragma region Images
	Image empty = Image(0, 0, 0);
	Image test = Image("Assets\\Images\\test2.jpg");
	Image alphatest = Image("Assets\\Images\\test3.png");
#pragma endregion
#pragma region Objects
	Obj::Baseobject* baseObject;
#pragma endregion

public:
	BPUS_Game() {
		// Camera
		camera = new Obj::Camera(Vector2(5, 2.5), &empty, 1.f);
		Obj_M::create(camera);

		baseObject = new Obj::Baseobject(Vector2(7, 2.5), &alphatest, 0.5f);
		baseObject->z = 0;
		Obj_M::create(baseObject);
	}

	void update(Input* input, float dt) {
		Obj_M::objects_update(input, dt);

		// Render
		render();

		// Debug
		std::string debug = baseObject->position.str() + std::to_string(baseObject->size);
		draw_text(Vector2(1, 0), debug.c_str(), .05f);
	}
};
