/*!
All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Samuel.h
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-authors	Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
			Ng Zheng Wei (zhengwei.ng@digipen.edu)
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

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Macros definitions 
			PROJ_OFFSET		- used to offset spawn position of a 
							projectile for Slow and Homing turrets
			SLOW_RANGE		- range of slow area of effect
			EXPLOSION_RANGE - range of exlposion area of effect
			HOMING_RANGE	- range for a homing projectile to 
							retrack a new enemy
*//*_____________________________________________________________*/
#define PROJ_OFFSET game.grid_height * 0.4
#define SLOW_RANGE game.grid_width * 2
#define EXPLOSION_RANGE game.grid_width * 1.5
#define HOMING_RANGE game.grid_width * 3


/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author	
@brief		This function removes turrets from play grid by setting
			them to in active.
@param word - int index_x, int index_y
@return		- void
*//*_____________________________________________________________*/
void remove_turret(int index_x, int index_y);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author	
@brief		This function renders all active turrets in the game
			base on the type of turret it is and the range for the
			turrets
@param word - void
@return		- void
*//*_____________________________________________________________*/
void render_turret(void);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		This function updates all active turrets in game,
			determinig if any enemies are in range and set them as 
			a target, shooting at enemy.
@param word - void
@return		- void
*//*_____________________________________________________________*/
void update_turret(void);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		This function creates a projectile base on the type of 
			turret that calls the shoot function and set to active.
			Float x and y - determine the position where the projectile
			is created.
			Modifiers mod - contains the turret's modifiers to be passed
			to the projectile's modifier
			ProjectileType - type determines what type of projectile 
			to be created
			Vector2 dir - determines direction of projectile to 
			travel towards during update
@param word - float x, float y, Modifiers mod, 
			  ProjectileType type, Vector2 dir
@return		- void
*//*_____________________________________________________________*/
void shoot(float x, float y, Modifiers mod, ProjectileType type, Vector2 dir);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		This function updates all active projectiles base on the 
			type of projectiles they are.
@param word - void
@return		- void
*//*_____________________________________________________________*/
void update_projectile(void);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		This function renders all active projectiles base on the
			type of porjectile they are.
@param word - void
@return		- void
*//*_____________________________________________________________*/
void render_projectile(void);


/*!
@author     Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@co-author
@brief		This function updates turret animation states.
@param word - Turret* t
@return		- void
*//*_____________________________________________________________*/
void update_turretAnimation(Turret* t);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		enum ParticleType contains the type of particle to be
			used in particle related fucntions.
*//*_____________________________________________________________*/
typedef enum ParticleType
{
	kParBasic,
	kParSlow,
	kParHoming,
	kParMax
}ParticleType;

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		struct Particle contains variables needed for a particle
			object to be able to function.

			Vector2 dir		- direction of particle to move towards
			Vector2 pos		- position of particle
			float size		- size of particle to render
			float timer		- increment timer to reach duration
			float duration  - how long a particle should last
			int alpha		- alpha of the particle
			int is_active	- bool to check if particle is active
			ParticleType type - enum of the type of particle
*//*_____________________________________________________________*/
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

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		particles array pool
*//*_____________________________________________________________*/
Particle particles[250];

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		This function creates a particle by finding an inactive
			particle in array particles and setting it to active
			initalising it with the params provided

			Vector2 pos		- position of particle
			Vector2 dir		- direction of particle to move towards
			float size		- size of particle to render
			float duration  - how long a particle should last
			ParticleType type - enum of the type of particle

@param word - Vector2 pos, Vector2 dir, float size, 
			float duration, ParticleType type
@return		void
*//*_____________________________________________________________*/
void create_particle(Vector2 pos, Vector2 dir, float size, float duration, ParticleType type);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		This function updates active particle in particle
			array base on type of particle (Currently all particle
			uses the same pattern of movement)
@param word - void
@return		- void
*//*_____________________________________________________________*/
void update_particle(void);

/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		This function renders all active particle in array base
			on the particle type
@param word - void
@return		- void
*//*_____________________________________________________________*/
void render_particle(void);

#endif