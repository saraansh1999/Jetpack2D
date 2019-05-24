#include "ring.h"
#include "main.h"

Ring::Ring(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    //this->rotation = 0;
    this->radius = 500;
    int N = 25;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[6*N];

    float angle = 0;
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data[6*i + 0] = this->radius * cos(angle*M_PI/180);
        vertex_buffer_data[6*i + 1] = this->radius * sin(angle*M_PI/180);
        vertex_buffer_data[6*i + 2] = 0.0f;
        angle += 180/N;
        vertex_buffer_data[6*i + 3] = this->radius * cos(angle*M_PI/180);
        vertex_buffer_data[6*i + 4] = this->radius * sin(angle*M_PI/180);
        vertex_buffer_data[6*i + 5] = 0.0f;
    }

    this->box.width = 2*this->radius;
    this->box.height = this->radius;
    this->object = create3DObject(GL_LINES, 2*N, vertex_buffer_data, color, GL_FILL);
}

void Ring::draw(glm::mat4 VP) {
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

void Ring::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->box.x = this->position[0];
    this->box.y = this->position[1];
}

void Ring::tick() {
    // this->position.x -= speed;
    // this->position.y -= speed;
}

