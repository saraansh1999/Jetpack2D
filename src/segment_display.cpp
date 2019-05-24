#include "segment_display.h"
#include "main.h"

int numbers[][7] = {
    1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 0, 0, 0, 0,
    1, 1, 0, 1, 1, 0, 1,
    1, 1, 1, 1, 0, 0, 1,
    0, 1, 1, 0, 0, 1, 1, 
    1, 0, 1, 1, 0, 1, 1,
    0, 0, 1, 1, 1, 1, 1, 
    1, 1, 1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 1, 1,
    0, 0, 0, 1, 1, 1, 0,
};

Segment::Segment(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    //this->rotation = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data6[] = {
        -20, 5, 0,
        -20, -5, 0,
        20, -5, 0,
        20, -5, 0,
        20, 5, 0,
        -20, 5, 0,
    };

    static const GLfloat vertex_buffer_data0[] = {
        20, 35, 0,
        -20, 25, 0,
        20, 25, 0,
        -20, 25, 0,
        20, 35, 0,
        -20, 35, 0,
    };

    static const GLfloat vertex_buffer_data3[] = {
        -20, -25, 0,
        -20, -35, 0,
        20, -35, 0,
        20, -35, 0,
        20, -25, 0,
        -20, -25, 0,
    };

    static const GLfloat vertex_buffer_data5[] = {
        -20, -5, 0,
        -10, -5, 0,
        -10, 35, 0,
        -10, 35, 0,
        -20, -5, 0,
        -20, 35, 0,
    };

    static const GLfloat vertex_buffer_data1[] = {
        10, -5, 0,
        20, -5, 0,
        20, 35, 0,
        20, 35, 0,
        10, -5, 0,
        10, 35, 0,
    };

    static const GLfloat vertex_buffer_data4[] = {
        -20, -35, 0,
        -10, -35, 0,
        -10, 5, 0,
        -10, 5, 0,
        -20, -35, 0,
        -20, 5, 0,
    };

    static const GLfloat vertex_buffer_data2[] = {
        10, -35, 0,
        20, -35, 0,
        20, 5, 0,
        20, 5, 0,
        10, -35, 0,
        10, 5, 0,
    };

    this->object[0] = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data0, color, GL_FILL);
    this->object[1] = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data1, color, GL_FILL);
    this->object[2] = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data2, color, GL_FILL);
    this->object[3] = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data3, color, GL_FILL);
    this->object[4] = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data4, color, GL_FILL);
    this->object[5] = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data5, color, GL_FILL);
    this->object[6] = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data6, color, GL_FILL);
    this->box.width = 20*2;
    this->box.height = 20*2*2;
}

void Segment::draw(glm::mat4 VP, int dig) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    //glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    for(int i=0;i<7;i++)
    {
        if(numbers[dig][i] == 1)
            draw3DObject(this->object[i]);
    }
}

void Segment::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Segment::tick() {
    // this->position.x -= speed;
    // this->position.y -= speed;
}

