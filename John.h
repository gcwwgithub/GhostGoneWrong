#include"cprocessing.h"
#include"game.h"
#define MAX_ENEMIES 6
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
	Fat_Ghost
} EnemyTypes;

typedef enum EnemyState {
	Inactive,
	Moving,
	Hurt,
	Death
	
}EnemyState;

typedef struct Enemy {
	int CurrentWaypoint, health,alpha,points;
	float xOrigin, yOrigin, enemy_width, enemy_height, 
		max_health, angle, speed, slow_amt, slow_timer;
	Coordinates data;
	EnemyState state;
	EnemyTypes type;
	CP_Image Render_Enemy;

	float timer;
}enemy;

void enemy_test_init(void);
void Draw_enemy(enemy* r);
void enemy_move(enemy* r, float Enemy_PathpointsX[], float Enemy_PathpointsY[], int number_of_points, LevelData* Level);
int direction_to_next_point(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
int update_point_num(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
void EnemyAnimationState(enemy* r);
int Check_state(enemy* r);

void EnemyDeath(enemy* r, LevelData* Level);
void Basic_Ghost(enemy* r);
void Enemies_init(int Basic_enemy_count, int Fast_enemy_count, int Fat_enemy_count,LevelData* Level);
void update_enemy(void);
void draw_multiple_enemies(void);
void Fast_Ghost_init(enemy* r);
void Fat_Ghost_init(enemy* r);

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


void update_enemy_health(enemy* r);
