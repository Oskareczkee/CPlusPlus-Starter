#pragma once

#define _USE_MATH_DEFINES /*M_PI*/

#include <GL/glew.h>
#include "Shape.h"
#include "program.h"

class StarFrame : public Shape {

public:
	StarFrame(const int& spike_count, const GLfloat& outer_radius, const GLfloat& inner_radius);
	void Draw(const Program& program) override;

protected:
	void InitializeVertexArray() override;
	void InitializeColorsArray() override;
private:
	int __spike_count = 5;

	/*normalized radius*/
	GLfloat __outer_radius = 0.3;
	GLfloat __inner_radius = 0.6;

	const static GLfloat center[]; //this can be changed later as variable, when glm is available
};