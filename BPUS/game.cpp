class BPUS_Game{
private:
#pragma region Images
	Texture popCat = Texture("Assets/Images/popcat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture box = Texture("Assets/Images/test3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
#pragma endregion
#pragma region Objects
	Object* cat;
	Obj::Baseobject* bso2;
#pragma endregion

	void loadTextures(Shader shader) {
		popCat.texUnit(shader, "tex0", 0);
		box.texUnit(shader, "tex0", 0);
	}

public:
	BPUS_Game(GLFWwindow* window, Shader shader) {
		loadTextures(shader);

		cat = new Object(Vector2(-1.5, -0.5), &popCat);
		bso2 = new Obj::Baseobject(Vector2(0.5, -0.5), &box);

		// Camera
		camera = new Obj::Camera(Vector2(0, 0), NULL);
		camera->setWindow(window);
		Obj_M::create(camera);

		Obj_M::create(cat);
		Obj_M::create(bso2);
	}

	void update(UpdateArguments args) {
		Obj_M::objects_update(args);
	}
};
