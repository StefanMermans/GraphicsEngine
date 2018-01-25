#include "Vec.h"

Vec3f::Vec3f()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3f::Vec3f(const Vec3f & other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

Vec3f::Vec3f(const float & x, const float & y, const float & z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3f & Vec3f::operator=(const Vec3f & other)
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
}

Vec4f::Vec4f()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Vec4f::Vec4f(const Vec4f & other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

Vec4f::Vec4f(const float & x, const float & y, const float & z, const float & w)
{

	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vec4f & Vec4f::operator=(const Vec3f & other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	y = other.y;

	return *this;
}
