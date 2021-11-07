#include"cprocessing.h"
#include"game.h"
#define MAX_ENEMIES 10
#define MAX_SPAWNING_ENEMIES MAX_ENEMIES-2

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
	Reached

}EnemyState;

typedef enum AbilityUsed{
	Used,
	charges_1,
	charges_2
}Ability_charge;

typedef struct Enemy {
	int CurrentWaypoint, health, alpha, points;
	float xOrigin, yOrigin, enemy_width, enemy_height,
		max_health, angle, speed, slow_amt, slow_timer;
	Coordinates data;
	EnemyState state;
	EnemyTypes type;
	CP_Image Render_Enemy;
	Ability_charge charges;
	int currentAnimState;

	float timer;
}enemy;


void Draw_enemy(enemy* r);
void enemy_move(enemy* r, float Enemy_PathpointsX[], float Enemy_PathpointsY[], int number_of_points, LevelData* Level);
int direction_to_next_point(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
int update_point_num(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
void EnemyAnimationState(enemy* r);
int Check_state(enemy* r);
void Reaper_ability(enemy* r);


void EnemyDeath(enemy* r, LevelData* Level);
void Basic_Ghost(enemy* r);
void Enemies_init(int Basic_enemy_count, int Fast_enemy_count, int Fat_enemy_count, LevelData* Level);
void update_enemy(void);
void draw_multiple_enemies(void);
void Fast_Ghost_init(enemy* r);
void Fat_Ghost_init(enemy* r);
void grimReaper_init(enemy* r);
void Reaper_minion_init(enemy* r);
void empty_enemy_init(enemy* r);

void Update_Path_Array(LevelData Level);

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
