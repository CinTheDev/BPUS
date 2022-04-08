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
        // Note: this collider can only be used as a static collider, no dynamics.
    private:
    public:
        Vector2 p1;
        Vector2 p2;

        collider_line() {
            p1 = Vector2();
            p2 = Vector2();
        }
        collider_line(Vector2 p1, Vector2 p2) : p1(p1), p2(p2) {}

        void init() override {
            obj_m::registerLine(this);
        }

        void update(updateArguments args) override {
            // Debug
            debug::draw_line(p1, p2, Vector3(1, 0.8, 0));
        }
    };

    class collider_rect : public collider {
        // Rectangle shaped collider, mostly used by other components
    public:
        Vector2 size;
        Vector2 offset;

        collider_rect() {
            size = Vector2(1, 1);
            offset = Vector2();
        }
        collider_rect(Vector2 s, Vector2 o) {
            size = s;
            offset = o;
        }

        void init() override {
            obj_m::registerRect(this);
            rigidbody = parent->getComponent<dynamics>();
        }

        void update(updateArguments args) {
            Vector3 color = Vector3(0, 1, 0);
            if (check_collision()) color = Vector3(1, 0, 0);
            debug::draw_rect(parent->position + offset, size, parent->rotation, color);
        }

        bool check_collision() override {
            if (rigidbody == nullptr) return false;

            // TODO: detect collision

            return false;
        }

        // Rect vs. rect
        void resolve_collision(collider_rect target) {
            // TODO: Static resolution

            // TODO: Dynamic resolution
        }

        // Rect vs. circle
        void resolve_collision(collider_circle target) {
            // TODO: Static resolution

            // TODO: Dynamic resolution
        }

        // Rect vs. line
        void resolve_collision(Vector2 close_dist) {
            // TODO: Static resolution

            // TODO: Dynamic resolution
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
        collider_circle(float r, Vector2 off) : radius(r), offset(off) {}

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

            // Circle vs. circle
            for (auto& c : obj_m::circle_colliders) {
                if (c == this) continue;

                double sqrlen = (c->parent->position - parent->position).sqrlen();
                if (sqrlen <= (radius + c->radius) * (radius + c->radius)) {
                    resolve_collision(c);
                    collision = true;
                }
            }
            // Circle vs. line
            for (auto& c : obj_m::line_colliders) {
                Vector2 segment = c->p2 - c->p1;
                Vector2 d1 = parent->position + offset - c->p1;

                double s_len = segment.sqrlen();

                double t = clamp(0.0, segment.dot(d1), s_len) / s_len;

                Vector2 closest_point = c->p1 + segment * t;

                double distance = (parent->position + offset - closest_point).len();

                if (distance <= radius) {
                    resolve_collision(closest_point);
                    collision = true;
                }
            }
            return collision;
        }

        // Circle vs. circle
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

        // Circle vs. line
        void resolve_collision(Vector2 close_dist) {
            Vector2 dist = close_dist - parent->position - offset;
            // Static resolution
            parent->position += dist.normalized() * (dist.len() - radius);

            // Dynamic resolution is just flipping the own speed.
            dist = dist.normalized() * bounciness;
            rigidbody->speed -= dist * dist.dot(rigidbody->speed) * (2 / bounciness);
        }
    };   
}
