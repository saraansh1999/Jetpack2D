#include "main.h"

#ifndef COLLISIONS_H
#define COLLISIONS_H

class CollisionDetector{

public:
    CollisionDetector() {}
    bool x_collision(bounding_box_t, bounding_box_t);
    bool y_collision(bounding_box_t, bounding_box_t);
    bool aa_ver_collision(bounding_box_t &one, bounding_box_t &two);
    bool normal_collision(bounding_box_t &one, bounding_box_t &two);
    bool inclined_collision(bounding_box_t &one, float angle, bounding_box_t &two, float delta);
    bool semicircular_collision(bounding_box_t &one, bounding_box_t &two, float delta);
};

#endif
