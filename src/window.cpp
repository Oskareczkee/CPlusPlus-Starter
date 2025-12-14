#include "window.h"

#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glerror.h"

const char* kVertexShader= RESOURCES_PATH "/SimpleShader.vertex.glsl";
const char* kFragmentShader= RESOURCES_PATH "/SimpleShader.fragment.glsl";


Window::Window(const char * title, int width, int height){
    title_ = title;
    width_ = width;
    height_ = height;
    cube_.SetInitAngle(15);
    cube_.SetVelocity(45);

    kdron_.SetInitAngle(15);
    kdron_.SetVelocity(45);

    last_time_ = 0;
}

void Window::Initialize(int major_gl_version, int minor_gl_version){

    InitGlfwOrDie(major_gl_version, minor_gl_version);
    InitGlewOrDie();


    std::cout << "OpenGL initialized: OpenGL version: " << glGetString(GL_VERSION) << " GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    InitModels();
    InitPrograms();

    view_matrix_.Translate(0, 0, initial_z_translation);
    SetViewMatrix();

    projection_matrix_ = Mat4::CreatePerspectiveProjectionMatrix(60, (float)width_/(float)height_, 0.1f, 100.0f);
    SetProjectionMatrix();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void Window::InitGlfwOrDie(int major_gl_version, int minor_gl_version){
    if ( !glfwInit() ) {
        std::cerr << "ERROR: Could not initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_gl_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_gl_version);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
    if (!window_){
        std::cerr << "ERROR: Could not create a new rendering window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window_);
}

void Window::InitGlewOrDie(){
    GLenum glew_init_result;
    glewExperimental = GL_TRUE;
    glew_init_result = glewInit();
    if (GLEW_OK != glew_init_result) {
        if (GLEW_ERROR_NO_GLX_DISPLAY!=glew_init_result || GLFW_PLATFORM_WAYLAND != glfwGetPlatform() ){
            std::cerr << "Glew ERROR: " << glewGetErrorString(glew_init_result) << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    }
#ifdef DEBUG
    if(glDebugMessageCallback){
        std::cout << "Register OpenGL debug callback " << std::endl;
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROC) OpenglCallbackFunction, NULL);
        GLuint unused_ids = 0;
        glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            &unused_ids,
            GL_FALSE);
    }
    else
        std::cout << "glDebugMessageCallback not available" << std::endl;
#endif

}

void Window::InitModels(){
    cube_.Initialize();
    kdron_.Initialize();
}

void Window::InitPrograms(){
    program_.Initialize(kVertexShader, kFragmentShader);
}

void Window::SetViewMatrix() const{
    glUseProgram(program_);
    program_.SetViewMatrix(view_matrix_);
}

void Window::SetProjectionMatrix() const{
    glUseProgram(program_);
    program_.SetProjectionMatrix(projection_matrix_);
}

void Window::ZoomUp()
{
    this->zoom_ += zoom_factor_;

    //do not allow for too big zoom
    if (this->zoom_ > 10.0f) this->zoom_ = 10.0f;
    SetCameraZoom();
}

void Window::ZoomDown()
{
    this->zoom_ -= zoom_factor_;

    //do not allow for too small zoom
    if (this->zoom_ < -10.0f) this->zoom_ = -10.0f;
    SetCameraZoom();
}

void Window::SetCameraZoom()
{
    view_matrix_.SetUnitMatrix();
    view_matrix_.Translate(0, 0, initial_z_translation + zoom_);
    SetViewMatrix();
}

void Window::ChangeToPerspectiveProjection()
{
    if (is_using_perspective_projection_)
        return; //perspective projection is already set

    is_using_perspective_projection_ = !is_using_perspective_projection_;
    projection_matrix_ = Mat4::CreatePerspectiveProjectionMatrix(60, (float)width_ / (float)height_, 0.1f, 100.0f);

    SetProjectionMatrix();
}

void Window::ChangeToOrthoPerspective()
{
    if (!is_using_perspective_projection_)
        return; //ortho projection is already set

    is_using_perspective_projection_ = !is_using_perspective_projection_;

    float aspect = (float)width_ / (float)height_;
    float orto_height = 1;
    float orto_width = orto_height * aspect;

    projection_matrix_ = Mat4::CreateOrthoProjectionMatrix( -orto_width/2, orto_width/2,
                                                            -orto_height/2, orto_height/2,
                                                            0.1f, 100.0f);

    SetProjectionMatrix();
}

void Window::Resize(int new_width, int new_height){
    width_ = new_width;
    height_ = new_height;

    float aspect = (float)width_ / (float)height_;
    float orto_height = 1;
    float orto_width = orto_height * aspect;

    projection_matrix_ = is_using_perspective_projection_ ? 
                        Mat4::CreatePerspectiveProjectionMatrix(60, (float)width_/(float)height_, 0.1f, 100.0f) :
                        Mat4::CreateOrthoProjectionMatrix(-orto_width / 2, orto_width / 2, -orto_height / 2, orto_height / 2, 0.1f, 100.0f);

    SetProjectionMatrix();
    glViewport(0, 0, width_, height_);
}

void Window::KeyEvent(int key, int /*scancode*/, int action, int /*mods*/){
    if(action == GLFW_PRESS){
        switch (key){
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window_, GLFW_TRUE);
            break;
            case GLFW_KEY_LEFT:
                this->show_cube_ ? cube_.Rotate(0, 10, 0) : kdron_.Rotate(0, 10, 0);
            break;
            case GLFW_KEY_RIGHT:
                this->show_cube_ ? cube_.Rotate(0, -10, 0) : kdron_.Rotate(0, -10, 0);
            break;
            case GLFW_KEY_UP:
                this->show_cube_ ? cube_.Rotate(-10,0,0) : kdron_.Rotate(-10, 0, 0);
                break;
            case GLFW_KEY_DOWN:
                this->show_cube_ ? cube_.Rotate(10, 0, 0) : kdron_.Rotate(10, 0, 0);
                break;
            case GLFW_KEY_SPACE:
                this->show_cube_ ? cube_.ToggleAnimated() : kdron_.ToggleAnimated();
                break;
            case GLFW_KEY_PAGE_UP:
                this->ZoomUp();
                break;
            case GLFW_KEY_PAGE_DOWN:
                this->ZoomDown();
                break;
            case GLFW_KEY_HOME:
                ChangeToPerspectiveProjection();
                break;
            case GLFW_KEY_END:
                ChangeToOrthoPerspective();
                break;
            break;

            //switch from kdron to cube and vice versa
            case GLFW_KEY_C:
                this->show_cube_ = !this->show_cube_;
                break;
            default:
                break;
        }
    }
    else if(action == GLFW_REPEAT){
        switch (key){
        case GLFW_KEY_LEFT:
            this->show_cube_ ? cube_.Rotate(0, 10, 0) : kdron_.Rotate(0, 10, 0);
            break;
        case GLFW_KEY_RIGHT:
            this->show_cube_ ? cube_.Rotate(0, -10, 0) : kdron_.SlowDown();
            break;
        case GLFW_KEY_UP:
            this->show_cube_ ? cube_.Rotate(-10, 0, 0) : kdron_.SlowDown();
            break;
        case GLFW_KEY_DOWN:
            this->show_cube_ ? cube_.Rotate(10, 0, 0) : kdron_.SlowDown();
            break;
        case GLFW_KEY_PAGE_UP:
            this->ZoomUp();
            break;
        case GLFW_KEY_PAGE_DOWN:
            this->ZoomDown();
            break;
        default:
            break;
        }
    }
}

void Window::Run(void){
    while (!glfwWindowShouldClose(window_)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        clock_t now = clock();
        if (last_time_ == 0) last_time_ = now;
        this->show_cube_ ? 
                        cube_.Move((float)(now - last_time_) / CLOCKS_PER_SEC):
                        kdron_.Move((float)(now - last_time_) / CLOCKS_PER_SEC);
        last_time_ = now;

        this->show_cube_ ? cube_.Draw(program_) : kdron_.Draw(program_);
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

}


/*
OLD CODE FOR COPYING PURPOSES
void Window::KeyEvent(int key, int, int action, int){
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
            break;
        case GLFW_KEY_LEFT:
            this->show_cube_ ? cube_.SlowDown() : kdron_.SlowDown();
            break;
        case GLFW_KEY_RIGHT:
            this->show_cube_ ? cube_.SpeedUp() : kdron_.SpeedUp();
            break;
        case GLFW_KEY_SPACE:
            this->show_cube_ ? cube_.ToggleAnimated() : kdron_.ToggleAnimated();
            break;

            //switch from kdron to cube and vice versa
        case GLFW_KEY_C:
            this->show_cube_ = !this->show_cube_;
            break;
        default:
            break;
        }
    }
    else if (action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_LEFT:
            this->show_cube_ ? cube_.SlowDown() : kdron_.SlowDown();
            break;
        case GLFW_KEY_RIGHT:
            this->show_cube_ ? cube_.SpeedUp() : kdron_.SpeedUp();
            break;
        default:
            break;
        }
    }
}

void Window::Run(void) {
    while (!glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        clock_t now = clock();
        if (last_time_ == 0) last_time_ = now;
        this->show_cube_ ?
            cube_.Move((float)(now - last_time_) / CLOCKS_PER_SEC) :
            kdron_.Move((float)(now - last_time_) / CLOCKS_PER_SEC);
        last_time_ = now;

        this->show_cube_ ? cube_.Draw(program_) : kdron_.Draw(program_);
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

}

*/
