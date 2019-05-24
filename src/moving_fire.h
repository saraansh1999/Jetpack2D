#include "main.h"

#ifndef MOVING_FIRE_H
#define MOVING_FIRE_H


class Moving_fire {
public:
    Moving_fire() {}
    Moving_fire(float x, float y, float limit_bottom, float limit_top, color_t color, int type);
    glm::vec3 position;
    float rotation;
    int type;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float limit_bottom, limit_top, radius;
    bounding_box_t box;
private:
    VAO *object_main;
    VAO *object1;
    VAO *object2;
};

#endif // MOVING_FIRE_H
