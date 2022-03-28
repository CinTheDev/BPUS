class BPUS_game {
private:
#pragma region Images
    texture popCat = texture("assets/images/popcat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture box = texture("assets/images/Box.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
#pragma endregion
#pragma region Objects
    object* cat;
    object* bso2;
#pragma endregion

    void loadTextures(shader shdr) {
        popCat.texUnit(shdr, "tex0", 0);
        box.texUnit(shdr, "tex0", 0);
    }

public:
    BPUS_game(GLFWwindow* window, shader shdr) {
        loadTextures(shdr);

        cat = new object(Vector2(0, -0.5), &popCat);
        cat->z = 1;
        comp::dynamics* kin = new comp::dynamics();
        kin->speed = Vector2(0, 4);
        kin->acceleration = Vector2(0, 5);
        cat->addComponent(kin);

        //comp::collider_line* test = new comp::collider_line(Vector2(1, 0), Vector2(0, 0));
        //test->alias = "Collider for cat";
        //cat->addComponent(test);

        bso2 = new object(Vector2(0, -0.5), &box);
        //test = new comp::collider_line(Vector2(0.5, 1), Vector2(0.5, 0));
        //test->alias = "Collider for bso2";
        //bso2->addComponent(test);
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
        //comp::dynamics* kin = (comp::dynamics*)cat->getComponent(&typeid(comp::dynamics));
        comp::dynamics* kin = cat->getComponent<comp::dynamics>();
        if (glfwGetKey(args.window, GLFW_KEY_W)) {
            kin->addForce(Vector2(0, 2) * args.deltatime);
        }

        if (glfwGetKey(args.window, GLFW_KEY_A)) {
            kin->addForce(Vector2(-1, 0) * args.deltatime);
        }

        if (glfwGetKey(args.window, GLFW_KEY_D)) {
            kin->addForce(Vector2(1, 0) * args.deltatime);
        }

        debug::draw_line(Vector2(0, 0), cat->position);
    }
};