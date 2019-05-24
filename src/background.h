#include "main.h"

#ifndef BACK_H
#define BACK_H


class Back {
public:
    Back() {}
    Back(float x, float y, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t box;
private:
    VAO *object;
};

#endif // BACK_H
