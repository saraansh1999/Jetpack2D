#include "iceball.h"
#include "main.h"

Iceball::Iceball(float x, float y, color_t color) {
    int N=20;
    float angle=0;
    float r=30.0f;
    this->position = glm::vec3(x, y, 0);
    //this->rotation = 0;
    speed = 10;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[9*N];
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data[9*i] = 0.0f;
        vertex_buffer_data[9*i + 1] = 0.0f;
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = r*cos(angle*M_PI/180.0f);
        vertex_buffer_data[9*i + 4] = r*sin(angle*M_PI/180.0f);
        vertex_buffer_data[9*i + 5] = 0.0f;
        vertex_buffer_data[9*i + 6] = r*cos((angle+360.0f/N)*M_PI/180.0f);
        vertex_buffer_data[9*i + 7] = r*sin((angle+360.0f/N)*M_PI/180.0f);
        vertex_buffer_data[9*i + 8] = 0.0f;
        angle += 360.0f/N;
    }

    this->object = create3DObject(GL_TRIANGLES, N*3, vertex_buffer_data, color, GL_FILL);
    this->radius = r;
    this->box.height = r*2;
    this->box.width = r*2;
    this->box.hspeed = this->box.vspeed = 0;
    this->box.y = y;
}

void Iceball::draw(glm::mat4 VP) {
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

void Iceball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Iceball::tick() {
    this->position[0] -= speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
    this->box.x = this->position[0];
}

