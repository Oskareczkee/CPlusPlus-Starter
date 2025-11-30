#pragma once

#pragma once

#define _USE_MATH_DEFINES /*M_PI*/

#include <GL/glew.h>
#include <math.h>
#include "Shape.h"
#include "program.h"

class FilledCircle : public Shape {

public:
	FilledCircle(const GLfloat& radius, const int& n_gon_count = 30);
	void Draw(const Program& program) override;

protected:
	void InitializeVertexArray() override;
	void InitializeColorsArray() override;

private:
	/*normalized radius*/
	GLfloat __radius = 0.3;
	GLuint __n_gon_count = 30;
	const static GLfloat center[]; //this can be changed later as variable, when glm is available
};