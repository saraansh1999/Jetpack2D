#include "main.h"

#ifndef ICEBALL_H
#define ICEBALL_H


class Iceball {
public:
    Iceball() {}
    Iceball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    float radius;
    bounding_box_t box;
private:
    VAO *object;
};

#endif // ICEBALL_H
