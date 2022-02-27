#pragma once

class object;

class component {
public:
    bool enabled = true;
    object* parent;
    std::string alias;

    bool operator==(const component& c) { return this == &c; }
    bool operator!=(const component& c) { return this != &c; }

    virtual void init() {}
    virtual void update(updateArguments args) {}
};
