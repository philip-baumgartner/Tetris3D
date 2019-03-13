#include "color.h"
#include <assert.h>

Color::Color() {
	this->R = 0;
	this->G = 0;
	this->B = 0;
}

Color::Color(float r, float g, float b) {
	this->R = r;
	this->G = g;
	this->B = b;
}

Color Color::operator *(const Color &c) const {
	Color result(0, 0, 0);
	result.R = this->R * c.R;
	result.G = this->G * c.G;
	result.B = this->B * c.B;
	return result;
}

Color Color::operator *(const float factor) const {
	Color result(0, 0, 0);
	result.R = this->R * factor;
	result.G = this->G * factor;
	result.B = this->B * factor;
	return result;
}

Color Color::operator +(const Color &c) const {
	Color result(0, 0, 0);
	result.R = this->R + c.R;
	if (result.R > 1)
		result.R = 1;
	result.G = this->G + c.G;
	if (result.G > 1)
		result.G = 1;
	result.B = this->B + c.B;
	if (result.B > 1)
		result.B = 1;
	return result;
}

Color &Color::operator +=(const Color &c) {
	this->R = this->R + c.R;
	if (R > 1)
		R = 1;
	this->G = this->G + c.G;
	if (G > 1)
		G = 1;
	this->B = this->B + c.B;
	if (B > 1)
		B = 1;
	return *this;
}