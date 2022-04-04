#pragma once

namespace comp {
    class dynamics : public component {
        // Component for everything related to dynamics (movement and acceleration)
    private:
    public:
        Vector2 speed = Vector2(); // Speed in m/s (actually units/second but lets use real units for the sake of physics)
        Vector2 acceleration = Vector2(); // in m/s²
        double mass = 1; // Mass in kg

        const double gravity = -9.81;

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


    class collider : public component {
        // General class used for colliders
    protected:
        dynamics* rigidbody;
    public:
        double bounciness = 1;

        virtual bool check_collision() { return false; }
        virtual void resolve_collision(Vector2 collision) {}

        void init() override {}

        ~collider() {}
    };

    class collider_line : public collider {
    private:
    public:
        void init() override {
            obj_m::registerLine(this);
        }
    };

    class collider_rect : public collider {
        // Rectangle shaped collider, mostly used by other components
    public:
        void init() override {
            obj_m::registerRect(this);
        }
    };

    class collider_circle : public collider {
        // Circle shaped collider, mostly used by other components
    public:
        float radius;
        Vector2 offset;

        collider_circle() {
            radius = 1;
            offset = Vector2();
        }
        collider_circle(float r, Vector2 off) {
            radius = r;
            offset = off;
        }

        void init() override {
            obj_m::registerCircle(this);
            rigidbody = parent->getComponent<dynamics>();
        }

        void update(updateArguments args) override {
            // Debug
            Vector3 color = Vector3(0, 0, 1);
            if (check_collision()) color = Vector3(1, 0, 0);
            debug::draw_ellipse(parent->position + offset, Vector2(radius, radius), parent->rotation, color);
        }

        bool check_collision() override {
            if (rigidbody == nullptr) return false;

            bool collision = false;

            for (auto& c : obj_m::circle_colliders) {
                if (c == this) continue;

                double sqrlen = (c->parent->position - parent->position).sqrlen();
                if (sqrlen <= (radius + c->radius) * (radius + c->radius)) {
                    resolve_collision(c);
                    collision = true;
                }
            }
            return collision;
        }

        void resolve_collision(comp::collider_circle* target) {
            Vector2 dist = target->parent->position - parent->position;
            double d = dist.len();
            dist.normalize();
            Vector2 point = dist * (radius + 0.5*(d - radius - target->radius));

            // Debug
            debug::draw_ray(parent->position + parent->size * 0.5, point * 0.95, Vector3(0, 1, 0));

            // Special case: Other object is static
            if (target->rigidbody == nullptr) {
                // Static resolution on own object
                parent->position += dist * (d - radius - target->radius);

                // Dynamic resolution is just flipping the own speed.
                dist *= bounciness;
                rigidbody->speed -= dist * dist.dot(rigidbody->speed) * (2 / bounciness);
                
                return;
            }

            // Static resolution
            parent->position += dist * (d - radius - target->radius) * 0.5;
            target->parent->position -= dist * (d - radius - target->radius) * 0.5;

            // Dynamic resolution
            Vector2 k = rigidbody->speed - target->rigidbody->speed;
            Vector2 n = dist;
            double p = 2.0 * n.dot(k) / (rigidbody->mass + target->rigidbody->mass);

            rigidbody->speed -= n * p * target->rigidbody->mass;
            target->rigidbody->speed += n * p * rigidbody->mass;
        }
    };   
}
