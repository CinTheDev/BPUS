#pragma once

class object;

class component {
public:
    bool enabled = true;
    object* parent;

    virtual void init() {}
    virtual void update(updateArguments args) {}
};
