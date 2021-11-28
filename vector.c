/*!
@file       vector.c
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@course     CSD 1400
@section    C
@date       28/11/2021
@brief    	Contains function definition addition, subtraction,
			magnitude, magnitude_sq, normalise, dot, rotation
			projection
*//*__________________________________________________________________________*/
#include "Vector.h"
#include <Math.h>

Vector2 addition(Vector2 v1, Vector2 v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	return v1;
}

Vector2 subtraction(Vector2 v1, Vector2 v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	return v1;
}

float magnitude(Vector2 v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}

float magnitude_sq(Vector2 v)
{
	return v.x * v.x + v.y * v.y;
}

Vector2 normalise(Vector2 v)
{
	Vector2 u;
	float mag = magnitude(v);
	u.x = v.x / mag;
	u.y = v.y / mag;
	return u;
}

float dot(Vector2 v1, Vector2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

Vector2 rotation(Vector2 v, double angle)
{
	Vector2 tmp;
	//convert to radian, rotate about (0,0)
	angle *= PI / 180;
	tmp.x = (float)(cos(angle) * v.x + -sin(angle) * v.y);
	tmp.y = (float)(sin(angle) * v.x + cos(angle) * v.y);
	return tmp;
}

Vector2 projection(Vector2 flr, Vector2 v)
{
	//is this correct proj v onto flr(floor)
	Vector2 tmp;
	tmp.x = (dot(flr, v) / dot(flr, flr)) * flr.x;
	tmp.y = (dot(flr, v) / dot(flr, flr)) * flr.y;
	return tmp;
}

