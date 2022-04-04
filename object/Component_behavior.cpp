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

    /*struct owned_vector2 {
    public:
        component* c;
        Vector2* v;

        bool operator<(const owned_vector2& vec) { return *v < *vec.v; }
    };*/

    class collider_line : public collider {
    /*private:
        Vector2 normal;

        // First is key, second is last position
        std::map<owned_vector2, Vector2> observed_points;

        void getPointsInRadius(Vector2 pos, double radius) {
            std::string debug = alias;
            std::vector<collider*> debug2(obj_m::colliders);
            for (auto& c : obj_m::colliders) {
                // Convert to collider_line if possible
                if (typeid(*c) != typeid(collider_line)) continue;
                if (c->parent == parent) continue;
                collider_line* co = (collider_line*)c;
                //Vector2* p1 = &co->point1;
                owned_vector2 v;
                v.c = c;
                v.v = &co->point1;
                Vector2* p2 = &co->point2;
                Vector2 ppos = co->parent->position;
                
                // If the first point is in range
                if (((*v.v + ppos) - pos).sqrlen() < radius * radius) {
                    if (map_contains(observed_points, v)) continue;
                    // Add it to the list if it isn't already
                    observed_points.insert({ v, *v.v + ppos });
                    //std::cout << alias << ": Add point 1 from component " << co->alias << std::endl;
                }
                // If the first point is not in range but in the list
                else if (map_contains(observed_points, v)) {
                    // Remove it from the list
                    observed_points.erase(v);
                    //std::cout << alias << ": Remove point 1 from component " << co->alias << std::endl;
                }

                // If the second point is in range
                if (((*v.v + ppos) - pos).sqrlen() < radius * radius) {
                    if (map_contains(observed_points, v)) continue;
                    // Add it to the list if it isn't already
                    observed_points.insert({ v, *v.v + ppos });
                    //std::cout << alias << ": Add point 2 from component " << co->alias << std::endl;
                }
                // If the secind point is not in range but in the list
                else if (map_contains(observed_points, v)) {
                    // Remove it from the list
                    observed_points.erase(v);
                    //std::cout << alias << ": Remove point 2 from component " << co->alias << std::endl;
                }
            }
        }

        void update_LastPositions() {
            for (auto& elem : observed_points) {
                elem.second = *elem.first.v;
            }
        }

        Vector2 getMidpoint() {
            return (point2 - point1) * 0.5;
        }

    public:

        Vector2 point1;
        Vector2 point2;

        collider_line(Vector2 p1, Vector2 p2) : point1(p1), point2(p2) {}

        Vector2* collide() override {
            //system("clear");
            for (auto& elem : observed_points) {
                Vector2 p1 = *elem.first.v - point1 - parent->position;
                Vector2 p2 = elem.second - point1 - parent->position;
                if (sign(normal.dot(p1) != sign(normal.dot(p2)))) {
                    //std::cout << alias << ": Probable collision" << std::endl;
                    return new Vector2(p1);
                }
            }

            return nullptr;
        }

        Vector2 getNormal() { return normal; }

        void calcNormal() {
            normal = (point2 - point1).rotate(0.5 * PI).normalized();
        }

        void init() override {
            //dynamics* d = parent->getComponent<dynamics>();
            //if (d) d->hitbox = this;
            calcNormal();
            obj_m::registerCollider(this);
        }

        void update(updateArguments args) override {
            getPointsInRadius(getMidpoint() + parent->position, (point2 - point1).len());
            update_LastPositions();
        }*/

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
