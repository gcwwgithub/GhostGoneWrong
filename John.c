#include "cprocessing.h"
#include <math.h>
#include "John.h"

void enemy_test_init(void)  //Initialising test enemy
{
	test.health = 10;
	test.speed = 100;
	test.CurrentWaypoint = 0;
	test.posX = (float)(Game.xOrigin+Game.gridWidth*1.5);
	test.posY = (float)(Game.yOrigin + Game.gridHeight * 0.5);
	test.enemy_width = Game.gridWidth;
	test.enemy_height = Game.gridHeight;
	test.angle = 180;
	test.type = Red;
	test.alpha = 255;
	Draw_Red_arrow = CP_Image_Load("./Assets/Enemies_clone.png");
	//test path
	for (int i = 0; i < 2; i++) {
		Xarray[i] = test.posX;
		Yarray[i] = (test.posY + Game.gridHeight * 6 * i);
	}
}

void Draw_enemy(enemy* r) { //Draws the enemy
	switch (r->type) {
	case Red:
		CP_Image_DrawAdvanced(Draw_Red_arrow, r->posX, r->posY, r->enemy_width, r->enemy_height, r->alpha, r->angle);
		break;
	case Blue:
		break;
	}
}

void enemy_move(enemy* r, float Enemy_PathpointsX[], float Enemy_PathpointsY[], int number_of_points) { //Enemy movement
	float Speed = (r->speed) * CP_System_GetDt();
	update_point_num(Enemy_PathpointsX, Enemy_PathpointsY, r);
	if (r->CurrentWaypoint + 1 == number_of_points) {
		r->state = Death;
	}

	Direction direction_now = direction_to_next_point(Enemy_PathpointsX, Enemy_PathpointsY, r);
	switch (direction_now) {
	case Up:
		r->posY -= Speed;
		break;
	case Down:
		r->posY += Speed;
		break;
	case Left:
		r->posX -= Speed;
		break;
	case Right:
		r->posX += Speed;
		break;
	case NoMove:
		break;
	}

}


Direction direction_to_next_point(float Enemy_PathpointsX[], float Enemy_PathpointsY[], enemy* r) {   //Which direction to move depending on points
	float Xdistance_between_points = (Enemy_PathpointsX[r->CurrentWaypoint + 1] - Enemy_PathpointsX[r->CurrentWaypoint]);
	float Ydistance_between_points = (Enemy_PathpointsY[r->CurrentWaypoint + 1] - Enemy_PathpointsY[r->CurrentWaypoint]);
	if (r->state == Death) {
		return NoMove;
	}
	if (Xdistance_between_points == 0) {
		if (Ydistance_between_points < 0) {
			return Up; //Upwards movement
		}
		else {
			return Down; //Downwards movement
		}
	}
	else if (Ydistance_between_points == 0) {
		if (Xdistance_between_points < 0) {
			return Left; //Left movement
		}
		else {
			return Right; //Right movement
		}
	}
	return 0;
}

int update_point_num(float Enemy_PathpointsX[], float Enemy_PathpointsY[], enemy* r) { //Update position to move towards next point 
	float covered_distanceX = (float)fabs((double)r->posX - (Enemy_PathpointsX[r->CurrentWaypoint]));
	float distance_between_pointsX = (float)fabs((double)Enemy_PathpointsX[r->CurrentWaypoint + 1] - (Enemy_PathpointsX[r->CurrentWaypoint]));
	float covered_distanceY = (float)fabs((double)r->posY - (Enemy_PathpointsY[r->CurrentWaypoint]));
	float distance_between_pointsY = (float)fabs((double)Enemy_PathpointsY[r->CurrentWaypoint + 1] - (Enemy_PathpointsY[r->CurrentWaypoint]));

	if (distance_between_pointsX <= covered_distanceX) {
		if (distance_between_pointsY <= covered_distanceY) {

			r->CurrentWaypoint++;
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
	if (r->state == Death) {
		r->alpha -= 10;
		if (r->alpha <= 0)
		CP_Image_Free(&Draw_Red_arrow);
	}
}
