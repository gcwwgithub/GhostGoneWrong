#include"cprocessing.h"
#include"game.h"
#define MAX_ENEMIES 60
#define MAX_SPAWNING_ENEMIES MAX_ENEMIES-10

typedef enum Direction {
	NoMove,
	Up,
	Down,
	Left,
	Right
}Direction;

typedef enum EnemyTypes {
	Basic,
	Fast_Ghost,
	Fat_Ghost,
	grimReaper
} EnemyTypes;

typedef enum EnemyState {
	Inactive,
	Moving,
	Hurt,
	Death,
	Reached,
	Adjusting

}EnemyState;

typedef enum AbilityUsed{
	Used,
	charges_1,
	charges_2
}Ability_charge;

typedef enum EnvironmentEffectEnemy {
	Effected,
	Applying
}EnvironmentEffectEnemy;



typedef struct Enemy {
	int CurrentWaypoint, health, alpha, points;
	float xOrigin, yOrigin, enemy_width, enemy_height,
		max_health, angle, speed, slow_amt, slow_timer;
	Coordinates data;
	EnemyState state;
	EnemyTypes type;
	CP_Image Render_Enemy;
	Ability_charge charges;
	EnvironmentEffectEnemy env_eff;
	int Enemy_pow_up[3];//Update to number of power ups

	int isToken;

	int WavePowUp_isActive;
	int currentAnimState;

	float EnemyPathX[50];
	float EnemyPathY[50];
	int pathPoints;
	float slowed_distance;

	int adjustingWaypoint;
	
	float movement_timer;
	float timer;
}enemy;


void Draw_enemy(enemy* r);
void enemy_move(enemy* r, float Enemy_PathpointsX[], float Enemy_PathpointsY[], int number_of_points, int CurrentGameLevel);
int direction_to_next_point(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
int update_point_num(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
void EnemyAnimationState(enemy* r);
int Check_state(enemy* r);
void Reaper_ability(enemy* r);


void EnemyDeath(enemy* r, int CurrentGameLevel);
void Basic_Ghost(enemy* r);
void Enemies_init(void);
void update_enemy(void);
void draw_multiple_enemies(void);
void Fast_Ghost_init(enemy* r);
void Fat_Ghost_init(enemy* r);
void grimReaper_init(enemy* r);
void Reaper_minion_init(enemy* r);
void empty_enemy_init(enemy* r);
//void Level0_waveEnemies_init(void);
void Reset_enemies(int current_level);
void Current_wave_check(enemy* r);

void wave_enemy_init(int Basic_Ghost_count, int Fast_Ghost_count, int Fat_Ghost_count, int Grim_Reaper_count, LevelData Level);

void Update_Path_Array(int CurrentGameLevel);
void Check_pathAdjustment(enemy* r);
void reset_enemy_path(enemy* r);

enemy test;
struct node* Enemy_node;

//test path
float Xarray[50];
float Yarray[50];
int count;
float timer;
int wave_timer;
int Array_count;
int Number_of_points;
enemy Enemy[MAX_ENEMIES];
enemy Reaper_minions[10];


void update_enemy_health_bar(enemy* r);

//Environmental effects
void Environment_check(int CurrentGameLevel);
void Env_eff_More_HP(void);
void Env_eff_Less_HP(void);
void Env_eff_Faster_Enemies(void);
void Env_eff_Slower_Enemies(void);
void Env_eff_Increased_Phantom_quartz(void);
void Env_eff_Decreased_Phantom_quartz(void);
void Env_eff_No_Phantom_quartz(void);
void Env_eff_IncreasedTurretDamage(void);
void Env_eff_DecreasedTurretDamage(void);
void Env_eff_DecreasedTurretAttackSpeed(void);
void Env_eff_IncreasedTurretAttackSpeed(void);