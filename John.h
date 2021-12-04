/*!
@file       John.h
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author  Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@course     CSD 1400
@section    C
@date       4/12/2021
@brief    	Contains function declaration for
EnemyAnimationState, CheckEnemyState, ReaperAbility, EnemyDeath,
RenderAllEnemies, RenderEnemyHealth 
ResetEnemyInit, EnemyWavePowUp, EnemyVariableChangeforPowUps,
EnemyInitforWaves, BasicGhostInit, FastGhostInit, FatGhostInit, GrimReaperInit, ReaperMinionInit, EmptyEnemyInit,
EnemyMovement, UpdateEnemyPathWaypointArray, CheckEnemyPathAdjustment, ResetEnemyPathWaypoints, DirectionToNextPoint, UpdateEnemyCurrentWaypoint,
EnvironmentEffCheck, EnvEffMoreHP, EnvEffLessHP, EnvEffFasterEnemies, EnvEffSlowerEnemies, EnvEffIncreasedPhantomQuartz,
EnvEffDecreasedPhantomQuartz, EnvEffNoPhantomQuartz, EnvEffIncreasedTurretDamage, EnvEffDecreasedTurretDamage, EnvEffDecreasedTurretAttackSpeed, EnvEffIncreasedTurretAttackSpeed.

Also Contains enum for Direction and EnvironmentEffectEnemy
*//*__________________________________________________________________________*/

#include"cprocessing.h"
#include"game.h"
#define MAX_SPAWNING_ENEMIES kMaxEnemies-10

typedef enum Direction {
	kNoMove,
	kUp,
	kDown,
	kLeft,
	kRight
}Direction;

typedef enum EnvironmentEffectEnemy {
	kEffected,
	kApplying
}EnvironmentEffectEnemy;

//Combined function for all enemy effects except render for game.c
void UpdateEnemies(void);


//Enemy updates for animation and collision
void EnemyAnimationState(Enemy* r);
int CheckEnemyState(Enemy* r);
void ReaperAbility(Enemy* r);
void EnemyDeath(Enemy* r);



//Enemy Render
void RenderAllEnemies(void);
void RenderEnemyHealth(Enemy* r);

//Setting up enemies for wave
void ResetEnemyInit(void);
void EnemyWavePowUp(Enemy* r);
void EnemyVariableChangeforPowUps(Enemy* r);

//Enemy variable setup and intialising
void EnemyInitforWaves(int basic_ghost_count, int fast_ghost_count, int fat_ghost_count, int grim_reaper_count, LevelData level);
void BasicGhostInit(Enemy* r);
void FastGhostInit(Enemy* r);
void FatGhostInit(Enemy* r);
void GrimReaperInit(Enemy* r);
void ReaperMinionInit(Enemy* r);
void EmptyEnemyInit(Enemy* r);

//Enemy Movement functions
void EnemyMovement(Enemy* r, float enemy_pathpoints_X[], float enemy_pathpoints_Y[], int number_of_points);
void UpdateEnemyPathWaypointArray(void);
void CheckEnemyPathAdjustment(Enemy* r);
void ResetEnemyPathWaypoints(Enemy* r);
int DirectionToNextPoint(float enemy_pathpoints_X[], float enemy_pathpoints_Y[], Enemy* r);
void UpdateEnemyCurrentWaypoint(float enemy_pathpoints_X[], float enemy_pathpoints_Y[], Enemy* r);


struct LinkedListNode* Enemy_node;

//global path for enemy path setup
float global_enemy_path_X_array[50];
float global_enemy_path_Y_array[50];
int array_counter;
int number_of_points;

//Enemy spawn timing
float enemy_timer;
int wave_timer;


//No of Enemies
int basic_enemy_count;
int fast_enemy_count;
int fat_enemy_count;




//Environmental effects
void EnvironmentEffCheck(void);
void EnvEffMoreHP(void);
void EnvEffLessHP(void);
void EnvEffFasterEnemies(void);
void EnvEffSlowerEnemies(void);
void EnvEffIncreasedPhantomQuartz(void);
void EnvEffDecreasedPhantomQuartz(void);
void EnvEffNoPhantomQuartz(void);
void EnvEffIncreasedTurretDamage(void);
void EnvEffDecreasedTurretDamage(void);
void EnvEffDecreasedTurretAttackSpeed(void);
void EnvEffIncreasedTurretAttackSpeed(void);


//Initialise all sounds
void MusicInit(void);