#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    bool on_ground;
    bool on_ceiling;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(float, float);
    //void tick(float, float, float);
    void tick(int);
    void hor_move(int);
    void hor_move(float, float, float, int);
    void ver_move(int);
    double speed, up_speed, down_speed, hor_accl, angular_speed;
    bounding_box_t box;
private:
    VAO *object;
};

#endif // PLAYER_H
