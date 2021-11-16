#pragma once
#include "vector.h"
#include "game.h"

//change value as need
#define MAX_PROJECTILE 100
#define MAX_TURRET 100
#define PROJ_OFFSET Game.gridHeight * 0.4
#define SLOW_RANGE Game.gridWidth * 2
#define EXPLOSION_RANGE Game.gridWidth * 2
#define HOMING_RANGE Game.gridWidth * 3
#define TRUE 1
#define FALSE 0

//enum of turret types
typedef enum TurretType
{
	T_BASIC,
	T_SLOW,
	T_HOMING,
	T_MINE,
	T_WALL,
	T_MAX
} TurretType;

//enum of projectile types
typedef enum ProjectileType
{
	P_BASIC,
	P_SLOW,
	P_HOMING,
	P_MINE,
} ProjectileType;

typedef enum TriangleAnimState
{
	INACTIVE,
	SHOOTING,
	ACTIVE
} TriangleAnimState;

typedef enum TurretPurchase
{
	TP_PRICE,
	TP_UPGRADE_PRICE,
	TP_UPGRADE_MAX_LEVEL,
	TP_MAX
}TurretPurchase;

typedef struct Modifiers
{
	int tracked_index;
	float damage, speed, range, shoot_rate,
		slow_amt, slow_timer;
} Modifiers;

//For battlefield effects
typedef enum Turret_Env_effects {
	No_Effect,
	Increased_damage,
	Increased_attack_speed,
	Decreased_damage,
	Decreased_attack_speed
}Turret_Env_effects;

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
	// sell price, upgrade price , 
	// total accumulated price (each upgrades + base price) 
	int sell_price, upgrade_price, total_price;
	int level;

	//For Battlefield effects
	Turret_Env_effects Env_effect;
} Turret;

typedef struct Projectile
{
	int isActive;
	float size, lifetime;
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
typedef struct Particle
{
	Vector2 dir;
	Vector2 pos;
	float size;
	float timer;
	float duration;
	int isActive;
} Particle;

Particle particles[200];

void create_particle(Vector2 pos, Vector2 dir, float size, float duration);
void update_particle();
void render_particle();

Projectile proj[MAX_PROJECTILE];
Turret turret[MAX_TURRET];
int turret_on_grid[GAME_GRID_ROWS][GAME_GRID_COLS];
int turret_purchasing[TP_MAX][T_MAX];