#pragma once
#include "vector.h"
#include "game.h"

//change value as need
#define MAX_PROJECTILE 100
#define MAX_TURRET 100
#define PROJ_OFFSET Game.gridHeight * 0.4
#define SLOW_RANGE Game.gridWidth * 2
//enum of turret types
typedef enum TurretType
{
	T_BASIC,
	T_SLOW,
	T_HOMING,
	T_MINE,
	T_WALL
} TurretType;

//enum of projectile types
typedef enum ProjectileType
{
	P_BASIC,
	P_SLOW,
	P_TRACKING,
	P_MINE,
} ProjectileType;

typedef enum TriangleAnimState
{
	INACTIVE,
	SHOOTING,
} TriangleAnimState;

typedef struct
{
	float damage, speed, range, cooldown,
		slow_amt, slow_timer;
} Modifiers;

typedef struct Turret
{
	int isActive;
	float size, angle;
	//flaot range, cooldown, damage;
	//float t_slow_amt, t_slow_timer; //for slow turret
	Modifiers mod;
	Vector2 dir;
	Coordinates data;
	TurretType type;
	TriangleAnimState currentAnimState;
	float turretAnimTimer;
	CP_Image turret_img;
	int animCounter;
} Turret;

typedef struct Projectile
{
	int isActive;
	float size;
	//float damage, speed, ;
	//float p_slow_amt, p_slow_timer; //for slow projectile
	Modifiers mod;
	Coordinates data;
	Vector2 dir;
	ProjectileType type;
} Projectile;

//init turret and projectiles
void turret_init(void);

/*
	use this to place turret/spawn turret (pass in type and the grid index)
	E.g to place on grid[0][1] pass in type, 0, 1
*/
void place_turret(TurretType type, int index_x, int index_y);

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

Projectile proj[MAX_PROJECTILE];
Turret turret[MAX_TURRET];
