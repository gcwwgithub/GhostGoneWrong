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
	Moving,
	Death
}EnemyState;

typedef struct Enemy {
	int health, CurrentWaypoint;
	float posX, posY, enemy_width, enemy_height, angle, speed;
	EnemyState state;
	EnemyTypes type;
}enemy;

void enemy_init(void);
void Draw_enemy(enemy* r);
void enemy_move(enemy* r, float Enemy_PathpointsX[], float Enemy_PathY[], int number_of_points);
int direction_to_next_point(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
int update_point_num(float enemy_pathpointsX[], float enemy_pathpointsY[], enemy* r);
int Collision(enemy* r, float x, float y);

enemy test;
CP_Image Draw_Red_arrow;