#include "player.h"
#include "main.h"
#include "timer.h"

float g_down = 9.8, g_up = 9.8, hor_attraction_speed = 20, ver_attraction_speed = 30;
float multiplier = 12.0f;
bool projectile, magnetic_influence = false;

Player::Player(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    //this->rotation = 0;
    speed = 7;
    up_speed = 5;
    down_speed = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -50.0f, -50.0f, 0.0f, // triangle 1 : begin
        -50.0f, 50.0f, 0.0f,
        50.0f, 50.0f, 0.0f, // triangle 1 : end
        50.0f, 50.0f, 0.0f, // triangle 2 : begin
        50.0f, -50.0f, 0.0f,
        -50.0f, -50.0f, 0.0f, // triangle 2 : end
    };

    this->on_ground = true;
    this->on_ceiling = false;
    this->box.height = 50.0f*2;
    this->box.width = 50.0f*2;
    this->box.hspeed = this->box.vspeed = 0;
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Player::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    //glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Player::hor_move(int dir)
{
    if(!this->on_ground && this->box.vspeed == -1 && !magnetic_influence)
        projectile = true;
    else
        this->position[0] += dir*speed;
    // if(speed<20)
    //     speed += dir*this->hor_accl*delta_time*multiplier;
    this->box.hspeed = dir;
}

void Player::hor_move(float x, float y, float r, int dir)
{
    projectile = false;
    angular_speed = 10;
    float angle = atan((y - this->position[1])/(x - this->position[0]));
    if(angle<0)
    {
        angle += (angular_speed*M_PI/180)*delta_time*multiplier*(-dir);
        if(angle<0)
            angle+=M_PI;
        else
            if(x>this->position[0])
            {
                this->position[0] = x - r*cos(angle);
                this->position[1] = y - r*sin(angle);
                this->box.x = this->position[0];
                this->box.y = this->position[1];
                return;
            }            
    }
    else
    {
        angle += (angular_speed*M_PI/180)*delta_time*multiplier*(-dir);
        //if(angle<0)
        //    ring_influence = false;
    }
    this->position[0] = x + r*cos(angle);
    this->position[1] = y + r*sin(angle);
    this->box.x = this->position[0];
    this->box.y = this->position[1];
}

void Player::ver_move(int dir)
{
    if(dir==1)
    {
        if(this->box.y + this->box.height/2 < height)
        {
        down_speed=0;
        this->on_ground = false;
        this->position[1] += up_speed*delta_time*multiplier;
        up_speed += g_up*delta_time*multiplier;
        }
        else
            this->on_ceiling = true;
        projectile = false;
    }
    if(dir==-1 && !this->on_ground)
    {
        up_speed=0;
        this->on_ceiling = false;
        if(!magnetic_influence)
        {
        this->position[1] -= down_speed*delta_time*multiplier;
        down_speed += g_down*delta_time*multiplier;
        }

    }
    if(this->on_ground || this->on_ceiling)
        projectile = false;
    this->box.vspeed = dir;
}

void Player::tick(int restrict_projectile)
{
    //this->hor_accl = 0;
    if(restrict_projectile)
        projectile = false;
    magnetic_influence = false;
    speed = 7;
    g_up = 9.8;
    hor_attraction_speed = 20;
    ver_attraction_speed = 30;
    if(projectile)
        this->position[0] += this->box.hspeed*speed;
    this->box.x = this->position[0];
    this->box.y = this->position[1];
}

void Player::tick(float xmag, float ymag)
{
    g_up = 5;
    if(projectile)
        projectile = false;
    magnetic_influence = true;
    //this->hor_accl = (xmag - this->position[0])/fabs(xmag - this->position[0]) * g_up*delta_time*multiplier;
    this->position[0] += (xmag - this->position[0])/fabs(xmag - this->position[0]) * hor_attraction_speed * delta_time * multiplier;
    this->position[1] += (ymag - this->position[1])/fabs(ymag - this->position[1]) * ver_attraction_speed * delta_time * multiplier;
    // if((ymag - this->position[1])>0)
    //     ver_attraction_speed += g_down*delta_time*multiplier;
    if(projectile)
        this->position[0] += this->box.hspeed*speed;
    this->box.x = this->position[0];
    this->box.y = this->position[1];
}


