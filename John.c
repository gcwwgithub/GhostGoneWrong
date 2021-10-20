#include "cprocessing.h"#include "cprocessing.h"
#include "John.h"
#include <math.h>

void enemy_init(void)
{
	test.health = 10;
	test.speed = 100;
	test.pos_num = 0;
	test.posX = 200;
	test.posY = 0;
	test.enemy_width = 20;
	test.enemy_height = 20;
	test.angle = 0;
	test.type = Red;
	Draw_Red_arrow = CP_Image_Load("./Assets/Enemies_clone.png");

}

void Draw_enemy(enemy* r) {
	switch (r->type) {
	case Red:
		CP_Image_DrawAdvanced(Draw_Red_arrow, r->posX, r->posY, r->enemy_width, r->enemy_height, 255, r->angle);
		break;
	case Blue:
		break;
	}
}

void enemy_move(enemy* r, float enemy_pathX[], float enemy_pathY[]) {
	float Speed = (r->speed) * CP_System_GetDt();
	if (update_point_num(enemy_pathX, enemy_pathY, r->posX, r->posY, r->pos_num) == 1) {
		r->pos_num++;
	}
	int direction = direction_to_next_point(enemy_pathX, enemy_pathY, r->pos_num);
	switch (direction) {
	case 1:
		r->posY -= Speed;
		break;
	case 2:
		r->posY += Speed;
		break;
	case 3:
		r->posX -= Speed;
		break;
	case 4:
		r->posX += Speed;
		break;
	}

}

//	void enemy_death();

int direction_to_next_point(float enemy_pathpointsX[], float enemy_pathpointsY[], int enemy_path_point_num) {   //Which direction to move depending on points
	float Xdistance_between_points = (enemy_pathpointsX[enemy_path_point_num + 1] - enemy_pathpointsX[enemy_path_point_num]);
	float Ydistance_between_points = (enemy_pathpointsY[enemy_path_point_num + 1] - enemy_pathpointsY[enemy_path_point_num]);
	if (Xdistance_between_points == 0) {
		if (Ydistance_between_points < 0) {
			return 1; //Upwards movement
		}
		else {
			return 2; //Downwards movement
		}
	}
	else if (Ydistance_between_points == 0) {
		if (Xdistance_between_points < 0) {
			return 3; //Left movement
		}
		else {
			return 4; //Right movement
		}
	}
	return 0;
}

int update_point_num(float enemy_pathpointsX[], float enemy_pathpointsY[], float enemy_posX, float enemy_posY, int enemy_current_point) { //return 1 when next point is hit
	float covered_distanceX = (float)fabs((double)enemy_posX - (enemy_pathpointsX[enemy_current_point]));
	float distance_between_pointsX = (float)fabs((double)enemy_pathpointsX[enemy_current_point + 1] - (enemy_pathpointsX[enemy_current_point]));
	float covered_distanceY = (float)fabs((double)enemy_posY - (enemy_pathpointsY[enemy_current_point]));
	float distance_between_pointsY = (float)fabs((double)enemy_pathpointsY[enemy_current_point + 1] - (enemy_pathpointsY[enemy_current_point]));
	if (covered_distanceX >= distance_between_pointsX) {
		if (covered_distanceY >= distance_between_pointsY) {
			return 1;
		}
	}
	return 0;
}

int Collision(enemy* r, float x, float y) {   //returns 1 if collide with enemy else return 0
	if (x<(r->posX + r->enemy_width / 2) && x>(r->posX - r->enemy_width / 2)) {
		if (y<(r->posY + r->posY / 2) && x>(r->posY - r->posY / 2)) {
			return 1;
		}
	}
	return 0;
}


void EnemyDeath(enemy* r) {
	if (r->health == 0) {
		r->state = Death;
	}
	return 0;
}
