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

        cat = new object(Vector2(0, 0), &popCat);
        cat->z = 1;
        comp::dynamics* kin = new comp::dynamics();
        kin->speed = Vector2(0, 0);
        kin->acceleration = Vector2(0, 9.81);
        kin->mass = 1;
        kin->alias = "Rigidbody for cat";
        cat->addComponent(kin);

        comp::collider_rect* test1 = new comp::collider_rect(Vector2(1, 1), Vector2(0, 0));
        test1->alias = "Rect collider for cat";
        test1->bounciness = 0.8;
        cat->addComponent(test1);

        bso2 = new object(Vector2(3, -0.75), &box);

        comp::dynamics* kin2 = new comp::dynamics();
        kin2->speed = Vector2(0, 0);
        kin2->acceleration = Vector2(0, 9.81);
        kin2->mass = 1;
        kin2->alias = "Rigidbody for bso2";
        bso2->addComponent(kin2);

        comp::collider_rect* test2 = new comp::collider_rect(Vector2(1, 1), Vector2(0, 0));
        //comp::collider_circle* test2 = new comp::collider_circle(0.5, Vector2(0.5, 0.5));
        test2->alias = "Rect collider for bso2";
        test2->bounciness = 0.8;
        bso2->addComponent(test2);
        bso2->z = 0;

        object* floor = new object();
        comp::collider_line* floorCollider = new comp::collider_line(Vector2(-5, -1.5), Vector2(5, -1.5));
        floor->addComponent(floorCollider);
        obj_m::create(floor);

        for (int i = 0; i <= 10; i++) {
            object* h = new object(Vector2(-10 + i * 2, 2));

            comp::collider_rect* hc = new comp::collider_rect(Vector2(1, 1), Vector2(0, 0));

            h->addComponent(hc);
            obj_m::create(h);
        }

        // Camera
        camera = new obj::Camera(Vector2(0, 0), NULL);
        camera->setZoom(3);
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
        comp::dynamics* kin = cat->getComponent<comp::dynamics>();
        if (glfwGetKey(args.window, GLFW_KEY_W)) {
            kin->addForce(Vector2(0, 0.5) * args.deltatime);
        }

        if (glfwGetKey(args.window, GLFW_KEY_A)) {
            kin->addForce(Vector2(-0.5, 0) * args.deltatime);
        }

        if (glfwGetKey(args.window, GLFW_KEY_D)) {
            kin->addForce(Vector2(0.5, 0) * args.deltatime);
        }

        if (glfwGetKey(args.window, GLFW_KEY_S)) {
            kin->addForce(Vector2(0, -0.5) * args.deltatime);
        }

        if (glfwGetKey(args.window, GLFW_KEY_Q)) {
            cat->rotation += 0.5 * args.deltatime;
        }

        if (glfwGetKey(args.window, GLFW_KEY_E)) {
            cat->rotation -= 0.5 * args.deltatime;
        }
    }
};
