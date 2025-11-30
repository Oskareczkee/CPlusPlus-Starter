#pragma once

#include "FilledCircle.h"

class Circle : public Shape {

public:
	Circle(const GLfloat& radius, const int& n_gon_count = 30);
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