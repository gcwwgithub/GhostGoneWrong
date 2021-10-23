#pragma once
#include "vector.h"
#include "game.h"

//change value as need
#define MAX_PROJECTILE 100
#define MAX_TURRET 100

//enum of turret types
typedef enum TurretType
{
	T_TRIANGLE,
	T_CIRCLE,
	T_STAR,
	T_PRECENTAGE
} TurretType;

//enum of projectile types
typedef enum ProjectileType
{
	P_BASIC,
	P_SLOW,
	P_PIERCE,
	P_PERCENTAGE,
} ProjectileType;

typedef enum TriangleAnimState
{
	INACTIVE,
	SHOOTING,
} TriangleAnimState;

typedef struct Turret
{
	int isActive;
	float size, angle, range, cooldown, damage;
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
	float damage, speed;
	Coordinates data;
	Vector2 dir;
} Projectile;

//init 
void turret_init(void);

/*
* use this to place turret/spawn turret (pass in type and the grid index)
* E.g to place on grid[0][1] pass in type, 0, 1
*/
void place_turret(TurretType type, int index_x, int index_y);

//void render_turret(Turret* t);
void render_turret(void);

//void update_turret(Turret* t);
void update_turret(void);

void shoot(float x, float y, Vector2 dir);

void update_projectile(void);

void render_projectile(void);

void update_turretAnimation(Turret* t);

Projectile proj[MAX_PROJECTILE];
Turret turret[MAX_TURRET];
