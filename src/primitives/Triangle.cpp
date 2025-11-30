#pragma once
#include "Triangle.h"

void Triangle::InitializeVertexArray(){
    this->vertices = std::vector<GLfloat>{
        -0.8f, -0.8f, 0.0f, 1.0f,
         0.0f,  0.8f, 0.0f, 1.0f,
         0.8f, -0.8f, 0.0f, 1.0f
    };
}

void Triangle::InitializeColorsArray() {
    this->colors = std::vector<GLfloat>{
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };
}