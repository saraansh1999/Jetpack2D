#include "main.h"

#ifndef VISERION_H
#define VISERION_H


class Viserion {
public:
    Viserion() {}
    Viserion(float x, float y, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(float, float);
private:
    VAO *object;
};

#endif // VISERION_H
