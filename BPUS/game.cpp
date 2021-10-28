class BPUS_Game{
private:
#pragma region Images
	/*Image empty = Image(0, 0, 0);
	Image test = Image("Assets\\Images\\test2.jpg");
	Image alphatest = Image("Assets\\Images\\test3.png");
	Image box = Image("Assets\\Images\\box.png");*/
	Texture popCat = Texture("Assets/Images/popcat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	
#pragma endregion
#pragma region Objects
	Obj::Baseobject* bso;
	Obj::Baseobject* bso2;
#pragma endregion

public:
	BPUS_Game(Shader shader) {
		popCat.texUnit(shader, "tex0", 0);

		// Camera
		//camera = new Obj::Camera(Vector2(5, 2.5), &empty, 1.f);
		//Obj_M::create(camera);

		bso = new Obj::Baseobject(Vector2(-0.65, -0.5), &popCat, Vector2(0.6, 1));
		bso2 = new Obj::Baseobject(Vector2(0, -0.5), &popCat, Vector2(0.6, 1));
		Obj_M::create(bso);
		Obj_M::create(bso2);
	}

	void update(Input* input, float dt) {
		Obj_M::objects_update(input, dt);

		// Debug
		//std::string debug = std::to_string(baseObject->image->w);
		//draw_text(Vector2(1, 0), debug.c_str(), .05f);
	}
};
