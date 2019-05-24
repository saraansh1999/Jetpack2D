#include "moving_fire.h"
#include "main.h"

Moving_fire::Moving_fire(float x, float y, float limit_bottom, float limit_top, color_t color, int type) {
    int N = 25;
    this->radius = 20;
    this->position = glm::vec3(x, y, 0);
    //this->rotation = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[9*N] = {
        -200.0f, -8.0f, 0.0f, // triangle 1 : begin
        -200.0f, 8.0f, 0.0f,
        200.0f, 8.0f, 0.0f, // triangle 1 : end
        200.0f, 8.0f, 0.0f, // triangle 2 : begin
        200.0f, -8.0f, 0.0f,
        -200.0f, -8.0f, 0.0f, // triangle 2 : end
    };

    this->object_main = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);    
    this->box.width = 200.0f*2 + this->radius*2;
    this->box.height = 8.0f*2;
    this->box.hspeed = 0;
    this->box.vspeed = 10*type;
    this->limit_top = limit_top;
    this->limit_bottom = limit_bottom;
    float theta = 0;
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data[9*i] = 0.0f;
        vertex_buffer_data[9*i + 1] = 0.0f;
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = radius*cos(theta*M_PI/180.0f);
        vertex_buffer_data[9*i + 4] = radius*sin(theta*M_PI/180.0f);
        vertex_buffer_data[9*i + 5] = 0.0f;
        vertex_buffer_data[9*i + 6] = radius*cos((theta+360.0f/N)*M_PI/180.0f);
        vertex_buffer_data[9*i + 7] = radius*sin((theta+360.0f/N)*M_PI/180.0f);
        vertex_buffer_data[9*i + 8] = 0.0f;
        theta += 360.0f/N;
    }
    this->object1 = create3DObject(GL_TRIANGLES, N*3, vertex_buffer_data, color, GL_FILL);
    theta = 0;
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data[9*i] = 0.0f;
        vertex_buffer_data[9*i + 1] = 0.0f;
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = radius*cos(theta*M_PI/180.0f);
        vertex_buffer_data[9*i + 4] = radius*sin(theta*M_PI/180.0f);
        vertex_buffer_data[9*i + 5] = 0.0f;
        vertex_buffer_data[9*i + 6] = radius*cos((theta+360.0f/N)*M_PI/180.0f);
        vertex_buffer_data[9*i + 7] = radius*sin((theta+360.0f/N)*M_PI/180.0f);
        vertex_buffer_data[9*i + 8] = 0.0f;
        theta += 360.0f/N;
    }
    this->object2 = create3DObject(GL_TRIANGLES, N*3, vertex_buffer_data, color, GL_FILL);
}

void Moving_fire::draw(glm::mat4 VP) {
   Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model = glm::translate(Matrices.model, this->position);
    //Matrices.model = glm::rotate(Matrices.model, (float)(this->angle*M_PI/180), glm::vec3(0.0f, 0.0f, 1.0f));
    //glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object_main);

    Matrices.model = glm::mat4(1.0f);
    Matrices.model = glm::translate(Matrices.model, this->position +  glm::vec3(this->box.width/2 - this->radius, 0, 0));
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);

    Matrices.model = glm::mat4(1.0f);
    Matrices.model = glm::translate(Matrices.model, this->position + glm::vec3(-this->box.width/2 + this->radius, 0, 0));
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object2);
}

void Moving_fire::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Moving_fire::tick() {
    // this->position.x -= speed;
    this->position[1] += this->box.vspeed*delta_time*50;
    if(this->position[1] + 2*this->radius > this->limit_top)
    {
        this->position[1] = this->limit_top - 2*this->radius - 1;
        this->box.vspeed *= -1;
    }
    if(this->position[1] - 2*this->radius < this->limit_bottom)
    {
        this->position[1] = this->limit_bottom + 2*this->radius + 1;
        this->box.vspeed *= -1;
    }
    this->box.x=this->position[0];
    this->box.y = this->position[1];
}

