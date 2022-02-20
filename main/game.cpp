class BPUS_game {
private:
#pragma region Images
    texture popCat = texture("assets/images/popcat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture box = texture("assets/images/Box.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
#pragma endregion
#pragma region Objects
    object* cat;
    obj::Baseobject* bso2;
#pragma endregion

    void loadTextures(shader shdr) {
        popCat.texUnit(shdr, "tex0", 0);
        box.texUnit(shdr, "tex0", 0);
    }

public:
    BPUS_game(GLFWwindow* window, shader shdr) {
        loadTextures(shdr);

        cat = new object(Vector2(-1.5, -0.5), &popCat);
        cat->z = 1;
        cat->addComponent(new comp::moveRight);
        bso2 = new obj::Baseobject(Vector2(0.5, -0.5), &box);
        bso2->z = 0;

        // Camera
        camera = new obj::Camera(Vector2(0, 0), NULL);
        camera->setWindow(window);
        obj_m::create(camera);

        obj_m::create(cat);
        obj_m::create(bso2);
    }

    ~BPUS_game() {
        obj_m::stop();
        popCat.Delete();
        box.Delete();
        delete cat;
        delete bso2;
    }

    void update(updateArguments args) {
        // h
    }
};