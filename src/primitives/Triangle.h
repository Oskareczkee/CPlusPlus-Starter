#pragma once

#include <GL/glew.h>

#include "Shape.h"
#include "program.h"

class Triangle: public Shape {
public:
	void InitializeVertexArray() override;
	void InitializeColorsArray() override;
};