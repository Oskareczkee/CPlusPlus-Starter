#pragma once

#include "Shape.h"

void Shape::InitializeColorsArray() {}
void Shape::InitializeVertexArray() {}

void Shape::Initialize() {

    //initialize colors and vertices if for some reason they have not been initialized
    if(this->vertices.empty())
        InitializeVertexArray();

    if(this->colors.empty())
        InitializeColorsArray();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(GLfloat), this->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &color_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(GLfloat), this->colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Shape::Draw(const Program& program) {
    glUseProgram(program);
    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glUseProgram(0);
}

Shape* Shape::SetFillColorRGB(const GLfloat& r, const GLfloat& g, const GLfloat& b)
{
    if (r < 0 || g < 0 || b < 0 || r>255 || g>255 || b>255)
        throw std::exception("Shape : FillColorRGB -> incorrect values");

    this->colors.clear(); //clear all colors

    //normalize r,g,b values
    GLfloat r_norm = r / 255.0;
    GLfloat g_norm = g / 255.0;
    GLfloat b_norm = b / 255.0;


    for (int x = 0; x < this->VertexCount(); x++)
    {
        this->colors.emplace_back(r_norm);
        this->colors.emplace_back(g_norm);
        this->colors.emplace_back(b_norm);
        this->colors.emplace_back(1.0);
    }

    return this; //return reference
}

Shape* Shape::SetFillColorRGB(const Color& color)
{
    return this->SetFillColorRGB(color.r, color.g, color.b);
}

Shape* Shape::SetFillColorRGBA(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a)
{
    if (r < 0 || g < 0 || b < 0 || r>255 || g>255 || b>255 || a < 0 || a > 1.0)
        throw std::exception("Shape : FillColorRGBA -> incorrect values");

    this->colors.clear(); //clear all colors

    //normalize r,g,b values
    GLfloat r_norm = r / 255.0;
    GLfloat g_norm = g / 255.0;
    GLfloat b_norm = b / 255.0;

    for (int x = 0; x < this->VertexCount(); x++)
    {
        this->colors.emplace_back(r_norm);
        this->colors.emplace_back(g_norm);
        this->colors.emplace_back(b_norm);
        this->colors.emplace_back(a);
    }

    return this; //return reference
}

Shape* Shape::SetFillColorRGBA(const Color& color) 
{
    return this->SetFillColorRGBA(color.r, color.g, color.b, color.a);
}

int Shape::VertexCount()
{
    //divde over 4 -> 1 vertex => 4 values
    return this->vertices.size() / 4;
}

Shape::Shape()
{
    InitializeVertexArray();
    InitializeColorsArray();
}

Shape::~Shape() {
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &this->color_buffer_);
    glDeleteBuffers(1, &this->vertex_buffer_);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &this->vao_);
}