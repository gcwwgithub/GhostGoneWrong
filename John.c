#include "cprocessing.h"
#include <math.h>
#include "John.h"
#include "Samuel.h"
#include "Gabriel.h"

void enemy_test_init(void)  //Initialising test enemy
{
	test.health = 4;
	test.max_health = 4;
	test.speed = 15;
	test.CurrentWaypoint = 0;
	test.data.xOrigin = (float)(Game.xOrigin + Game.gridWidth * 1.5);
	test.data.yOrigin = (float)(Game.yOrigin + Game.gridHeight * 0.5);
	test.enemy_width = Game.gridWidth;
	test.enemy_height = Game.gridHeight;
	test.angle = 180;
	test.type = Basic;
	test.alpha = 255;
	test.data.objectType = objectRectangle;
	test.data.width = Game.gridWidth * 0.5f;
	test.data.height = Game.gridHeight;
	test.state = Moving;
	count = 0;
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
	case Basic:
		CP_Image_DrawAdvanced(r->Render_Enemy, r->data.xOrigin, r->data.yOrigin, r->enemy_width, r->enemy_height, r->alpha, r->angle);
		r->timer += CP_System_GetDt();
		break;
	case Fast_Ghost:
		CP_Image_DrawAdvanced(r->Render_Enemy, r->data.xOrigin, r->data.yOrigin, r->enemy_width, r->enemy_height, r->alpha, r->angle);
		r->timer += CP_System_GetDt();
		break;
	}
}

void EnemyAnimationState(enemy* r)
{
	int i = Check_state(r);
	if (i == 1) {
		if (r->timer >= 0.25) {
			r->state = Moving;
		}
	}
	switch (r->type) {
	case Basic:
		r->Render_Enemy = basicGhostImageArray[i];
		break;
	case Fast_Ghost:
		r->Render_Enemy = fastGhostImageArray[i];
		break;
	case Fat_Ghost:
		r->Render_Enemy = fatGhostImageArray[i];
		break;
	}
}
int Check_state(enemy* r) {
	switch (r->state) {
	case Moving:
		return 0;
	case Hurt:
		return 1;
	case Death:
		return 2;
	}
	return 0;
}

void enemy_move(enemy* r, float Enemy_PathpointsX[], float Enemy_PathpointsY[], int number_of_points) { //Enemy movement
	float Speed = (r->speed) * r->slow_amt * CP_System_GetDt();
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



void EnemyDeath(enemy* r) {  //function updates and checks for collision or death also what happens upon death
	for (int i = 0; i < MAX_PROJECTILE; ++i) {
		if (proj[i].isActive) {
			Coordinates a = r->data;
			a.width *= 0.7f;
			a.height *= 0.7f;
			if (Collision_Detection(a, proj[i].data) == 1) {
				proj[i].isActive = 0;
				if (r->state != Death)
				{
					col_type_projectile(&proj[i]);
					r->health -= turret[i].mod.damage;
					r->state = Hurt;
					r->timer = 0;


				}
				insert_new_node(&firstNode, r->data.xOrigin, r->data.yOrigin, r->type);
			}
		}
	}

	if (r->health <= 0) {
		r->state = Death;
	}
	if (r->state == Death) {
		if (r->alpha >= 0) {
			r->alpha -= 10;
		}
		if (r->alpha <= 0) {
			r->state = Inactive;
		}

	}
}

void Enemies_init(int Basic_enemy_count, int Fast_enemy_count, int Fat_enemy_count) {
	timer = 0;
	count = 0;
	Enemy_node = NULL;
	wave_timer = 0;

	//test path
	for (int i = 0; i < 2; i++) {
		Xarray[i] = test.data.xOrigin;
		Yarray[i] = (test.data.yOrigin + Game.gridHeight * 6 * i);
	}
	for (int i = 0; i < Basic_enemy_count; i++) {
		Basic_Ghost(&Enemy[i]);
	}
	for (int i = Basic_enemy_count; i < Basic_enemy_count + Fast_enemy_count; i++) {
		Fast_Ghost_init(&Enemy[i]);
	}
	for (int i = Basic_enemy_count + Fast_enemy_count; i < MAX_ENEMIES; i++) {
		Fat_Ghost_init(&Enemy[i]);
	}
}

void Basic_Ghost(enemy* r) { // setup variable for basic ghost enemy
	r->health = 4;
	r->max_health = 4;
	r->speed = 10;
	r->CurrentWaypoint = 0;
	r->data.xOrigin = Xarray[0];
	r->data.yOrigin = Yarray[0];
	r->enemy_width = Game.gridWidth;
	r->enemy_height = Game.gridHeight;
	r->angle = 0;
	r->type = Basic;
	r->alpha = 255;
	r->data.objectType = objectCircle;
	r->data.width = Game.gridWidth;
	r->data.height = Game.gridWidth;
	r->state = Inactive;
	r->timer = 0;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
}

void Fast_Ghost_init(enemy* r) { // setup variable for fast ghost enemy
	r->health = 2;
	r->max_health = 2;
	r->speed = 40;
	r->CurrentWaypoint = 0;
	r->data.xOrigin = Xarray[0];
	r->data.yOrigin = Yarray[0];
	r->enemy_width = Game.gridWidth;
	r->enemy_height = Game.gridHeight;
	r->angle = 0;
	r->type = Fast_Ghost;
	r->alpha = 255;
	r->data.objectType = objectCircle;
	r->data.width = Game.gridWidth;
	r->data.height = Game.gridWidth;
	r->state = Inactive;
	r->timer = 0;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
}

void Fat_Ghost_init(enemy* r) {
	r->health = 10;
	r->max_health = 10;
	r->speed = 10;
	r->CurrentWaypoint = 0;
	r->data.xOrigin = Xarray[0];
	r->data.yOrigin = Yarray[0];
	r->enemy_width = Game.gridWidth;
	r->enemy_height = Game.gridHeight;
	r->angle = 0;
	r->type = Fat_Ghost;
	r->alpha = 255;
	r->data.objectType = objectCircle;
	r->data.width = Game.gridWidth;
	r->data.height = Game.gridWidth;
	r->state = Inactive;
	r->timer = 0;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
}

void update_enemy(void) {
	timer += CP_System_GetDt();
	if (timer >= 1) {
		count++;
		timer--;
	}
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if ((Enemy[i].state == Inactive) && (Enemy[i].health >= 1)) {

			int b = count;
			if (b - wave_timer >= 5) {
				Enemy[i].state = Moving;
				wave_timer = count;
			}
		}
		if (Enemy[i].state == Inactive) //dont check if inactive
			continue;

		if (Enemy[i].slow_timer > 0.f)
		{
			Enemy[i].slow_timer -= CP_System_GetDt();
			if (Enemy[i].slow_timer <= 0.f)
				Enemy[i].slow_amt = 1.f;
		}


		enemy_move(&Enemy[i], Xarray, Yarray, 2);
		EnemyDeath(&Enemy[i]);
	}
	enemy* En = &Enemy[0];
}
void draw_multiple_enemies(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (Enemy[i].state == Inactive) {
			continue;
		}
		EnemyAnimationState(&Enemy[i]);
		switch (Enemy[i].type) {
		case Basic:
			CP_Image_DrawAdvanced(Enemy[i].Render_Enemy, Enemy[i].data.xOrigin, Enemy[i].data.yOrigin, Enemy[i].enemy_width, Enemy[i].enemy_height, Enemy[i].alpha, Enemy[i].angle);
			Enemy[i].timer += CP_System_GetDt();
			break;
		case Fast_Ghost:
			CP_Image_DrawAdvanced(Enemy[i].Render_Enemy, Enemy[i].data.xOrigin, Enemy[i].data.yOrigin, Enemy[i].enemy_width, Enemy[i].enemy_height, Enemy[i].alpha, Enemy[i].angle);
			Enemy[i].timer += CP_System_GetDt();
			break;
		case Fat_Ghost:
			CP_Image_DrawAdvanced(Enemy[i].Render_Enemy, Enemy[i].data.xOrigin, Enemy[i].data.yOrigin, Enemy[i].enemy_width, Enemy[i].enemy_height, Enemy[i].alpha, Enemy[i].angle);
			Enemy[i].timer += CP_System_GetDt();
			break;
		}
		update_enemy_health(&Enemy[i]);
	}
}

void update_enemy_health(enemy* r)
{
	if (r->health != 0)
	{
		float newWidth = r->health / r->max_health;
		CP_Settings_Fill(COLOR_RED);
		CP_Graphics_DrawRect(r->data.xOrigin - r->enemy_width, r->data.yOrigin - r->enemy_height, r->enemy_width * 2, r->enemy_height / 4);

		CP_Settings_Fill(COLOR_GREEN);
		CP_Graphics_DrawRect(r->data.xOrigin - r->enemy_width, r->data.yOrigin - r->enemy_height, r->enemy_width * 2 * newWidth, r->enemy_height / 4);
	}


}
