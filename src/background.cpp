#include "background.h"
#include "main.h"

Back::Back(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, -9);
    //this->rotation = 0;
    //speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -width, -height, 0.0f, // triangle 1 : begin
        -width, height, 0.0f,
        tot, height, 0.0f, // triangle 1 : end
        tot, height, 0.0f, // triangle 2 : begin
        tot, -height, 0.0f,
        -width, -height, 0.0f, // triangle 2 : end
    };

    this->box.height = 90.0f*2;
    this->box.width = tot*2;
    this->box.hspeed = this->box.vspeed = 0;
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Back::draw(glm::mat4 VP) {
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

void Back::set_position(float x, float y) {
    this->position = glm::vec3(x, y, -9);
}

void Back::tick() {
    //this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
    this->box.x = this->position[0];
    this->box.y = this->position[1];
}


