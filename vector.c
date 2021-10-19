#include "Vector.h"
#include <Math.h>

Vector2 addition(Vector2 v1, Vector2 v2)
{
	v1.pos_x += v2.pos_x;
	v1.pos_y += v2.pos_y;
	return v1;
}

Vector2 subtraction(Vector2 v1, Vector2 v2)
{
	v1.pos_x -= v2.pos_x;
	v1.pos_y -= v2.pos_y;
	return v1;
}

float magnitude(Vector2 v)
{
	return sqrtf(v.pos_x * v.pos_x + v.pos_y * v.pos_y);
}

float magnitude_sq(Vector2 v)
{
	return v.pos_x * v.pos_x + v.pos_y * v.pos_y;
}

Vector2 normalise(Vector2 v)
{
	Vector2 u;
	float mag = magnitude(v);
	u.pos_x = v.pos_x / mag;
	u.pos_y = v.pos_y / mag;
	return u;
}

float dot(Vector2 v1, Vector2 v2)
{
	return v1.pos_x * v2.pos_x + v1.pos_y * v2.pos_y;
}

Vector2 rotation(Vector2 v, double angle)
{
	Vector2 tmp;
	//convert to radian, rotate about (0,0)
	angle *= PI / 180;
	tmp.pos_x = (float)(cos(angle) * v.pos_x + -sin(angle) * v.pos_y);
	tmp.pos_y = (float)(sin(angle) * v.pos_x + cos(angle) * v.pos_y);
	return tmp;
}

Vector2 projection(Vector2 flr, Vector2 v)
{
	//is this correct proj v onto flr(floor)
	Vector2 tmp;
	tmp.pos_x = (dot(flr, v) / dot(flr, flr)) * flr.pos_x;
	tmp.pos_y = (dot(flr, v) / dot(flr, flr)) * flr.pos_y;
	return tmp;
}

