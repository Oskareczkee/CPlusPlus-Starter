#pragma once
#include <GLFW/glfw3.h>

struct Color { 
	GLfloat r=0, g=0, b=0, a=0; 

	Color SetSaturation(const float& saturation) const
	{
		Color new_color;
		float lum = 0.299f * this->r + 0.587f * this->g + 0.114f * this->b;
		new_color.r = this->r * saturation + lum * (1.0f - saturation);
		new_color.g = this->g * saturation + lum * (1.0f - saturation);
		new_color.b = this->b * saturation + lum * (1.0f - saturation);

		return new_color;
	}

};