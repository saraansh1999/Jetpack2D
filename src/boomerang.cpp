#include "boomerang.h"
#include "main.h"

Boomerang::Boomerang(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    //this->rotation = 0;
    speed = -100;
    ver_speed = 25;
    hor_accl = 5;
    multiplier = 10;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        0, 0, 0,
        0, 50, 0,
        -30, -30, 0,
        0, 0, 0,
        0, 50, 0,
        30, -30, 0,
    };

    this->box.width = 30*2;
    this->box.height = 50 + 30;
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Boomerang::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    Matrices.model = glm::translate (Matrices.model, this->position);    // glTranslatef
    Matrices.model = glm::rotate(Matrices.model, float(glfwGetTime()), glm::vec3(0, 0, 1));
    //glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Boomerang::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    speed = -100;
    ver_speed = 25;
}

void Boomerang::tick() {
    this->rotation += speed;
    this->position[1] -= ver_speed*delta_time*multiplier;
    this->position[0] += speed*delta_time*multiplier;
    speed += hor_accl*delta_time*multiplier;
    // this->position.x -= speed;
    // this->position.y -= speed;
    this->box.x = this->position[0];
    this->box.y = this->position[1];
}

