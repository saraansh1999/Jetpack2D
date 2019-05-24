#include "main.h"

#ifndef META_H
#define META_H


class Meta {
public:
    Meta();
    void hit(GLFWwindow *window);
    void coin_collect(int);
    void powerup_collect(int);
    int get_coins();
    int get_lives();
private:
    int lives;
    int coins;
};

#endif // META_H
