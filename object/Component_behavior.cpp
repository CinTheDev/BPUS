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
    public:
        dynamics* rigidbody;
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

        Vector2* getCorners() {
            Vector2 pivot = size*0.5;

		    Vector2 pos0 = (Vector2(-pivot.x, -pivot.y)).rotate(parent->rotation);
		    Vector2 pos1 = (Vector2(-pivot.x,  pivot.y)).rotate(parent->rotation);
		    Vector2 pos2 = (Vector2( pivot.x,  pivot.y)).rotate(parent->rotation);
		    Vector2 pos3 = (Vector2( pivot.x, -pivot.y)).rotate(parent->rotation);

		    pos0 += parent->position + size * 0.5;
		    pos1 += parent->position + size * 0.5;
		    pos2 += parent->position + size * 0.5;
		    pos3 += parent->position + size * 0.5;

            Vector2* edges = new Vector2[4];
            edges[0] = pos0;
            edges[1] = pos1;
            edges[2] = pos2;
            edges[3] = pos3;            
            return edges;
        }

        bool check_collision_rect(Vector2* corners1, Vector2* corners2, double& overlap) {
            for (int i = 0; i < 2; i++) {

                // Use SAT on both shapes
                for (int a = 0; a < 4; a++) {
                    int b = (a + 1) % 4;
                    Vector2 axisProject = Vector2(-(corners1[b].y - corners1[a].y), corners1[b].x - corners1[a].x).normalized();

                    double min_r1 = INFINITY, max_r1 = -INFINITY;
                    for (int p = 0; p < 4; p++) {
                        double q = corners1[p].dot(axisProject);
                        min_r1 = std::min(min_r1, q);
                        max_r1 = std::max(max_r1, q);
                    }

                    double min_r2 = INFINITY, max_r2 = -INFINITY;
                    for (int p = 0; p < 4; p++) {
                        double q = corners2[p].dot(axisProject);
                        min_r2 = std::min(min_r2, q);
                        max_r2 = std::max(max_r2, q);
                    }

                    overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

                    if (!(max_r2 >= min_r1 && max_r1 >= min_r2)) {
                        return false;
                    }
                }

                // Swap both shapes
                Vector2* temp = corners1;
                corners1 = corners2;
                corners2 = temp;
            }
            return true;
        }
        bool check_collision_circle(collider_circle* c, Vector2& closest_point); // Need to define later (after circle)

        bool check_collision() override {
            if (rigidbody == nullptr) return false;

            bool collision = false;

            // Go through rect colliders
            for (auto& c : obj_m::rect_colliders) {
                if (c == this) continue;

                // Check the other one against it
                double overlap = INFINITY;
                Vector2* corners1 = getCorners();
                Vector2* corners2 = c->getCorners();

                if(check_collision_rect(corners1, corners2, overlap)) {
                    resolve_collision_rect(c, overlap);
                    collision = true;
                }

                delete[] corners1, corners2;
            }
            // Go through circle colliders
            for (auto& c : obj_m::circle_colliders) {
                Vector2 closest_point;

                if (check_collision_circle(c, closest_point)) {                    
                    resolve_collision_circle(c, closest_point);
                    collision = true;
                }
            }
            for (auto& c : obj_m::line_colliders) {
                // Go through line colliders
            }

            return collision;
        }

        // Rect vs. rect
        void resolve_collision_rect(collider_rect* target, double overlap) {
            if (target->rigidbody == nullptr) {
                Vector2 d = (target->parent->position - parent->position).normalized();
                parent->position -= d * overlap;
                return;

                // TODO: Dynamic resolution with static
            }

            Vector2 d = (target->parent->position - parent->position).normalized();
            parent->position -= d * overlap * 0.5;
            target->parent->position += d * overlap * 0.5;

            // TODO: Dynamic resolution with dynamic
        }

        // Rect vs. circle
        void resolve_collision_circle(collider_circle* target, Vector2 close_dist); // Algorithm is after cirlce definition

        // Rect vs. line
        void resolve_collision_line(Vector2 close_dist) {
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

        bool check_collision_circle(Vector2 pos, float r) {
            double sqrlen = (pos - parent->position).sqrlen();
            if (sqrlen <= (radius + r)*(radius + r)) {
                return true;
            }
            return false;
        }
        bool check_collision_line(Vector2 p1, Vector2 p2, Vector2& closest_point) {
            Vector2 segment = p2 - p1;
            Vector2 d1 = parent->position + offset - p1;

            double s_len = segment.sqrlen();

            double t = clamp(0.0, segment.dot(d1), s_len) / s_len;

            closest_point = p1 + segment * t;

            double distance = (parent->position + offset - closest_point).len();

            if (distance <= radius) {
                return true;
            }

            return false;
        }
        bool check_collision_rect(Vector2* corners, Vector2& closest_point) {
            for (int i = 0; i < 4; i++) {
                Vector2 start = corners[i];
                Vector2 end = corners[(i + 1) % 4];

                if (check_collision_line(start, end, closest_point)) {
                    return true;
                }
            }

            return false;
        }

        bool check_collision() override {
            if (rigidbody == nullptr) return false;

            bool collision = false;

            // Circle vs. circle
            for (auto& c : obj_m::circle_colliders) {
                if (c == this) continue;

                if (check_collision_circle(c->parent->position, c->radius)) {
                    resolve_collision_circle(c);
                    collision = true;
                }
            }
            // Circle vs. rect
            for (auto& c : obj_m::rect_colliders) {
                Vector2* corners = c->getCorners();
                Vector2 closest_point;
                if(check_collision_rect(corners, closest_point)) {
                    resolve_collision_rect(c, closest_point);
                    collision = true;
                }

                delete[] corners;
            }
            // Circle vs. line
            for (auto& c : obj_m::line_colliders) {
                Vector2 closest_point;
                if (check_collision_line(c->p1, c->p2, closest_point)) {
                    resolve_collision_line(closest_point);
                    collision = true;
                }
            }
            return collision;
        }

        // Circle vs. circle
        void resolve_collision_circle(comp::collider_circle* target) {
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
        void resolve_collision_line(Vector2 close_dist) {
            Vector2 dist = close_dist - parent->position - offset;
            // Static resolution
            parent->position += dist.normalized() * (dist.len() - radius);

            // Dynamic resolution is just flipping the own speed.
            dist = dist.normalized() * bounciness;
            rigidbody->speed -= dist * dist.dot(rigidbody->speed) * (2 / bounciness);
        }

        // Circle vs. rect
        void resolve_collision_rect(collider_rect* target, Vector2 close_dist) {
            if (target->rigidbody == nullptr) {
                Vector2 dist = close_dist - parent->position - offset;
                Vector2 dist_o = dist.normalized() * (dist.len() - radius);

                parent->position += dist_o;

                dist = dist.normalized() * bounciness;
                rigidbody->speed -= dist * dist.dot(rigidbody->speed) * (2 / bounciness);
            }

            Vector2 dist = close_dist - parent->position - offset;
            Vector2 dist_o = dist.normalized() * (dist.len() - radius);

            parent->position += dist_o * 0.5;
            target->parent->position -= dist_o * 0.5;

            // TODO: Dynamic resolution
        }        
    };

    // Definition for rect (it needs to know what a circle is)
    bool collider_rect::check_collision_circle(collider_circle* c, Vector2& closest_point) {
        Vector2* corners = getCorners();
        if (c->check_collision_rect(corners, closest_point)) {
            delete[] corners;
            return true;
        }
        delete[] corners;
        return false;
    }

    // Definition for rect (it needs to know what a circle is)
    void collider_rect::resolve_collision_circle(collider_circle* target, Vector2 close_dist) {
        if (target->rigidbody == nullptr) {
            Vector2 dist = close_dist - target->parent->position - target->offset;
            Vector2 dist_o = dist.normalized() * (dist.len() - target->radius);

            parent->position -= dist_o * 0.5;

            return;
        }

        target->resolve_collision_rect(this, close_dist);

        // TODO: Implement dynamic resolution
    }
}
