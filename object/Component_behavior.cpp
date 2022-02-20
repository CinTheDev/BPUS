#pragma once

namespace comp {
    class moveRight : public component {
        using component::component;

    public:
        void update(updateArguments args) override {
            parent->position.x += args.deltatime;
        }

        void init() {}
    };
}
