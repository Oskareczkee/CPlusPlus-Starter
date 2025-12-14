#include <GL/glew.h>
#include <iostream>

#include "KDron.h"
#include "vertices.h"

KDron::KDron(float init_velocity, float init_angle) {
    velocity_ = init_velocity;
    angle_ = init_angle;
    animated_ = true;
}


void KDron::Move(float delta_t) {
    if (!animated_) return;
    angle_ += delta_t * velocity_;
    if (angle_ > 360) angle_ -= 360;
    if (angle_ < -360) angle_ += 360;

    //set current angles
    this->pitch_ = angle_;
    this->yaw_ = angle_;

    model_matrix_.SetUnitMatrix();
    model_matrix_.RotateAboutX(angle_);
    model_matrix_.RotateAboutY(angle_);
}



void KDron::SpeedUp() {
    velocity_ *= 1.09544511501;
}

void KDron::SlowDown() {
    velocity_ /= 1.09544511501;
}

void KDron::ToggleAnimated() {
    animated_ = !animated_;
}

void KDron::Rotate(float pitch, float yaw, float roll)
{

    this->pitch_ = this->pitch_ + pitch;
    this->yaw_ = this->yaw_ + yaw;
    this->roll_ = this->roll_ + roll;

    if (pitch_ > 360) pitch_ -= 360;    if (pitch_ < -360) pitch_ += 360;
    if (yaw_ > 360) yaw_ -= 360;        if (yaw_ < -360) yaw_ += 360;
    if (roll_ > 360) roll_ -= 360;      if (roll_ < -360) roll_ += 360;

    model_matrix_.SetUnitMatrix();
    model_matrix_.RotateAboutY(yaw_);
    model_matrix_.RotateAboutX(pitch_);
    model_matrix_.RotateAboutZ(roll_);
}

void KDron::SetRotation(float pitch, float yaw, float roll)
{
    this->pitch_ = pitch;
    this->yaw_ = yaw;
    this->roll_ = roll;

    if (pitch_ > 360) pitch_ -= 360;    if (pitch_ < -360) pitch_ += 360;
    if (yaw_ > 360) yaw_ -= 360;        if (yaw_ < -360) yaw_ += 360;
    if (roll_ > 360) roll_ -= 360;      if (roll_ < -360) roll_ += 360;

    model_matrix_.SetUnitMatrix();
    model_matrix_.RotateAboutY(yaw_);
    model_matrix_.RotateAboutX(pitch_);
    model_matrix_.RotateAboutZ(roll_);
}

void KDron::Initialize() {
    const ColorVertex kVertices[12] =
    {
        { { -1.0f, -1.0f,  1.0f, 1.0f }, { 0, 0, 1, 1 } },  //0
        { { 0.0f,  -1.0f,  1.0f, 1.0f }, { 0, 1, 1, 1 } },  //1
        { {  1.0f, -1.0f,  1.0f, 1.0f }, { 0, 0, 1, 1 } },  //2
        { {  1.0f, -1.0f, -1.0f, 1.0f }, { 0, 1, 0, 1 } },  //3
        { {  1.0f, 0.0f,  -1.0f, 1.0f }, { 0, 0, 1, 1 } },  //4
        { {  1.0f, 0.0f, 0.0f, 1.0f }, { 0, 1, 0, 1 } },    //5
        { {  0.0f,  1.0f, -1.0f, 1.0f }, { 0, 0, 1, 1 } },  //6
        { {  -1.0, 0.0f, -1.0f, 1.0f }, { 0, 1, 0, 1 } },   //7
        { { -1.0f, -1.0f, -1.0f, 1.0f }, { 0, 0, 1, 1 } },  //8
        { { -1.0f, 0.0f, 0.0f, 1.0f }, { 0, 1, 0, 1 } },    //9
        { { -1.0f,  0.0f, 1.0f, 1.0f }, { 0, 0, 1, 1 } },   //10
        { { 1.0f,  0.0f, 1.0f, 1.0f }, { 0, 1, 0, 1 } }     //11
    };

    const GLuint kIndices[60] =
    {
      0,1,10,  1,2,11,
      2,3,5,   3,4,5,
      2,11,5,  4,6,5,
      6,7,9,   6,9,5,
      9,5,1,   1,9,10,
      1,5,11,  7,8,9,
      8,9,0,   0,9,10,
      4,6,7,   3,8,7,
      3,4,7,   0,2,8,
      2,3,8,
    };


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(kVertices[0]), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(kVertices[0]),
        (GLvoid*)sizeof(kVertices[0].position));
    glEnableVertexAttribArray(1);


    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void KDron::Draw(const ModelProgram& program) const {

    glUseProgram(program);
    glBindVertexArray(vao_);

    program.SetModelMatrix(model_matrix_);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);


}
