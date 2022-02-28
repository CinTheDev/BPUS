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

        // First is key, second is last position
        std::map<Vector2*, Vector2> observed_points;

        void getPointsInRadius(Vector2 pos, double radius) {
            std::string debug = alias;
            std::vector<collider*> debug2(obj_m::colliders);
            for (auto& c : obj_m::colliders) {
                // Convert to collider_line if possible
                if (typeid(*c) != typeid(collider_line)) continue;
                if (c->parent == parent) continue;
                collider_line* co = (collider_line*)c;
                Vector2* p1 = &co->point1;
                Vector2* p2 = &co->point2;
                Vector2 ppos = co->parent->position;
                
                // If the first point is in range
                if (((*p1 + ppos) - pos).sqrlen() < radius * radius) {
                    if (map_contains(observed_points, p1)) continue;
                    // Add it to the list if it isn't already
                    observed_points.insert({ p1, *p1 + ppos });
                    //std::cout << alias << ": Add point 1 from component " << co->alias << std::endl;
                }
                // If the first point is not in range but in the list
                else if (map_contains(observed_points, p1)) {
                    // Remove it from the list
                    observed_points.erase(p1);
                    //std::cout << alias << ": Remove point 1 from component " << co->alias << std::endl;
                }

                // If the second point is in range
                if (((*p2 + ppos) - pos).sqrlen() < radius * radius) {
                    if (map_contains(observed_points, p2)) continue;
                    // Add it to the list if it isn't already
                    observed_points.insert({ p2, *p2 + ppos });
                    //std::cout << alias << ": Add point 2 from component " << co->alias << std::endl;
                }
                // If the secind point is not in range but in the list
                else if (map_contains(observed_points, p2)) {
                    // Remove it from the list
                    observed_points.erase(p2);
                    //std::cout << alias << ": Remove point 2 from component " << co->alias << std::endl;
                }
            }
        }

        void update_LastPositions() {
            for (auto& elem : observed_points) {
                elem.second = *elem.first;
            }
        }

        void collide() {
            //system("clear");
            for (auto& elem : observed_points) {
                Vector2 p1 = *elem.first - point1 - parent->position;
                Vector2 p2 = elem.second - point1 - parent->position;
                if (sign(normal.dot(p1) != sign(normal.dot(p2)))) {
                    //std::cout << alias << ": Probable collision" << std::endl;
                }
            }
        }

        Vector2 getMidpoint() {
            return (point2 - point1) * 0.5;
        }

    public:

        Vector2 point1;
        Vector2 point2;

        collider_line(Vector2 p1, Vector2 p2) : point1(p1), point2(p2) {}

        void calcNormal() {
            normal = (point2 - point1).rotate(0.5 * PI).normalized();
        }

        void init() override {
            calcNormal();
            obj_m::registerCollider(this);
        }

        void update(updateArguments args) override {
            collide();
            getPointsInRadius(getMidpoint() + parent->position, (point2 - point1).len());
            update_LastPositions();
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
