#pragma once
#include <vector>
#include <GL/glew.h>
#include "../program.h"
#include "utility/Color.h"

class Shape {
public:
    virtual void Initialize();
    virtual void Draw(const Program& program);

    /*All setters should return reference to the object, this makes object creation much easier*/
    Shape* SetFillColorRGB(const GLfloat& r, const GLfloat& g, const GLfloat& b);
    Shape* SetFillColorRGB(const Color& color);
    Shape* SetFillColorRGBA(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a);
    Shape* SetFillColorRGBA(const Color& color);

    int VertexCount();

    Shape();
    virtual ~Shape();
protected:
    virtual void InitializeVertexArray();
    virtual void InitializeColorsArray();

    GLuint vao_;
    GLuint vertex_buffer_;
    GLuint color_buffer_;

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
};