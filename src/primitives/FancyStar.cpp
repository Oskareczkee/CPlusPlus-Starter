#include "FancyStar.h"

FancyStar::FancyStar(const int& spike_count, const GLfloat& outer_radius, const GLfloat& inner_radius)
{
	/*These will be set to default values if these values are incorrect*/
	if (spike_count >= 3)
		this->__spike_count = spike_count;

	if (outer_radius > 0.0)
		this->__outer_radius = outer_radius;

	if (inner_radius > 0.0)
		this->__inner_radius = inner_radius;
}

void FancyStar::Draw(const Program& program)
{
	FilledStar* filled_star = new FilledStar(__spike_count, __outer_radius, __inner_radius);
	filled_star->SetFillColorRGBA(fill_color);
	filled_star->Initialize();

	StarFrame* star_frame = new StarFrame(__spike_count, __outer_radius, __inner_radius);
	star_frame->SetFillColorRGBA(frame_color);
	star_frame->Initialize();

	filled_star->Draw(program);
	star_frame->Draw(program);

	delete filled_star;
	delete star_frame;
}

FancyStar* FancyStar::SetFillColor(const Color& color)
{
	fill_color = color;
	return this;
}

FancyStar* FancyStar::SetFrameColor(const Color& color)
{
	frame_color = color;
	return this;
}
