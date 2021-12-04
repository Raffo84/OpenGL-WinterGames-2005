#include "Color.h"
#include "GL/glut.h"
#include <iostream>

Color::Color(void)
{
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
}

Color::Color(Color* rhs)
{
	r = rhs->r;
	g = rhs->g;
	b = rhs->b;
}

Color::Color(float newR, float newG, float newB)
{
	r = newR;
	g = newG;
	b = newB;
	// std::cout << newR << " " << newG << " " << newB << std::endl;
}

Color::~Color(void)
{
}

void Color::switchToColor() const
{
	glColor3f (r, g, b);
}

bool Color::operator==(Color rhs) {
	return (r == rhs.r && g == rhs.g && b == rhs.b) ? (true) : (false);
}