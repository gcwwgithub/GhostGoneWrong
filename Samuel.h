#pragma once
#include "vector.h"

//change value as need
#define MAX_PROJECTILE 100
#define MAX_TURRET 100

//enum of turret types
typedef enum TurretType
{
	TRIANGLE,
	CIRCLE,
	STAR
} TurretType;

typedef struct Turret
{
	Vector2 dir, n_dir /*base dir*/;
	float pos_x, pos_y, size, angle, range;
	TurretType type;
} Turret;

typedef struct Projectile
{
	int isActive;
	float x, y;
	Vector2 dir;
} Projectile;

void render_turret(Turret* t);
//void render_turret(void);

void update_turret(Turret* t);
//void update_turret(void);

void shoot(float x, float y, Vector2 dir);

void projectile_update();

//void draw_projectile(Projectile p);

Projectile proj[MAX_PROJECTILE];
Turret turret[MAX_TURRET];

//test variables
Turret test;
CP_Image turret_img;