cmake_minimum_required(VERSION 3.30)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>") #we want to use the static runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Release>:Release>")

# The toolchain file setup should ideally be in a CMakePresets.json file,
# but we'll keep it here for simplicity
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(GamedevPlayground VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ADD DEPENDECIES
find_package(SDL3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(PNG CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# No C++20 modules
set(CMAKE_CXX_SCAN_FOR_MODULES OFF) 

# Playground executable
add_executable(Playground)


# Playground sources and headers
file(GLOB_RECURSE Playground_SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)

#Added automatically by vsc
target_sources(Playground PRIVATE ${Playground_SOURCES} "src/primitives/Shape.h" "src/primitives/Shape.cpp" "src/primitives/Triangle.h" "src/primitives/Triangle.cpp" "src/primitives/FilledStar.h" "src/primitives/FilledStar.cpp" "src/primitives/Star.h" "src/primitives/Star.cpp" "src/primitives/FilledCircle.h" "src/primitives/FilledCircle.cpp" "src/primitives/Circle.h" "src/primitives/Circle.cpp" "src/primitives/StarFrame.h" "src/primitives/StarFrame.cpp" "src/primitives/FancyStar.h" "src/utility/Color.h")

# Playground include dirs
target_include_directories(Playground PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# CUSTOM PREPROCESSOR DEFINES
target_compile_definitions(Playground PRIVATE
    RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources"
    MY_TEST_DEFINE
)

# Add directories with any custom libraries here -> add it to target_link_libraries as well
target_link_directories(Playground PRIVATE
  #"${CMAKE_CURRENT_SOURCE_DIR}/mylib/"
)

# Automatically links to these libs
target_link_libraries(Playground PRIVATE
    SDL3::SDL3
    GLEW::GLEW
    glfw
    PNG::PNG
)


# Set configuration properties for Playground
if(MSVC) #Windows -> Visual Studio, Microsoft compiler
    target_compile_options(Playground PRIVATE
        /W3
        /MDd # multithreaded build
        /ZI # program database for edit and continue
    )
else() #Anything else (GCC)
    target_compile_options(Playground PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic
    )
endif()

# Set Playground as the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Playground)

# Filters
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${Playground_SOURCES})

# Enable solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
