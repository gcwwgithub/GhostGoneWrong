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
	P_AOE,
	P_PIERCE,
	P_PERCENTAGE,
} ProjectileType;

typedef struct Turret
{
	int isActive;
	float size, angle, range, cooldown, damage;
	Vector2 dir;
	ObjectData data;
	TurretType type;
} Turret;

typedef struct Projectile
{
	int isActive;
	float damage, speed;
	ObjectData data;
	Vector2 dir;
} Projectile;

//init 
void turret_init(void);

//use this to place turret/spawn turret (pass in type and the position)
void place_turret(TurretType type, float x, float y);

//void render_turret(Turret* t);
void render_turret(void);

//void update_turret(Turret* t);
void update_turret(void);

void shoot(float x, float y, Vector2 dir);

void update_projectile(void);

void render_projectile(void);

Projectile proj[MAX_PROJECTILE];
Turret turret[MAX_TURRET];

//test variables
CP_Image turret_img;