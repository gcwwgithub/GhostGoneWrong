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
void UpdateEnemies(void);


//Enemy updates for animation and collision
void EnemyAnimationState(Enemy* r);
int CheckEnemyState(Enemy* r);
void ReaperAbility(Enemy* r);
void EnemyDeath(Enemy* r);



//Enemy Render
void RenderEnemy(Enemy* r);
void RenderAllEnemies(void);
void RenderEnemyHealth(Enemy* r);

//void Level0_waveEnemies_init(void);
void ResetEnemyInit(void);
void EnemyWavePowUp(Enemy* r);
void EnemyVariableChangeforPowUps(Enemy* r);

//Enemy variable setup and intialising
void EnemyInitforWaves(int Basic_Ghost_count, int Fast_Ghost_count, int Fat_Ghost_count, int Grim_Reaper_count, LevelData Level);
void BasicGhostInit(Enemy* r);
void FastGhostInit(Enemy* r);
void FatGhostInit(Enemy* r);
void GrimReaperInit(Enemy* r);
void ReaperMinionInit(Enemy* r);
void EmptyEnemyInit(Enemy* r);

//Enemy Movement functions
void EnemyMovement(Enemy* r, float Enemy_PathpointsX[], float Enemy_PathpointsY[], int number_of_points);
void UpdateEnemyPathWaypointArray(void);
void CheckEnemyPathAdjustment(Enemy* r);
void ResetEnemyPathWaypoints(Enemy* r);
int DirectionToNextPoint(float enemy_pathpointsX[], float enemy_pathpointsY[], Enemy* r);
int UpdateEnemyCurrentWaypoint(float enemy_pathpointsX[], float enemy_pathpointsY[], Enemy* r);


struct LinkedListNode* Enemy_node;

//global path for enemy path setup
float GlobalEnemyPathXArray[50];
float GlobalEnemyPathYArray[50];
int Array_counter;
int Number_of_points;

//Enemy spawn timing
float Enemy_timer;
int wave_timer;


//No of Enemies
int basicEnemyNum;
int fastEnemyNum;
int fatEnemyNum;




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



void MusicInit(void);