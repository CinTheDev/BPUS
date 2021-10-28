class BPUS_Game{
private:
#pragma region Images
	Texture popCat = Texture("Assets/Images/popcat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	
#pragma endregion
#pragma region Objects
	Obj::Baseobject* bso = new Obj::Baseobject(Vector2(-0.65, -0.5), &popCat, Vector2(0.6, 1));
	Obj::Baseobject* bso2 = new Obj::Baseobject(Vector2(0, -0.5), &popCat, Vector2(0.6, 1));
#pragma endregion

public:
	BPUS_Game(Shader shader) {
		popCat.texUnit(shader, "tex0", 0);

		// Camera
		//camera = new Obj::Camera(Vector2(5, 2.5), &empty, 1.f);
		//Obj_M::create(camera);

		Obj_M::create(bso);
		Obj_M::create(bso2);
	}

	void update(Input* input, float dt) {
		Obj_M::objects_update(input, dt);

	}
};
