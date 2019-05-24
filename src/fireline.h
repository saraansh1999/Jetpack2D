#include "main.h"

#ifndef FIRELINE_H
#define FIRELINE_H


class Fireline {
public:
    Fireline() {}
    Fireline(float x, float y, float angle, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float length, angle, radius;
    bounding_box_t box;
private:
    VAO *object_main;
    VAO *object1;
    VAO *object2;
};

#endif // FIRELINE_H
