#pragma once
#include "vector.h"
#include "game.h"

//change value as need
#define PROJ_OFFSET game.grid_height * 0.4
#define SLOW_RANGE game.grid_width * 2
#define EXPLOSION_RANGE game.grid_width * 1.5
#define HOMING_RANGE game.grid_width * 3

//init turret and projectiles
void turret_init(void);
/*
	use this to place turret/spawn turret (pass in type and the grid index)
	E.g to place on grid[0][1] pass in type, 0, 1
*/
void place_turret(TurretType type, int index_x, int index_y);

//remove a turret from grid ()
void remove_turret(int index_x, int index_y);

//sell them turrets
void sell_turret(int t_index);

//upgrade system (for now using index will change depending how to access this easier)
void upgrade_turret(int t_index);

//void render_turret(Turret* t);
void render_turret(void);

//void update_turret(Turret* t);
void update_turret(void);

// Pass in the position to shoot from, type is an enum so can pass it turrettype if want or int, direction to shoot in
void shoot(float x, float y, Modifiers mod, ProjectileType type, Vector2 dir);

//updates projectiles
void update_projectile(void);

//render projectiles
void render_projectile(void);

//deteremine the collisoin effect base on type of proj
void col_type_projectile(Projectile* p);

//update animation
void update_turretAnimation(Turret* t);

//particle test
typedef enum PARTICLE_TYPE
{
	PAR_BASIC,
	PAR_SLOW,
	PAR_HOMING,
	PAR_MAX
}PARTICLE_TYPE;

typedef struct Particle
{
	Vector2 dir;
	Vector2 pos;
	float size;
	float timer;
	float duration;
	int alpha;
	int isActive;
	PARTICLE_TYPE type;
} Particle;


Particle particles[250];

void create_particle(Vector2 pos, Vector2 dir, float size, float duration, PARTICLE_TYPE type);
void update_particle();
void render_particle();