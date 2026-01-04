#include "window.h"

#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glerror.h"

const char* kVertexShader=RESOURCES_PATH"/TextureShader.vertex.glsl";
const char* kFragmentShader=RESOURCES_PATH"/TextureShader.fragment.glsl";

const char* kIceTexureFile=RESOURCES_PATH"/ice.tga";
const char* kColorTexureFile=RESOURCES_PATH"/texture.tga";

//3+2+2+1+7=15, 15mod9 = 6
const char* kPlanetUranusTextureFile = RESOURCES_PATH"/uranusmap.tga";
const char* kMoonUmbrielTextureFile = RESOURCES_PATH"/umbriel.tga";
const char* kSpaceTextureFile = RESOURCES_PATH"/space.tga";

const int kPlaneM = 30;
const int kPlaneN = 40;

const int kTorusM = 20;
const int kTorusN = 30;
const float kTorusR = 2;
const float kTorus_r = 0.75;

const int kUranusM = 20;
const int kUranusN = 30;
const float kUranusR = 2;

const int kUmbrielM = 20;
const int kUmbrielN = 30;
const float kUmbrielR = .25;
const float kUmbrielOffset = 2.5;
float UmbrielAngle = 0;

//10 deg/sec around planet
const float kUmbrielSpeed = 2*M_PI/36;

Window::Window(const char * title, int width, int height){
    title_ = title;
    width_ = width;
    height_ = height;
    last_time_ = 0;
}

void Window::Initialize(int major_gl_version, int minor_gl_version){

    InitGlfwOrDie(major_gl_version, minor_gl_version);
    InitGlewOrDie();


    std::cout << "OpenGL initialized: OpenGL version: " << glGetString(GL_VERSION) << " GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    InitTextures();
    InitModels();
    InitPrograms();

    view_matrix_.Translate(0, -1, -8);
    SetViewMatrix();

    projection_matrix_ = Mat4::CreatePerspectiveProjectionMatrix(60, (float)width_/(float)height_, 0.1f, 100.0f);
    SetProjectionMatrix();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.25f, 0.75f, 0.35f, 0.0f);

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
    glfwWindowHint(GLFW_SAMPLES, 8);
    glEnable(GL_MULTISAMPLE);
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

void Window::InitTextures(){
    color_texture_.Initialize(kColorTexureFile);
    ice_texture_.Initialize(kIceTexureFile);
    uranus_texture_.Initialize(kPlanetUranusTextureFile);
    moon_texture_.Initialize(kMoonUmbrielTextureFile);
    space_texture_.Initialize(kSpaceTextureFile);
}

void Window::InitModels(){
    plane_.Initialize(kPlaneM, kPlaneN);
    plane_.SetTexture(space_texture_);
    plane_.SetTextureUnit(GL_TEXTURE0);

    torus_.Initialize(kTorusM, kTorusN, kTorusR, kTorus_r);
    torus_.SetTexture(color_texture_);
    torus_.SetTextureUnit(GL_TEXTURE0);

    planet_uranus_.Initialize(kUranusN, kUranusN, kUranusR);
    planet_uranus_.SetTexture(uranus_texture_);
    planet_uranus_.SetTextureUnit(GL_TEXTURE0);

    moon_umbriel_.Initialize(kUmbrielN, kUmbrielN, kUmbrielR);
    moon_umbriel_.SetTexture(moon_texture_);
    moon_umbriel_.SetTextureUnit(GL_TEXTURE0);
}

void Window::InitPrograms(){
    program_.Initialize(kVertexShader, kFragmentShader);
    program_.SetTextureUnit(0);
}

void Window::SetViewMatrix() const{
    glUseProgram(program_);
    program_.SetViewMatrix(view_matrix_);
}

void Window::SetProjectionMatrix() const{
    glUseProgram(program_);
    program_.SetProjectionMatrix(projection_matrix_);
}


void Window::Resize(int new_width, int new_height){
    width_ = new_width;
    height_ = new_height;
    projection_matrix_ = Mat4::CreatePerspectiveProjectionMatrix(60, (float)width_/(float)height_, 0.1f, 100.0f);
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
                planet_uranus_.SlowDown();
                moon_umbriel_.SlowDown();
                //torus_.SlowDown();
            break;
            case GLFW_KEY_RIGHT:
                planet_uranus_.SpeedUp();
                moon_umbriel_.SpeedUp();
                //torus_.SpeedUp();
            break;
            case GLFW_KEY_SPACE:
                planet_uranus_.ToggleAnimated();
                moon_umbriel_.ToggleAnimated();
                //torus_.ToggleAnimated();
            break;
            default:
            break;
        }
    }
    else if(action == GLFW_REPEAT){
        switch (key){
            case GLFW_KEY_LEFT:
                planet_uranus_.SlowDown();
                moon_umbriel_.SlowDown();
                //torus_.SlowDown();
            break;
            case GLFW_KEY_RIGHT:
                planet_uranus_.SpeedUp();
                moon_umbriel_.SpeedUp();
                //torus_.SpeedUp();
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

        float delta_t =(float)(now - last_time_) / CLOCKS_PER_SEC;

        //make ready for transforms
        planet_uranus_.MakeReadyForTransforms();
        moon_umbriel_.MakeReadyForTransforms();

        //move all planets - rotations before translation, otherwise we will get wrong result
        //torus_.Move((float)(now - last_time_) / CLOCKS_PER_SEC);
        planet_uranus_.Move(delta_t);
        moon_umbriel_.Move(delta_t);

        //transform moon

        UmbrielAngle += kUmbrielSpeed * delta_t;
        if (UmbrielAngle >= 2 * M_PI) UmbrielAngle -= 2 * M_PI;

        //rotate around x-z axis with offset
        moon_umbriel_.Translate(kUmbrielOffset * cos(UmbrielAngle),
                                0,
                                -kUmbrielOffset * sin(UmbrielAngle));


        last_time_ = now;

        planet_uranus_.Draw(program_);
        moon_umbriel_.Draw(program_);
        //torus_.Draw(program_);
        plane_.Draw(program_);

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

}
