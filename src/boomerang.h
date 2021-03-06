#include "main.h"

#ifndef BOOMERANG_H
#define BOOMERANG_H


class Boomerang {
public:
    Boomerang() {}
    Boomerang(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed, ver_speed, hor_accl, g, multiplier;
    bounding_box_t box;
private:
    VAO *object;
};

#endif // BOOMERANG_H
