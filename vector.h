#ifndef _VECTOR_H
#define _VECTOR_H

#define PI 3.14159265358979323846

//vector stuff add more stuff if needed

typedef struct Vector2
{
	float pos_x;
	float pos_y;
} Vector2;

float magnitude(Vector2 v1);

Vector2 normalise(Vector2 v1);

float dot(Vector2 v1, Vector2 v2);

Vector2 rotation(Vector2 v1, double angle);

Vector2 projection(Vector2 flr, Vector2 v);

#endif