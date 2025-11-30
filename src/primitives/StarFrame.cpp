#pragma once

#include "StarFrame.h"

const GLfloat StarFrame::center[] = { 0,0,0,1.0 }; //center of the screen, later this can be changed when glm is available

StarFrame::StarFrame(const int& spike_count, const GLfloat& outer_radius, const GLfloat& inner_radius)
{
	/*These will be set to default values if these values are incorrect*/
	if (spike_count >= 3)
		this->__spike_count = spike_count;

	if (outer_radius > 0.0)
		this->__outer_radius = outer_radius;

	if (inner_radius > 0.0)
		this->__inner_radius = inner_radius;


	InitializeVertexArray();
	InitializeColorsArray();
}

void StarFrame::Draw(const Program& program) {
	glUseProgram(program);
	glBindVertexArray(vao_);

	glDrawArrays(GL_LINE_STRIP, 0, this->VertexCount());

	glBindVertexArray(0);
	glUseProgram(0);
}

void StarFrame::InitializeVertexArray()
{
	//this is pretty much init from filled star, but without center vertex set

	//outside
	for (int x = 0; x <= this->__spike_count * 2; x++) {
		float angle = x * M_PI / this->__spike_count - M_PI / 2; // -> -M_PI/2 -> rotate by 90 degrees by default, so the star won't be rotated

		//create one point with outer radius and one with inner radius
		float radius = (x % 2 == 0) ? this->__outer_radius : this->__inner_radius;

		vertices.emplace_back(radius * cos(angle));
		vertices.emplace_back(radius * sin(angle));
		vertices.emplace_back(0.0); //do not set z-axis for now
		vertices.emplace_back(1.0);
	}

	//connect each vertex to the center, thus creating frame
	for (int x = 0; x <= this->__spike_count * 2; x++) {
		float angle = x * M_PI / this->__spike_count - M_PI / 2; // -> -M_PI/2 -> rotate by 90 degrees by default, so the star won't be rotated

		//create one point with outer radius and one with inner radius
		float radius = (x % 2 == 0) ? this->__outer_radius : this->__inner_radius;

		vertices.emplace_back(radius * cos(angle));
		vertices.emplace_back(radius * sin(angle));
		vertices.emplace_back(0.0); //do not set z-axis for now
		vertices.emplace_back(1.0);

		//connect this vertex with center
		vertices.emplace_back(center[0]);
		vertices.emplace_back(center[1]);
		vertices.emplace_back(center[2]);
		vertices.emplace_back(center[3]);
	}
}

void StarFrame::InitializeColorsArray() {
	if (this->vertices.empty())
		throw std::exception("FilledStar : Colors array must be initialized after Vertex Array");

	//we create linear gradient over whole star by default
	for (int x = 0; x < this->VertexCount(); x++) {
		this->colors.emplace_back((x % 3) == 0 ? 1.0 : 0.0);
		this->colors.emplace_back((x % 3) == 1 ? 1.0 : 0.0);
		this->colors.emplace_back((x % 3) == 2 ? 1.0 : 0.0);
		this->colors.emplace_back(1.0);
	}
}
