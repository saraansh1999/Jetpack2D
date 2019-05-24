#include "main.h"

#ifndef BALLOONS_H
#define BALLOONS_H


class Balloons {
public:
    Balloons() {}
    Balloons(float x, float y, color_t color);
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

#endif // BALLOONS_H
