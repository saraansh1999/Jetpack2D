#include "collisions.h"

float penetration;

float dist(float x1, float y1, float x2, float y2)
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

bool CollisionDetector::x_collision(bounding_box_t one, bounding_box_t two)
{
    return (fabs(one.x - two.x) * 2 <= (one.width + two.width));
}

bool CollisionDetector::y_collision(bounding_box_t one, bounding_box_t two)
{
    return (fabs(one.y - two.y) * 2 <= (one.height + two.height));
}

bool CollisionDetector::aa_ver_collision(bounding_box_t &one, bounding_box_t &two)
{
    if((fabs(one.x - two.x) * 2 <= (one.width + two.width)) && (fabs(one.y - two.y) * 2 <= (one.height + two.height)))
    {
        penetration = fabs(fabs(one.y-two.y) - (one.height + two.height)/2);
       
        if(two.vspeed == 1)
            two.y -= penetration;
        else
            two.y += penetration;
        
        return true;
    }

    return false;
}

bool CollisionDetector::normal_collision(bounding_box_t &one, bounding_box_t &two)
{
    return (fabs(one.x - two.x) * 2 <= (one.width + two.width)) && (fabs(one.y - two.y) * 2 <= (one.height + two.height));
}

bool CollisionDetector::inclined_collision(bounding_box_t &one, float angle, bounding_box_t &two, float delta)
{
    glm::vec2 left_corner(one.x - one.width*cos(angle*M_PI/180)/2, one.y - one.width*sin(angle*M_PI/180)/2);
    glm::vec2 right_corner(one.x + one.width*cos(angle*M_PI/180)/2, one.y + one.width*sin(angle*M_PI/180)/2);
    float posx = two.x + two.width/2;
    float posy = two.y + two.height/2;
    if(dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) <= one.width + delta && dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) >= one.width - delta)
        return true;
    posx = two.x - two.width/2;
    posy = two.y + two.height/2;
    if(dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) <= one.width + delta && dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) >= one.width - delta)
        return true;
    posx = two.x + two.width/2;
    posy = two.y - two.height/2;
    if(dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) <= one.width + delta && dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) >= one.width - delta)
        return true;
    posx = two.x - two.width/2;
    posy = two.y - two.height/2;
    if(dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) <= one.width + delta && dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) >= one.width - delta)
        return true; 
    posx = two.x + two.width/2;
    posy = two.y;
    if(dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) <= one.width + delta && dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) >= one.width - delta)
        return true; 
    posx = two.x;
    posy = two.y + two.height/2;
    if(dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) <= one.width + delta && dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) >= one.width - delta)
        return true; 
    posx = two.x - two.width/2;
    posy = two.y;
    if(dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) <= one.width + delta && dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) >= one.width - delta)
        return true; 
    posx = two.x;
    posy = two.y - two.height/2;
    if(dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) <= one.width + delta && dist(posx, posy, left_corner.x, left_corner.y) + dist(posx, posy, right_corner.x, right_corner.y) >= one.width - delta)
        return true; 
    
    return false;
    
}       

bool CollisionDetector::semicircular_collision(bounding_box_t &one, bounding_box_t &two, float delta)
{
    float radius = dist(two.x, two.y, one.x, one.y);
    if(radius <= one.width/2 + delta && radius >= one.width/2 - delta && one.y<two.y)
    {
        float angle = atan((one.y - two.y)/(one.x - two.x));
        if(angle<0)
            angle += M_PI;
        two.x = one.x + (one.width/2)*cos(angle);
        two.y = one.y + (one.width/2)*sin(angle);
        return true;
    }
    
    return false;
}