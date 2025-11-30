#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "primitives/Triangle.h"
#include "primitives/Shape.h"
#include "primitives/FilledStar.h"
#include "primitives/Star.h"
#include "primitives/FilledCircle.h"
#include "primitives/Circle.h"
#include "primitives/StarFrame.h"
#include "primitives/FancyStar.h"
#include "utility/Color.h"
#include "program.h"

class Window{
 public:
    Window(const char*, int, int);
    void Initialize(int major_gl_version, int minor_gl_version);
    void Resize(int new_width, int new_height);
    void KeyEvent(int key, int scancode, int action, int mods);
    void Run(void);
    operator GLFWwindow*(){return window_;}
 private:
    int width_;
    int height_;
    const char* title_;
    GLFWwindow* window_;

    const Color color = { 255,0,255 }; //(3+2+2+1+7)mod10 = 5

    uint32_t current_shape_idx = 0;
    std::vector<Shape*> shapes;

    Program program_;

    void InitGlfwOrDie(int major_gl_version,
                       int minor_gl_version);
    void InitGlewOrDie();
    void InitModels();
    void InitPrograms();
};


#endif // WINDOW_H

