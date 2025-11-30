#pragma once

#define _USE_MATH_DEFINES /*M_PI*/

#include <GL/glew.h>
#include "Shape.h"
#include "StarFrame.h"
#include "FilledStar.h"
#include "program.h"
#include "utility/Color.h"

class FancyStar : public Shape {

public:
	FancyStar(const int& spike_count, const GLfloat& outer_radius, const GLfloat& inner_radius);
	void Draw(const Program& program) override;

	FancyStar* SetFillColor(const Color& color);
	FancyStar* SetFrameColor(const Color& color);

	Shape* SetFillColorRGB(const Color& color)															= delete;
	Shape* SetFillColorRGB(const GLfloat& r, const GLfloat& g, const GLfloat& b)						= delete;
	Shape* SetFillColorRGBA(const Color& color)															= delete;
	Shape* SetFillColorRGBA(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a)		= delete;

protected:
private:
	int __spike_count = 5;

	/*normalized radius*/
	GLfloat __outer_radius = 0.3;
	GLfloat __inner_radius = 0.6;

	Color frame_color = { 255,255,255,1 };
	Color fill_color = { 0,255,255,1 };
};