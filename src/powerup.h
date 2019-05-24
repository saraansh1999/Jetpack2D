#include "main.h"

#ifndef POWERUP_H
#define POWERUP_H


class Powerup {
public:
    Powerup() {}
    Powerup(float x, float y, color_t color, int type);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool tick(float, float);
    double speed, ver_speed, multiplier;
    float radius;
    int type;
    bounding_box_t box;
private:
    VAO *object;
};

#endif // POWERUP_H
