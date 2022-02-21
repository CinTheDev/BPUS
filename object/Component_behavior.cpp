#pragma once

namespace comp {
    class collider : public component {
        // General class used for colliders
    };

    class collider_line : public collider {
        // Line shaped collider, mostly used by other components
    };

    class collider_rect : public collider {
        // Rectangle shaped collider, mostly used by other components
    };

    class collider_circle : public collider {
        // Circle shaped collider, mostly used by other components
    };

    class kinematics : public component {
        // Component for everything related to kinematics (movement and acceleration)
        // Can also take advantage of colliders to simulate collisions
    private:
    public:
        Vector2 speed = Vector2(); // Speed in m/s (actually units/second but for the sake of physics let's use real units)
        Vector2 acceleration = Vector2(); // in m/s²
        double mass = 0; // Mass in kg

        double gravity = -9.81;

        // Adds a certain amount of energy to the object (with direction)
        void addForce(Vector2 joules) {
            // using v = sqrt(2W / m)
            speed.x += sqrt(2 * joules.x / mass);
            speed.y += sqrt(2 * joules.y / mass);
        }

        // Sets the acceleration constant, so the force is applied continuously
        void setForce(Vector2 newtons) {
            acceleration = newtons / mass;
        }

        void init() override {
            // Initialize kinematics
        }

        void update(updateArguments args) override {
            speed.y += gravity * args.deltatime;
            speed += acceleration * args.deltatime;

            // I know Euler's method isn't great, but I'm not smart enough to use better stuff
            //parent->position += speed * args.deltatime;
            parent->position += (speed + acceleration * 0.5 * args.deltatime) * args.deltatime;
        }
    };
}
