/*!
@file       Samuel.h
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)\
@co-author	
@course     CSD 1400
@section    C
@date       28/11/2021
@brief    	Contains function declaration remove_turret, render_turret, 
			update_turret, shoot, update_projectile, render_projectile,
			update_turretAnimation, create_particle, update_particle and
			render_particle. Contains ParticleType enum and struct Particle.
*//*__________________________________________________________________________*/
#ifndef GOSTGONEWRONG_CURRENTHEADERFILES_SAMUEL_H
#define GOSTGONEWRONG_CURRENTHEADERFILES_SAMUEL_H

#include "vector.h"
#include "game.h"

//change value as need
#define PROJ_OFFSET game.grid_height * 0.4
#define SLOW_RANGE game.grid_width * 2
#define EXPLOSION_RANGE game.grid_width * 1.5
#define HOMING_RANGE game.grid_width * 3


/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void remove_turret(int index_x, int index_y);

/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void render_turret(void);

/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void update_turret(void);

// Pass in the position to shoot from, type is an enum so can pass it turrettype if want or int, direction to shoot in
/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void shoot(float x, float y, Modifiers mod, ProjectileType type, Vector2 dir);

//updates projectiles
/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void update_projectile(void);

//render projectiles
/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void render_projectile(void);

//update animation
/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void update_turretAnimation(Turret* t);

//particle test
typedef enum ParticleType
{
	kParBasic,
	kParSlow,
	kParHoming,
	kParMax
}ParticleType;

typedef struct Particle
{
	Vector2 dir;
	Vector2 pos;
	float size;
	float timer;
	float duration;
	int alpha;
	int is_active;
	ParticleType type;
} Particle;


Particle particles[250];

/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void create_particle(Vector2 pos, Vector2 dir, float size, float duration, ParticleType type);

/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void update_particle();

/*!
@brief This function
@param word -
@return
*//*_____________________________________________________________*/
void render_particle();

#endif