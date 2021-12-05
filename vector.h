/*!
@file       vector.h
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@course     CSD 1400
@section    C
@date       28/11/2021
@brief    	Contains function declaration addition, subtraction,
			magnitude, magnitude_sq, normalise, dot, rotation and
			projection. Marco definition of PI.
*//*__________________________________________________________________________*/
#ifndef _VECTOR_H
#define _VECTOR_H

#define PI 3.14159265358979323846

typedef struct Vector2
{
	float x;
	float y;
} Vector2;


/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Vector addition
@param word - Vector2 v1, Vector2 v2
@return Vector2
*//*_____________________________________________________________*/
Vector2 addition(Vector2 v1, Vector2 v2);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Vector subtraction
@param word -
@return
*//*_____________________________________________________________*/
Vector2 subtraction(Vector2 v1, Vector2 v2);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief	finds magnitude of vector
@param word - Vector2 v1
@return float
*//*_____________________________________________________________*/
float magnitude(Vector2 v1);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		magnitude sq of a vector
@param word - Vector2 v1
@return float
*//*_____________________________________________________________*/
float magnitude_sq(Vector2 v1);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Normalise vector
@param word - Vector2 v1
@return Vector2
*//*_____________________________________________________________*/
Vector2 normalise(Vector2 v1);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Dot product of vector
@param word - Vector2 v1, Vector2 v2
@return float
*//*_____________________________________________________________*/
float dot(Vector2 v1, Vector2 v2);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Rotation about origin 
@param word - Vector2 v1, double angle
@return Vector2
*//*_____________________________________________________________*/
Vector2 rotation(Vector2 v1, double angle);

/*!
@brief projection of vector v onto flr
@param word - Vector2 flr, Vector2 v 
@return Vector2
*//*_____________________________________________________________*/
Vector2 projection(Vector2 flr, Vector2 v);

#endif