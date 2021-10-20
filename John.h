
typedef enum EnemyTypes {
	Red,
	Blue
} EnemyTypes;

typedef enum EnemyState {
	Moving,
	Death
}enemystate;
typedef struct Enemy {
	int health, pos_num;
	float posX, posY,enemy_width,enemy_height, angle, speed;
	enemystate state;
	EnemyTypes type;
}enemy;

void enemy_init(void);
void Draw_enemy(enemy* r);
void enemy_move(enemy* r, float enemy_pathX[], float enemy_pathY[]);
int direction_to_next_point(float enemy_pathpointsX[], float enemy_pathpointsY[], int enemy_path_point_num);
int update_point_num(float enemy_pathpointsX[], float enemy_pathpointsY[], float enemy_posX, float enemy_posY, int enemy_path_point_num);
int Collision(enemy* r, float x, float y);

enemy test;
CP_Image Draw_Red_arrow;