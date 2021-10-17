Image empty(0, 0, 0);
Image test("Assets\\Images\\test2.jpg");
Image alphatest("Assets\\Images\\test3.png");

Obj::Empty* obj;
Obj::Baseobject* obj2;

class BPUS_Game{
	// Networking has been removed for now
	// https://www.youtube.com/watch?v=f_1lt9pfaEo has everything important
private:
	std::vector<Image*> images;

public:
	BPUS_Game() {
		// Camera
		camera = new Obj::Camera(Vector2(5, 2.5), &empty, 1.f);
		Obj_M::create(camera);

		//obj = new Obj::Empty(Vector2(0, 0), &test, 200.f);
		//obj->z = 1;
		obj2 = new Obj::Baseobject(Vector2(7, 2.5), &alphatest, 0.5f);
		obj2->z = 0;
		//Obj_M::create(obj);
		Obj_M::create(obj2);
	}

	void update(Input* input, float dt) {
		Obj_M::objects_update(input, dt);

		// Render
		render();

		// Debug
		std::string debug = obj2->position.str() + std::to_string(obj2->size);
		draw_text(Vector2(1, 0), debug.c_str(), .05f);
	}
};
