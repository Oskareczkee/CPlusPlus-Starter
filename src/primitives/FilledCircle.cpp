#pragma once

#include "FilledCircle.h"


const GLfloat FilledCircle::center[] = { 0,0,0,1.0 }; //center of the screen, later this can be changed when glm is available

FilledCircle::FilledCircle(const GLfloat& radius, const int& n_gon_count)
{
	if (radius > 0.0)
		this->__radius = radius;

	if (n_gon_count >= 3)
		this->__n_gon_count = n_gon_count;

	InitializeVertexArray();
	InitializeColorsArray();
}

void FilledCircle::InitializeVertexArray()
{
	//set center point
	vertices.emplace_back(center[0]);
	vertices.emplace_back(center[1]);
	vertices.emplace_back(center[2]);
	vertices.emplace_back(center[3]);

	for (int x = 0; x <= this->__n_gon_count; x++) {
		float angle = x *2*M_PI / this->__n_gon_count + M_PI_2; //+ M_PI_2 makes this usefull to draw filled polygons

		vertices.emplace_back(this->__radius * cos(angle) + center[0]);
		vertices.emplace_back(this->__radius * sin(angle) + center[1]);
		vertices.emplace_back(0.0); //do not set z-axis for now
		vertices.emplace_back(1.0);
	}
}

void FilledCircle::InitializeColorsArray()
{
	if (this->vertices.empty())
		throw std::exception("FilledCircle : Colors array must be initialized after Vertex Array");

	//we create linear gradient over whole circle by default
	for (int x = 0; x < this->VertexCount(); x++) {
		this->colors.emplace_back((x % 3) == 0 ? 1.0 : 0.0);
		this->colors.emplace_back((x % 3) == 1 ? 1.0 : 0.0);
		this->colors.emplace_back((x % 3) == 2 ? 1.0 : 0.0);
		this->colors.emplace_back(1.0);
	}
}

void FilledCircle::Draw(const Program& program)
{
	glUseProgram(program);
	glBindVertexArray(vao_);

	glDrawArrays(GL_TRIANGLE_FAN, 0, this->VertexCount());

	glBindVertexArray(0);
	glUseProgram(0);
}


