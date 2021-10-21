#include "cprocessing.h"
#include <math.h>
#include "John.h"
#include "Samuel.h"

void enemy_test_init(void)  //Initialising test enemy
{
	test.health = 5;
	test.speed = 15;
	test.CurrentWaypoint = 0;
	test.data.xOrigin = (float)(Game.xOrigin + Game.gridWidth * 1.5);
	test.data.yOrigin = (float)(Game.yOrigin + Game.gridHeight * 0.5);
	test.enemy_width = Game.gridWidth;
	test.enemy_height = Game.gridHeight;
	test.angle = 180;
	test.type = Red;
	test.alpha = 255;
	test.data.objectType = objectRectangle;
	test.data.width = test.enemy_width*0.5f;
	test.data.height = test.enemy_width*0.5f;
	test.state = Moving;
	count = 0;

	ImageArray[0] = CP_Image_Load("./Assets/RedArrow1.png");
	ImageArray[1] = CP_Image_Load("./Assets/RedArrow2.png");
	ImageArray[2] = CP_Image_Load("./Assets/RedArrow3.png");
	currentArrowImage = ImageArray[0];
	test.timer = 0;

	//test path
	for (int i = 0; i < 2; i++) {
		Xarray[i] = test.data.xOrigin;
		Yarray[i] = (test.data.yOrigin + Game.gridHeight * 6 * i);
	}
}

void Draw_enemy(enemy* r) { //Draws the enemy
	EnemyAnimationState(r);
	switch (r->type) {
	case Red:
		CP_Image_DrawAdvanced(currentArrowImage, r->data.xOrigin, r->data.yOrigin, r->enemy_width, r->enemy_height, r->alpha, r->angle);
		r->timer += CP_System_GetDt();
		break;
	case Blue:
		break;
	}
}

void EnemyAnimationState(enemy* r)
{
	switch (r->state)
	{
	case Moving:
		currentArrowImage = ImageArray[0];
		break;
	case Hurt:
		currentArrowImage = ImageArray[1];
		if (r->timer >= 0.25)
		{
			r->state = Moving;
			r->timer = 0;
		}
		break;
	case Death:
		currentArrowImage = ImageArray[2];
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
		r->data.yOrigin -= Speed;
		break;
	case Down:
		r->data.yOrigin += Speed;
		break;
	case Left:
		r->data.xOrigin -= Speed;
		break;
	case Right:
		r->data.xOrigin += Speed;
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
	float covered_distanceX = (float)fabs((double)r->data.xOrigin - (Enemy_PathpointsX[r->CurrentWaypoint]));
	float distance_between_pointsX = (float)fabs((double)Enemy_PathpointsX[r->CurrentWaypoint + 1] - (Enemy_PathpointsX[r->CurrentWaypoint]));
	float covered_distanceY = (float)fabs((double)r->data.yOrigin - (Enemy_PathpointsY[r->CurrentWaypoint]));
	float distance_between_pointsY = (float)fabs((double)Enemy_PathpointsY[r->CurrentWaypoint + 1] - (Enemy_PathpointsY[r->CurrentWaypoint]));

	if (distance_between_pointsX <= covered_distanceX) {
		if (distance_between_pointsY <= covered_distanceY) {

			r->CurrentWaypoint++;
		}
	}
	return 0;
}



void EnemyDeath(enemy* r) {
	for (int i = 0; i < MAX_PROJECTILE; ++i) {
		if (proj[i].isActive) {
			if (Collision_Detection(r->data, proj[i].data) == 1) {

				proj[i].isActive = 0;
				if (r->state != Death)
				{
					r->health -= turret[i].damage;
					r->state = Hurt;
					r->timer = 0;
				}
				
			}
		}
	}

	if (r->health == 0) {
		r->state = Death;
	}
	if (r->state == Death) {
		r->alpha -= 10;
		if (r->alpha <= 0)
			CP_Image_Free(&currentArrowImage);
	}
}
