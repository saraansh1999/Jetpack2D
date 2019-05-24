#include "main.h"

#ifndef PROP_H
#define PROP_H


class Prop {
public:
    Prop() {}
    Prop(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double vspeed, hspeed, acc, multiplier;
    float radius;
    bounding_box_t box;
private:
    VAO *object;
};

#endif // PROP_H
