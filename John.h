#include"cprocessing.h"
#include"game.h"
#define MAX_SPAWNING_ENEMIES kMaxEnemies-10

typedef enum Direction {
	NoMove,
	Up,
	Down,
	Left,
	Right
}Direction;

typedef enum EnvironmentEffectEnemy {
	Effected,
	Applying
}EnvironmentEffectEnemy;

//Combined function for all enemy effects for game.c
void update_enemy(void);


//Enemy updates for animation and collision
void EnemyAnimationState(Enemy* r);
int Check_state(Enemy* r);
void Reaper_ability(Enemy* r);
void EnemyDeath(Enemy* r);



//Enemy Render
void Draw_enemy(Enemy* r);
void draw_multiple_enemies(void);
void update_enemy_health_bar(Enemy* r);

//void Level0_waveEnemies_init(void);
void Reset_enemies(void);
void Current_wave_check(Enemy* r);
void Power_Up_check(Enemy* r);

//Enemy variable setup and intialising
void wave_enemy_init(int Basic_Ghost_count, int Fast_Ghost_count, int Fat_Ghost_count, int Grim_Reaper_count, LevelData Level);
void Basic_Ghost(Enemy* r);
void Fast_Ghost_init(Enemy* r);
void Fat_Ghost_init(Enemy* r);
void grimReaper_init(Enemy* r);
void Reaper_minion_init(Enemy* r);
void empty_enemy_init(Enemy* r);

//Enemy Movement functions
void enemy_move(Enemy* r, float Enemy_PathpointsX[], float Enemy_PathpointsY[], int number_of_points);
void Update_Path_Array(void);
void Check_pathAdjustment(Enemy* r);
void reset_enemy_path(Enemy* r);
int direction_to_next_point(float enemy_pathpointsX[], float enemy_pathpointsY[], Enemy* r);
int update_point_num(float enemy_pathpointsX[], float enemy_pathpointsY[], Enemy* r);


struct LinkedListNode* Enemy_node;

//global path for enemy path setup
float Xarray[50];
float Yarray[50];
int Array_count;
int Number_of_points;

//Enemy spawn timing
float Enemy_timer;
int wave_timer;


//No of Enemies
int basicEnemyNum;
int fastEnemyNum;
int fatEnemyNum;




//Environmental effects
void Environment_check(void);
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



void Music_init(void);