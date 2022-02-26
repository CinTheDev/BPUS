#pragma once

namespace comp {
    class collider : public component {
        // General class used for colliders
    public:
        void init() override {
            obj_m::registerCollider(this);
        }

        ~collider() {
            obj_m::removeCollider(this);
        }
    };

    class collider_line : public collider {
    private:
        Vector2 normal;
    public:

        Vector2 point1;
        Vector2 point2;

        void calcNormal() {
            normal = (point2 - point1).rotate(0.5 * PI).normalized();
        }

        void init() override {
            calcNormal();
            obj_m::registerCollider(this);
        }

        void update(updateArguments args) override {

        }
    };

    class collider_rect : public collider {
        // Rectangle shaped collider, mostly used by other components
    public:
        void init() override {
            obj_m::registerCollider(this);
        }
    };

    class collider_circle : public collider {
        // Circle shaped collider, mostly used by other components
    public:
        void init() override {
            obj_m::registerCollider(this);
        }
    };

    class dynamics : public component {
        // Component for everything related to dynamics (movement and acceleration)
        // Can also take advantage of colliders to simulate collisions
    private:
    public:
        Vector2 speed = Vector2(); // Speed in m/s (actually units/second but for the sake of physics let's use real units)
        Vector2 acceleration = Vector2(); // in m/s²
        double mass = 1; // Mass in kg

        double gravity = -9.81;

        // Adds a certain amount of energy to the object (with direction)
        void addForce(Vector2 joules) {
            // using v = sqrt(2W / m)
            speed.x += sqrt(2 * abs(joules.x) / mass) * sign(joules.x);
            speed.y += sqrt(2 * abs(joules.y) / mass) * sign(joules.y);
        }

        // Sets the acceleration constant, so the force is applied continuously
        void setForce(Vector2 newtons) {
            acceleration = newtons / mass;
        }

        void init() override {
            // Initialize dynamics
        }

        void update(updateArguments args) override {
            // using v = a * t
            speed.y += gravity * args.deltatime;
            speed += acceleration * args.deltatime;

            // I know Euler's method isn't great, but I'm not smart enough to use better stuff
            parent->position += speed * args.deltatime;
        }
    };
}
