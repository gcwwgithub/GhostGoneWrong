#include"cprocessing.h"
#include"game.h"
#define MAX_ENEMIES 2
typedef enum Direction {
	NoMove,
	Up,
	Down,
	Left,
	Right
}Direction;

typedef enum EnemyTypes {
	Red,
	Blue
} EnemyTypes;

typedef enum EnemyState {
	Inactive,
	Moving,
	Hurt,
	Death
	
}EnemyState;

typedef struct Enemy {
	int CurrentWaypoint, alpha;
	float xOrigin, yOrigin, enemy_width, enemy_height, health, angle, speed;
	Coordinates data;
	EnemyState state;
	EnemyTypes type;

	float timer;
}enemy;

void enemy_test_init(void);
void Draw_enemy(enemy* r);
void enemy_move(enemy* r, float Enemy_PathpointsX[], float Enemy_PathY[], int number_of_points);
int direction_to_next_point(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
int update_point_num(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
void EnemyAnimationState(enemy* r);

void EnemyDeath(enemy* r);
void Red_arrow(enemy* r);
void Enemies_init(void);
void update_enemy(void);
void draw_multiple_enemies(void);

enemy test;
CP_Image ImageArray[3];
CP_Image currentArrowImage;

//test path
float xpoint;
float ypoint;
float Xarray[2];
float Yarray[2];
int count;
float timer;
enemy Enemy[MAX_ENEMIES];
/* //test enemy movement
enemy_move(&test, Xarray, Yarray, 2);
Draw_enemy(&test);
EnemyDeath(&test);

//test enemy
enemy_test_init();*/