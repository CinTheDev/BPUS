class BPUS_Game{
private:
#pragma region Images
	Texture popCat = Texture("Assets/Images/popcat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture box = Texture("Assets/Images/test3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
#pragma endregion
#pragma region Objects
	Obj::Empty* empty;
	Obj::Baseobject* bso2;
#pragma endregion

	void loadTextures(Shader shader) {
		popCat.texUnit(shader, "tex0", 0);
		box.texUnit(shader, "tex0", 0);
	}

public:
	BPUS_Game(Shader shader) {
		loadTextures(shader);

		empty = new Obj::Empty(Vector2(-0.65, -0.5), &popCat, Vector2(0.6, 1));
		bso2 = new Obj::Baseobject(Vector2(0, -0.5), &box, Vector2(0.6, 1));

		// Camera
		camera = new Obj::Camera(Vector2(0, 0), NULL);
		Obj_M::create(camera);

		Obj_M::create(empty);
		Obj_M::create(bso2);
	}

	void update(UpdateArguments args) {
		Obj_M::objects_update(args);

	}
};
