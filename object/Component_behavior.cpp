#pragma once

namespace comp {
    class dynamics : public component {
        // Component for everything related to dynamics (movement and acceleration)
    private:
    public:
        Vector2 speed = Vector2(); // Speed in m/s (actually units/second but lets use real units for the sake of physics)
        Vector2 acceleration = Vector2(); // in m/s²

        double angularSpeed = 0;
        double angularAcceleration = 0;

        double mass = 1; // Mass in kg

        const double gravity = -9.81;

        // Adds a certain amount of energy to the object (with direction)
        void addForce(Vector2 joules) {
            // using v = sqrt(2W / m)
            speed.x += sqrt(abs(joules.x) / (0.5 * mass)) * sign(joules.x);
            speed.y += sqrt(abs(joules.y) / (0.5 * mass)) * sign(joules.y);
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

            angularSpeed += angularAcceleration * args.deltatime;

            // I know Euler's method isn't great, but I'm not smart enough to use better stuff
            parent->position += speed * args.deltatime;
            parent->rotation += angularSpeed * args.deltatime;

            acceleration = Vector2();
            angularAcceleration = 0;
        }
    };


    class collider : public component {
        // General class used for colliders
    public:
        dynamics* rigidbody;
        double bounciness = 0.8;

        virtual bool check_collision() { return false; }
        virtual void resolve_collision(Vector2 collision) {}

        // Gets the Moment of Inertia for the specific shape
        virtual double get_MoI() { return 0; }
        void addAngularSpeed(double joules) {
            // Uses v = sqrt( W / (0.5 * I) )
            rigidbody->angularSpeed += sqrt( abs(joules) / (0.5 * get_MoI()) ) * sign(joules);
        }

        void addForce(Vector2 point, Vector2 force) {
            // https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics modified
            // r is the offset (point)
            // f is the force (force)

            double t = point.x * force.y - point.y * force.x;

            rigidbody->acceleration += force / rigidbody->mass;
            rigidbody->angularAcceleration += t / get_MoI();

            //rigidbody->angularSpeed += t / get_MoI(rigidbody->mass);
            //rigidbody->speed += force / rigidbody->mass;
        }

        void init() override {}
        void update(updateArguments args) override {}

        virtual ~collider() override {}
    };

    class collider_line : public collider {
        // Note: this collider can only be used as a static collider, no dynamics.
    private:
    public:
        Vector2 p1;
        Vector2 p2;

        Vector2 normal;

        void calculateNormal() {
            normal = p2 - p1;
            normal = Vector2(-normal.y, normal.x).normalized();
        }
        Vector2 getMiddle() {
            return p1 + (p2 - p1) * 0.5;
        }

        collider_line() {
            p1 = Vector2();
            p2 = Vector2();

            normal = Vector2();
        }
        collider_line(Vector2 p1, Vector2 p2) : p1(p1), p2(p2) {
            calculateNormal();
        }
        ~collider_line() override {
            obj_m::removeLine(this);
        }

        double get_MoI(double mass) {
            // Uses I = 1/12 * m * l²
            double len = (p2 - p1).len();
            return 1/12 * mass * len*len;
        }

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
    private:
        bool inside_rect(Vector2 point, Vector2* corners) {
            Vector2 debug0 = corners[0];
            Vector2 debug1 = corners[1];
            Vector2 debug2 = corners[2];
            Vector2 debug3 = corners[3];

            Vector2 AM = point - corners[0];
            Vector2 AB = corners[1] - corners[0];
            Vector2 AD = corners[3] - corners[0];
            return (0 < AM.dot(AB) < AB.dot(AB)) && (0 < AM.dot(AD) < AD.dot(AD));
        }

        bool SAT(Vector2* corners_1, Vector2* corners_2, double& overlap, Vector2& point) {
            for (int i = 0; i < 2; i++) {

                // Use SAT on both shapes
                for (int a = 0; a < 4; a++) {
                    int b = (a + 1) % 4;
                    Vector2 axisProject = Vector2(-(corners_1[b].y - corners_1[a].y), corners_1[b].x - corners_1[a].x).normalized();

                    double min_r1 = INFINITY, max_r1 = -INFINITY;
                    for (int p = 0; p < 4; p++) {
                        double q = corners_1[p].dot(axisProject);
                        
                        if (!inside_rect(corners_1[p], corners_2)) {
                            point = corners_1[p];
                        }

                        min_r1 = std::min(min_r1, q);
                        max_r1 = std::max(max_r1, q);
                    }

                    double min_r2 = INFINITY, max_r2 = -INFINITY;
                    for (int p = 0; p < 4; p++) {
                        double q = corners_2[p].dot(axisProject);

                        min_r2 = std::min(min_r2, q);
                        max_r2 = std::max(max_r2, q);
                    }

                    double new_overlap = std::min(max_r1, max_r2) - std::max(min_r1, min_r2);
                    overlap = std::min(overlap, new_overlap);

                    if (!(max_r2 >= min_r1 && max_r1 >= min_r2)) {
                        return false;
                    }
                }

                // Swap both shapes
                Vector2* temp_vec = corners_1;
                corners_1 = corners_2;
                corners_2 = temp_vec;
            }

            return true;
        }

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
        ~collider_rect() override {
            obj_m::removeRect(this);
        }

        double get_MoI() override {
            // Uses I = 1/12 * m * (h² + w²)
            return 1.0/12.0 * rigidbody->mass * (size.sqrlen());
        }

        void init() override {
            obj_m::registerRect(this);
            rigidbody = parent->getComponent<dynamics>(false);
        }

        void update(updateArguments args) override {
            Vector3 color = Vector3(0, 1, 0);
            if (check_collision()) color = Vector3(1, 0, 0);
            debug::draw_rect(parent->position + offset, size, parent->rotation, color);
        }

        Vector2 getCenter() {
            return parent->position + offset + size*0.5;
        }

        Vector2* getCorners() {
            Vector2 pivot = size*0.5;

		    Vector2 pos0 = (Vector2(-pivot.x, -pivot.y)).rotate(parent->rotation);
		    Vector2 pos1 = (Vector2(-pivot.x,  pivot.y)).rotate(parent->rotation);
		    Vector2 pos2 = (Vector2( pivot.x,  pivot.y)).rotate(parent->rotation);
		    Vector2 pos3 = (Vector2( pivot.x, -pivot.y)).rotate(parent->rotation);

		    pos0 += getCenter();
		    pos1 += getCenter();
		    pos2 += getCenter();
		    pos3 += getCenter();

            Vector2* edges = new Vector2[4];
            edges[0] = pos0;
            edges[1] = pos1;
            edges[2] = pos2;
            edges[3] = pos3;            
            return edges;
        }

        bool check_collision_rect(collider_rect* c1, collider_rect* c2, double& overlap, Vector2& coll_point) {
            Vector2* corners_1 = c1->getCorners();
            Vector2* corners_2 = c2->getCorners();
            bool result = SAT(corners_1, corners_2, overlap, coll_point);
            delete[] corners_1, corners_2;
            return result;
        }
        bool check_collision_circle(collider_circle* c, Vector2& closest_point); // Need to define later (after circle)

        bool check_collision_line(collider_line* c, double& overlap, Vector2& coll_point) {
            // Calculate make line like a big rect, but in a way that it still handles like a regular line
            Vector2* lineCorners = new Vector2[4] {
                c->p1,
                c->p2,
                c->p2 + c->normal * sign(c->normal.dot(getCenter() - c->p1)) * -1, // p3
                c->p1 + c->normal * sign(c->normal.dot(getCenter() - c->p1)) * -1, // p4
            };

            Vector2* rectCorners = getCorners();
            bool result = SAT(lineCorners, rectCorners, overlap, coll_point);
            delete[] lineCorners, rectCorners;
            return result;
        }

        bool check_collision() override {
            if (rigidbody == nullptr) return false;

            bool collision = false;

            // Go through rect colliders
            for (auto& c : obj_m::rect_colliders) {
                if (c == this) continue;

                // Check the other one against it
                double overlap = INFINITY;
                Vector2 coll_point = Vector2();

                if(check_collision_rect(this, c, overlap, coll_point)) {
                    resolve_collision_rect(c, overlap, coll_point);
                    collision = true;
                }
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
                double overlap = INFINITY;
                Vector2 coll_point = Vector2();

                if (check_collision_line(c, overlap, coll_point)) {
                    resolve_collision_line(c, overlap, coll_point);
                    collision = true;
                }
            }

            return collision;
        }

        void rect_collide_static(Vector2 point, Vector2 normal) {
            debug::draw_ray(getCenter(), point);

            double inverted_mass = 1 / rigidbody->mass;

            Vector3 c_n_3 = Vector3(normal.x, normal.y, 0);
            Vector3 c_p_3 = Vector3(point.x, point.y, 0);
            Vector3 theta = c_p_3.cross(c_n_3).cross(c_p_3) / get_MoI();
            double J = (normal.dot(rigidbody->speed * -(1 + bounciness)))/(inverted_mass + c_n_3.dot(theta));

            double vj = normal.normalized().dot(rigidbody->speed * -(1 + bounciness));
            rigidbody->speed += normal.normalized() * J * inverted_mass;

            rigidbody->angularSpeed += point.cross(normal * J) / get_MoI();
        }

        void rect_collide_dynamic(Vector2 point, Vector2 normal, collider_rect c) {
            // TODO: this
        }

        // Rect vs. rect
        void resolve_collision_rect(collider_rect* target, double overlap, Vector2 coll_point) {
            if (target->rigidbody == nullptr) {
                Vector2 d = (target->getCenter() - getCenter()).normalized();
                parent->position -= d * overlap;

                // TODO: Dynamic resolution with static
                coll_point -= getCenter();
                Vector2 coll_norm = d - getCenter();
                rect_collide_static(coll_point, coll_norm);

                return;
            }

            Vector2 d = (target->getCenter() - getCenter()).normalized();
            parent->position -= d * overlap * 0.5;
            target->parent->position += d * overlap * 0.5;

            // TODO: Dynamic resolution with dynamic
        }

        // Rect vs. circle
        void resolve_collision_circle(collider_circle* target, Vector2 close_dist); // Algorithm is after cirlce definition

        // Rect vs. line
        void resolve_collision_line(collider_line* target, double overlap, Vector2 coll_point) {
            // Project the center on the line and offset
            Vector2 l = target->p2 - target->p1;
            float t = l.dot(getCenter() - target->p1) / l.sqrlen();
            Vector2 point = target->p1 + l * t;

            Vector2 d = (point - getCenter()).normalized();
            parent->position -= d * overlap;

            // Linear impulse
            Vector2 coll_norm = point - getCenter();
            coll_point -= getCenter();

            rect_collide_static(coll_point, coll_norm);
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
        ~collider_circle() override {
            obj_m::removeCircle(this);
        }

        Vector2 getPos() {
            return parent->position + offset;
        }

        double get_MoI() override {
            // Uses I = 1/2 * m * r²
            return 1.0/2.0 * rigidbody->mass * radius*radius;
        }

        void init() override {
            obj_m::registerCircle(this);
            rigidbody = parent->getComponent<dynamics>(false);
        }

        void update(updateArguments args) override {
            // Debug
            Vector3 color = Vector3(0, 0, 1);
            if (check_collision()) color = Vector3(1, 0, 0);
            debug::draw_ellipse(parent->position + offset, Vector2(radius, radius), parent->rotation, color);
        }

        bool check_collision_circle(Vector2 pos, float r) {
            double sqrlen = (pos - getPos()).sqrlen();
            if (sqrlen <= (radius + r)*(radius + r)) {
                return true;
            }
            return false;
        }
        bool check_collision_line(Vector2 p1, Vector2 p2, Vector2& closest_point) {
            Vector2 segment = p2 - p1;
            Vector2 d1 = getPos() - p1;

            double s_len = segment.sqrlen();

            double t = clamp(0.0, segment.dot(d1), s_len) / s_len;

            closest_point = p1 + segment * t;

            double distance = (getPos() - closest_point).len();

            if (distance <= radius) {
                return true;
            }

            return false;
        }
        bool check_collision_rect(comp::collider_rect* c, Vector2& closest_point) {
            bool collision = false;

            Vector2* corners = c->getCorners();

            closest_point = getPos().x < c->getCenter().x ? Vector2(0, 0) : Vector2(INFINITY, INFINITY);
            Vector2 close_dist = Vector2();

            for (int i = 0; i < 4; i++) {
                Vector2 start = corners[i];
                Vector2 end = corners[(i + 1) % 4];

                if (check_collision_line(start, end, close_dist)) {
                    collision = true;

                    // After 3 hours of testing and frustration I found out that the weird collision bug only affects one side
                    // of the shape, the side depending on if I use min or max. So I cover both sides with this cheap solution.
                    if (parent->position.x < c->parent->position.x) {
                        if (closest_point.sqrlen() <= close_dist.sqrlen()) closest_point = close_dist;
                    }
                    else {
                        if (closest_point.sqrlen() >= close_dist.sqrlen()) closest_point = close_dist;
                    }
                }
            }

            delete[] corners;

            return collision;
        }

        bool check_collision() override {
            if (rigidbody == nullptr) return false;

            bool collision = false;

            // Circle vs. circle
            for (auto& c : obj_m::circle_colliders) {
                if (c == this) continue;

                if (check_collision_circle(c->getPos(), c->radius)) {
                    resolve_collision_circle(c);
                    collision = true;
                }
            }
            // Circle vs. rect
            for (auto& c : obj_m::rect_colliders) {
                Vector2 closest_point;
                if(check_collision_rect(c, closest_point)) {
                    resolve_collision_rect(c, closest_point);
                    collision = true;
                }
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
            Vector2 dist = target->getPos() - getPos();
            double d = dist.len();
            dist.normalize();
            Vector2 point = dist * (radius + 0.5*(d - radius - target->radius));

            // Debug
            debug::draw_ray(getPos(), point * 0.95, Vector3(0, 1, 0));

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
            Vector2 dist = close_dist - getPos();
            // Static resolution
            parent->position += dist.normalized() * (dist.len() - radius);

            // Dynamic resolution is just flipping the own speed.
            dist = dist.normalized() * bounciness;
            rigidbody->speed -= dist * dist.dot(rigidbody->speed) * (2 / bounciness);
        }

        // Circle vs. rect
        void resolve_collision_rect(collider_rect* target, Vector2 close_dist) {
            if (target->rigidbody == nullptr) {
                Vector2 dist = close_dist - getPos();
                Vector2 dist_o = dist.normalized() * (dist.len() - radius);

                parent->position += dist_o;

                dist = dist.normalized() * bounciness;
                rigidbody->speed -= dist * dist.dot(rigidbody->speed) * (2 / bounciness);

                return;
            }

            Vector2 dist = close_dist - getPos();
            Vector2 dist_o = dist.normalized() * (dist.len() - radius);

            parent->position += dist_o * 0.5;
            target->parent->position -= dist_o * 0.5;

            // TODO: Dynamic resolution
        }        
    };

    // Definition for rect (it needs to know what a circle is)
    bool collider_rect::check_collision_circle(collider_circle* c, Vector2& closest_point) {
        return c->check_collision_rect(this, closest_point);
    }

    // Definition for rect (it needs to know what a circle is)
    void collider_rect::resolve_collision_circle(collider_circle* target, Vector2 close_dist) {
        if (target->rigidbody == nullptr) {
            Vector2 dist = close_dist - target->parent->position - target->offset;
            Vector2 dist_o = dist.normalized() * (dist.len() - target->radius);

            parent->position -= dist_o;

            return;
        }

        target->resolve_collision_rect(this, close_dist);

        // TODO: Implement dynamic resolution
    }
}
