#include "main.h"

#ifndef COINS_H
#define COINS_H


class Coins {
public:
    Coins() {}
    Coins(float x, float y, color_t color, int type);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    float radius;
    int type;
    bounding_box_t box;
private:
    VAO *object;
};

#endif // COINS_H
