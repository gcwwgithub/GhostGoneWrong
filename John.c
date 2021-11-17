#include "cprocessing.h"
#include <math.h>
#include "John.h"
#include "Samuel.h"
#include "Gabriel.h"
#include "Anderson.h"


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
	r->currentAnimState = i;

}
int Check_state(enemy* r) {
	switch (r->state) {
	case Moving:
		return 0;
	case Adjusting:
		return 0;
	case Hurt:
		return 1;
	case Death:
		return 2;
	}
	return 0;
}

void enemy_move(enemy* r, float Enemy_PathpointsX[], float Enemy_PathpointsY[], int number_of_points, int CurrentGameLevel) { //Enemy movement
	float Speed = (r->speed) * r->slow_amt * CP_System_GetDt();
	update_point_num(Enemy_PathpointsX, Enemy_PathpointsY, r);
	if (r->CurrentWaypoint>=2 && r->state == Adjusting) {
		r->state = Moving;
		reset_enemy_path(r);
		r->CurrentWaypoint = r->adjustingWaypoint;
	}
	if (r->CurrentWaypoint + 1 == number_of_points && r->state != Adjusting) {
		if (r->state != Death && r->state != Inactive && r->state != Reached) {
			Level->health -= 10;
			insert_new_node_portal(&portalEnterFirstNode, r->data.xOrigin,
				r->data.yOrigin, 0);
		}
		r->state = Reached;
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
	if (r->state == Death||r->state==Reached) {
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



void EnemyDeath(enemy* r, int CurrentGameLevel) {  //function updates and checks for collision or death also what happens upon death
	for (int i = 0; i < MAX_PROJECTILE; ++i) {
		if (proj[i].isActive) {
			Coordinates a = r->data;
			a.width *= 0.7f;
			a.height *= 0.7f;
			if (Collision_Detection(a, proj[i].data) == 1) {
				proj[i].isActive = 0;
				col_type_projectile(&proj[i]);
				if (r->state != Death)
				{
					if (proj[i].type != T_SLOW)
					{
						r->health -= turret[i].mod.damage;
						r->state = Hurt;
						r->timer = 0;
					}

					insert_new_node(&bulletRadiusFirstNode, r->data.xOrigin, r->data.yOrigin, proj[i].type);
				}

			}
		}
	}

	if (r->health <= 0) {
		if (r->state == Hurt) {
			Level->phantomQuartz += r->points;
		}
		r->state = Death;

	}
	if (r->state == Death || r->state == Reached) {
		if (r->alpha >= 0) {
			r->alpha -= 10;
		}
		if (r->alpha <= 0) {
			r->state = Inactive;
			r->health = 0; //do not delete this line
			if (!r->isToken) {
				enemiesLeft--;
			}
			switch (r->type)
			{
			case Basic:
				basicEnemyNum--;
				break;
			case Fast_Ghost:
				fastEnemyNum--;
				break;
			case Fat_Ghost:
				fatEnemyNum--;
				break;
			}
		}

	}
}

void Enemies_init(void) {
	timer = 0;
	count = 0;
	Enemy_node = NULL;
	wave_timer = 0;
	Array_count = 1;
	Number_of_points = 0;


	//test path
	for (int i = 0; i < MAX_ENEMIES; i++) {
		empty_enemy_init(&Enemy[i]);
	}
}

void Basic_Ghost(enemy* r) { // setup variable for basic ghost enemy
	r->health = 4;
	r->max_health = 4;
	r->speed = 15;
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
	r->points = 25;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
	r->currentAnimState = 0;
	reset_enemy_path(r);
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
	r->points = 25;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
	r->currentAnimState = 0;
	reset_enemy_path(r);
}

void Fat_Ghost_init(enemy* r) {
	r->health = 10;
	r->max_health = 10;
	r->speed = 15;
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
	r->points = 300;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
	r->currentAnimState = 0;
	reset_enemy_path(r);
}

void update_enemy(void) {
	timer += CP_System_GetDt();
	if (timer >= 1) {
		count++;
		timer = 0;

	}
	for (int i = 0; i < MAX_ENEMIES; i++) {
		int spawn_timer = 5;
		if ((Enemy[i].state == Inactive) && (Enemy[i].health >= 1) && (count / spawn_timer <= MAX_ENEMIES)) {
			int state_check=0;
			int b = count;
			for (int j=0; j < MAX_ENEMIES; j++) {
				if (Enemy[j].state == Inactive) {
					state_check++;
				}
			}
			if (state_check == MAX_ENEMIES) {
				wave_timer = b - spawn_timer;
			}
			if (b - wave_timer >= spawn_timer) {
				Enemy[i].state = Moving;
				wave_timer = count;
				insert_new_node_portal(&portalSpawnFirstNode, Enemy[i].data.xOrigin,
					Enemy[i].data.yOrigin, 0);


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

		Update_Path_Array(currentGameLevel);
		Check_pathAdjustment(&Enemy[i]);
		enemy_move(&Enemy[i], Enemy[i].EnemyPathX, Enemy[i].EnemyPathY, Number_of_points, currentGameLevel);
		EnemyDeath(&Enemy[i], currentGameLevel);
		Reaper_ability(&Enemy[i]);
		Environment_check(currentGameLevel);
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
			RenderWithAlphaChanged(basicGhostSpriteSheet, basicGhostSpriteArray[Enemy[i].currentAnimState],
				Enemy[i].data.xOrigin, Enemy[i].data.yOrigin, Enemy[i].enemy_width, Enemy[i].enemy_height, Enemy[i].alpha);

			Enemy[i].timer += CP_System_GetDt();
			break;
		case Fast_Ghost:
			RenderWithAlphaChanged(fastGhostSpriteSheet, fastGhostSpriteArray[Enemy[i].currentAnimState],
				Enemy[i].data.xOrigin, Enemy[i].data.yOrigin, Enemy[i].enemy_width, Enemy[i].enemy_height, Enemy[i].alpha);
			Enemy[i].timer += CP_System_GetDt();
			break;
		case Fat_Ghost:
			RenderWithAlphaChanged(fatGhostSpriteSheet, fatGhostSpriteArray[Enemy[i].currentAnimState],
				Enemy[i].data.xOrigin, Enemy[i].data.yOrigin, Enemy[i].enemy_width, Enemy[i].enemy_height, Enemy[i].alpha);
			Enemy[i].timer += CP_System_GetDt();
			break;
		case grimReaper:
			RenderWithAlphaChanged(grimReaperSpriteSheet, grimReaperSpriteArray[Enemy[i].currentAnimState],
				Enemy[i].data.xOrigin, Enemy[i].data.yOrigin, Enemy[i].enemy_width, Enemy[i].enemy_height, Enemy[i].alpha);
			Enemy[i].timer += CP_System_GetDt();
			break;
		}
		update_enemy_health_bar(&Enemy[i]);

	}
}

void update_enemy_health_bar(enemy* r)
{
	if (r->health > 0)
	{
		if (r->health <= 0)
		{
			r->health = 0;
		}

		float newWidth = r->health / r->max_health;
		CP_Settings_Fill(COLOR_RED);
		CP_Graphics_DrawRect(r->data.xOrigin - r->enemy_width, r->data.yOrigin - r->enemy_height, r->enemy_width * 2, r->enemy_height / 4);

		CP_Settings_Fill(COLOR_GREEN);
		CP_Graphics_DrawRect(r->data.xOrigin - r->enemy_width, r->data.yOrigin - r->enemy_height, r->enemy_width * 2 * newWidth, r->enemy_height / 4);
	}


}

void Update_Path_Array(int CurrentGameLevel) {
	int nextPathRow = 1, nextPathCol = 1;
	for (int currentCost = 1; currentCost <= Level[CurrentGameLevel].grid[Level[CurrentGameLevel].exitRow][Level[CurrentGameLevel].exitCol].cost; currentCost++) {
		for (int currentRow = 0; currentRow < gameGridRows; currentRow++) {
			for (int currentCol = 0; currentCol < gameGridCols; currentCol++) {
				if (Level[CurrentGameLevel].grid[currentRow][currentCol].cost == currentCost && (Level[CurrentGameLevel].grid[currentRow][currentCol].type == Path || Level[CurrentGameLevel].grid[currentRow][currentCol].type == Exit)) {
					nextPathRow = currentRow;
					nextPathCol = currentCol;
					break;
				}
			}
		}
		Xarray[Array_count] = (Game.xOrigin + Game.gridWidth * (0.5 + nextPathCol));
		Yarray[Array_count] = (Game.yOrigin + Game.gridHeight * (0.5 + nextPathRow));
		Array_count++;
	}
	Number_of_points = Array_count;
	Array_count = 1;
}

void grimReaper_init(enemy* r) {
	r->health = 20;
	r->max_health = 20;
	r->speed = 15;
	r->CurrentWaypoint = 0;
	r->data.xOrigin = Xarray[0];
	r->data.yOrigin = Yarray[0];
	r->enemy_width = Game.gridWidth;
	r->enemy_height = Game.gridHeight;
	r->angle = 0;
	r->type = grimReaper;
	r->alpha = 255;
	r->data.objectType = objectCircle;
	r->data.width = Game.gridWidth;
	r->data.height = Game.gridWidth;
	r->state = Inactive;
	r->timer = 0;
	r->points = 300;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
	r->currentAnimState = 0;
	r->charges = charges_1;
	reset_enemy_path(r);
}

void Reaper_minion_init(enemy* r) {
	int a = 0;
	for (int i = MAX_SPAWNING_ENEMIES; i < MAX_ENEMIES; i++) {
		if (a == 0) {
			if (Enemy[i].state == Inactive) {
				Basic_Ghost(&Enemy[i]);
				Enemy[i].data.xOrigin = Xarray[r->CurrentWaypoint - 1];
				Enemy[i].data.yOrigin = Yarray[r->CurrentWaypoint - 1];
				Enemy[i].CurrentWaypoint = r->CurrentWaypoint - 1;
				Enemy[i].state = Moving;
				Enemy[i].isToken = 1;
				a++;
			}
		}
		else if (a == 1) {
			if (Enemy[i].state == Inactive) {
				Basic_Ghost(&Enemy[i]);
				Enemy[i].data.xOrigin = Xarray[r->CurrentWaypoint + 1];
				Enemy[i].data.yOrigin = Yarray[r->CurrentWaypoint + 1];
				Enemy[i].CurrentWaypoint = r->CurrentWaypoint + 1;
				Enemy[i].state = Moving;
				Enemy[i].isToken = 1;
				a++;
			}
		}
		else if (a == 2) {
			r->charges = Used;
			return;
		}
	}
}


void Reaper_ability(enemy* r) {
	if (r->type == grimReaper) {
		if (r->health == 0.5 * r->max_health) {
			if (r->charges == charges_1) {
				Reaper_minion_init(r);
			}
		}
	}
}

void empty_enemy_init(enemy* r) {
	r->health = 0;
	r->max_health = 0;
	r->speed = 0;
	r->CurrentWaypoint = 0;
	r->data.xOrigin = 0;
	r->data.yOrigin = 0;
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
	r->points = 0;
	//for the freeze turret & enemy interaction
	r->slow_amt = 0;
	r->slow_timer = 0;
	r->currentAnimState = 0;
	r->charges = Used;
	r->env_eff = Applying;
	//r->Enemy_pow_up.Less_HP = 0;
	//r->Enemy_pow_up.More_Points = 0;
	//r->Enemy_pow_up.SpeedDown = 0;
	r->WavePowUp_isActive = 0;
	r->isToken = 0;
	int pathPoints;
	float slowed_distance;
	for (int i = 0; i < 50; i++) {
		r->EnemyPathX[i] = 0;
		r->EnemyPathY[i] = 0;
	}
}




void wave_enemy_init(int Basic_Ghost_count, int Fast_Ghost_count, int Fat_Ghost_count, int Grim_Reaper_count, LevelData Level) {
	timer = 0;
	count = 0;
	Enemy_node = NULL;
	wave_timer = 0;
	Xarray[0] = (Game.xOrigin + Game.gridWidth * (0.5 + Level.spawnCol));
	Yarray[0] = (Game.yOrigin + Game.gridHeight * (0.5 + Level.spawnRow));

	for (int i = 0; i < MAX_ENEMIES; i++) {
		empty_enemy_init(&Enemy[i]);
	}
	int a = Basic_Ghost_count + Fast_Ghost_count;
	int b = a + Fast_Ghost_count;
	int c = b + Grim_Reaper_count;
	Xarray[0] = (Game.xOrigin + Game.gridWidth * (0.5 + Level.spawnCol));
	Yarray[0] = (Game.yOrigin + Game.gridHeight * (0.5 + Level.spawnRow));
	for (int i = 0; i < Basic_Ghost_count; i++) {
		Basic_Ghost(&Enemy[i]);
	}
	for (int i = Basic_Ghost_count; i < a && i < MAX_SPAWNING_ENEMIES; i++) {
		Fast_Ghost_init(&Enemy[i]);
	}
	for (int i = a; i < b && i < MAX_SPAWNING_ENEMIES; i++) {
		Fat_Ghost_init(&Enemy[i]);
	}
	for (int i = b; i < MAX_SPAWNING_ENEMIES && i < c; i++) {
		grimReaper_init(&Enemy[i]);
	}
}

void Reset_enemies(int current_level) {
	if (currentGameState == Building) {
		Update_Path_Array(currentGameLevel);
		if (buildingTime > 0.05f && Level[current_level].currentWave + 1 < MAX_NUMBER_OF_WAVES) {
			int BasicCount = Level[current_level].waveEnemies[Level[current_level].currentWave + 1][Basic];
			int FastCount = Level[current_level].waveEnemies[Level[current_level].currentWave + 1][Fast_Ghost];
			int FatCount = Level[current_level].waveEnemies[Level[current_level].currentWave + 1][Fat_Ghost];
			int ReaperCount = Level[current_level].waveEnemies[Level[current_level].currentWave + 1][grimReaper];
			wave_enemy_init(BasicCount, FastCount, FatCount, ReaperCount, Level[current_level]);
			enemiesLeft = BasicCount + FastCount + FatCount + ReaperCount;
		}
	}
}


void Check_pathAdjustment(enemy* r) {
	int XorY = 0;
	int check = 0;
	if (r->state != Adjusting&&r->state != Inactive&&r->state !=Death&&r->state != Reached) {
		for (int j = 0; j < Number_of_points; j++) {
			if (r->EnemyPathX[r->CurrentWaypoint + 1] == Xarray[j] && r->EnemyPathY[r->CurrentWaypoint + 1] == Yarray[j]) {
				check++;
			}
		}
		if (check < 1) {
			int ClosestWaypoint = 0;
			float d = 10000;
			for (int i = 0; i < Number_of_points; i++) {
				float a = fabs((double)r->data.xOrigin - Xarray[i]);
				float b = fabs((double)r->data.yOrigin - Yarray[i]);
				float c = a + b;
				if (c < d) {
					d = c;
					ClosestWaypoint = i;
					r->adjustingWaypoint = i;
					if (a - b < 0) {
						XorY = 2;
					}
					else {
						XorY = 1;
					}
				}
			}
			if (XorY == 1) {
				r->EnemyPathX[0] = r->data.xOrigin;
				r->EnemyPathY[0] = r->data.yOrigin;
				r->EnemyPathX[1] = Xarray[ClosestWaypoint];
				r->EnemyPathY[1] = r->data.yOrigin;
				r->EnemyPathX[2] = Xarray[ClosestWaypoint];
				r->EnemyPathY[2] = Yarray[ClosestWaypoint];
				r->CurrentWaypoint = 0;
			}
			else if (XorY == 2) {
				r->EnemyPathX[0] = r->data.xOrigin;
				r->EnemyPathY[0] = r->data.yOrigin;
				r->EnemyPathX[1] = r->data.xOrigin;
				r->EnemyPathY[1] = Yarray[ClosestWaypoint];
				r->EnemyPathX[2] = Xarray[ClosestWaypoint];
				r->EnemyPathY[2] = Yarray[ClosestWaypoint];

				r->CurrentWaypoint = 0;
			}
			r->state = Adjusting;
		}
	}
}

void reset_enemy_path(enemy* r) {
	for (int i = 0; i < 50; i++) {
		r->EnemyPathX[i] = Xarray[i];
		r->EnemyPathY[i] = Yarray[i];
	}
}


/*
NoEnvironmentalEffects,
IncreasedPhantomQuartz,
DecreasedPhantomQuartz,
FasterEnemies,
SlowerEnemies,
IncreasedTurretDamage,
DecreasedTurretDamage,
MoreHP,
LessHP,
IncreasedTurretAttackSpeed,
DecreasedTurretAttackSpeed,
NoPhantomQuartz
*/
void Env_eff_IncreasedTurretDamage(void){
	static float damage_increase[MAX_TURRET];
	static int level_check[MAX_TURRET];
	for (int i = 0; i < MAX_TURRET; i++) {
		if (turret[i].isActive) {
			if (Level[currentGameLevel].currentEffect == IncreasedTurretDamage && turret[i].Env_effect == No_Effect) {
				damage_increase[i] = turret[i].mod.damage * 0.2;
				turret[i].mod.damage += damage_increase[i];
				level_check[i] = turret[i].level;
				turret[i].Env_effect = Increased_damage;
			}
			else if (Level[currentGameLevel].currentEffect == IncreasedTurretDamage && turret[i].Env_effect == Increased_damage) {
				if (turret[i].level != level_check[i]) {
					turret[i].mod.damage -= damage_increase[i];
					damage_increase[i] = 0;
					turret[i].Env_effect = No_Effect;
				}
			}
			else if (Level[currentGameLevel].currentEffect != IncreasedTurretDamage && turret[i].Env_effect == Increased_damage) {
				turret[i].mod.damage -= damage_increase[i];
				damage_increase[i] = 0;
				turret[i].Env_effect = No_Effect;
			}
		}
	}
}

void Env_eff_DecreasedTurretDamage(void) {
	static float damage_decrease[MAX_TURRET];
	static int level_check[MAX_TURRET];
	for (int i = 0; i < MAX_TURRET; i++) {
		if (turret[i].isActive) {
			if (Level[currentGameLevel].currentEffect == DecreasedTurretDamage && turret[i].Env_effect == No_Effect) {
				if (turret[i].mod.damage == 1.0f) {
					turret[i].Env_effect = Decreased_damage;
					level_check[i] = turret[i].level;
					damage_decrease[i] = 0;
					continue;
				}
				damage_decrease[i] = turret[i].mod.damage * 0.2;
				turret[i].mod.damage -= damage_decrease[i];
				turret[i].Env_effect = Decreased_damage;
				level_check[i] = turret[i].level;
			}
			else if (Level[currentGameLevel].currentEffect == DecreasedTurretDamage && turret[i].Env_effect == Decreased_damage) {
				if (turret[i].level != level_check[i]) {
					turret[i].mod.damage += damage_decrease[i];
					damage_decrease[i] = 0;
					turret[i].Env_effect = No_Effect;
				}
			}
			else if (Level[currentGameLevel].currentEffect != DecreasedTurretDamage && turret[i].Env_effect == Decreased_damage) {
				turret[i].mod.damage += damage_decrease[i];
				damage_decrease[i] = 0;
				turret[i].Env_effect = No_Effect;
			}
		}
	}
}

void Env_eff_IncreasedTurretAttackSpeed(void) {
	static float atk_spd_increase[MAX_TURRET];
	static int level_check[MAX_TURRET];
	for (int i = 0; i < MAX_TURRET; i++) {
		if (Level[currentGameLevel].currentEffect == IncreasedTurretAttackSpeed && turret[i].Env_effect == No_Effect) {
			atk_spd_increase[i] = turret[i].mod.shoot_rate * 0.2;
			turret[i].mod.shoot_rate -= atk_spd_increase[i];
			level_check[i] = turret[i].level;
			turret[i].Env_effect = Increased_attack_speed;
		}
		else if (Level[currentGameLevel].currentEffect == IncreasedTurretAttackSpeed && turret[i].Env_effect == Increased_attack_speed) {
			if (turret[i].level != level_check[i]) {
				turret[i].mod.shoot_rate += atk_spd_increase[i];
				atk_spd_increase[i] = 0;
				turret[i].Env_effect = No_Effect;
			}
		}
		else if (Level[currentGameLevel].currentEffect != IncreasedTurretAttackSpeed && turret[i].Env_effect == Increased_attack_speed) {
			turret[i].mod.shoot_rate += atk_spd_increase[i];
			atk_spd_increase[i] = 0;
			turret[i].Env_effect = No_Effect;
		}
	}
}

void Env_eff_DecreasedTurretAttackSpeed(void) {
	static float atk_spd_decrease[MAX_TURRET];
	static float level_check[MAX_TURRET];
	for (int i = 0; i < MAX_TURRET; i++) {
		if (Level[currentGameLevel].currentEffect == DecreasedTurretAttackSpeed && turret[i].Env_effect == No_Effect) {
			atk_spd_decrease[i] = turret[i].mod.shoot_rate * 0.2;
			turret[i].mod.shoot_rate += atk_spd_decrease[i];
			level_check[i] = turret[i].level;
			turret[i].Env_effect = Decreased_attack_speed;
		}
		else if (Level[currentGameLevel].currentEffect == DecreasedTurretAttackSpeed && turret[i].Env_effect == Decreased_attack_speed) {
			if (turret[i].level != level_check[i]) {
				turret[i].mod.shoot_rate -= atk_spd_decrease[i];
				atk_spd_decrease[i] = 0;
				turret[i].Env_effect = No_Effect;
			}
		}
		else if (Level[currentGameLevel].currentEffect != DecreasedTurretAttackSpeed && turret[i].Env_effect == Decreased_attack_speed) {
			turret[i].mod.shoot_rate -= atk_spd_decrease[i];
			atk_spd_decrease[i] = 0;
			turret[i].Env_effect = No_Effect;
		}
	}
}

void Env_eff_More_HP(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (Enemy[i].env_eff == Applying && Enemy[i].health > 1) {
			int increase_hp = (int)Enemy[i].max_health * 0.3;
			Enemy[i].max_health += increase_hp;
			Enemy[i].health += increase_hp;
			Enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Less_HP(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (Enemy[i].env_eff == Applying && Enemy[i].health > 1) {
			int decrease_hp = Enemy[i].max_health * 0.2;
			Enemy[i].max_health -= decrease_hp;
			Enemy[i].health -= decrease_hp;
			Enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Faster_Enemies(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (Enemy[i].env_eff == Applying && Enemy[i].health > 1) {
			Enemy[i].speed *= 1.2;
			Enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Slower_Enemies(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (Enemy[i].env_eff == Applying && Enemy[i].health > 1) {
			Enemy[i].speed /= 1.2;
			Enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Increased_Phantom_quartz(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (Enemy[i].env_eff == Applying && Enemy[i].health > 1) {
			Enemy[i].points *= 1.2;
			Enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Decreased_Phantom_quartz(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (Enemy[i].env_eff == Applying && Enemy[i].health > 1) {
			Enemy[i].points /= 1.2;
			Enemy[i].env_eff = Effected;
		}
	}
}
void Env_eff_No_Phantom_quartz(void) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (Enemy[i].env_eff == Applying && Enemy[i].health > 1) {
			Enemy[i].points = 0;
			Enemy[i].env_eff = Effected;
		}
	}
}

void Environment_check(int CurrentGameLevel) {
	Env_eff_IncreasedTurretAttackSpeed();
	Env_eff_DecreasedTurretAttackSpeed();
	Env_eff_IncreasedTurretDamage();
	Env_eff_DecreasedTurretDamage();

	switch (Level[CurrentGameLevel].currentEffect) {
	case NoEnvironmentalEffects:
		break;
	case IncreasedPhantomQuartz:
		Env_eff_Increased_Phantom_quartz();
		break;
	case DecreasedPhantomQuartz:
		Env_eff_Decreased_Phantom_quartz();
		break;
	case FasterEnemies:
		Env_eff_Faster_Enemies();
		break;
	case SlowerEnemies:
		Env_eff_Slower_Enemies();
		break;
	case MoreHP:
		Env_eff_More_HP();
		break;
	case LessHP:
		Env_eff_Less_HP();
		break;
	case NoPhantomQuartz:
		Env_eff_No_Phantom_quartz();
		break;
	}
}

void Change_current_effect(int CurrentGameLevel) {
	EnvironmentalEffects a = CP_Random_RangeInt(0, 11);
	Level[CurrentGameLevel].currentEffect = a;
}

/*void movement_redone(enemy* r) {
	r->movement_timer += CP_System_GetDt();
	int fake_distance_covered = r->speed * timer;
	int distance_covered = fake_distance_covered - r->slowed_distance;
	int distance_between_start_and_end = fabs((double)(r->EnemyPathX[0] - r->EnemyPathX[r->pathPoints-1]) + (r->EnemyPathY[0] - r->EnemyPathY[r->pathPoints-1]));
	int Waypoint = 0;
	for (int i = 0; i < r->pathPoints; i++) {
		int distance_between_points = fabs((double)(r->EnemyPathX[i] - r->EnemyPathX[i + 1])) + fabs((double)(r->EnemyPathY[i] - r->EnemyPathY[i + 1]));
		if (distance_covered >= distance_between_points) {
			Waypoint++;
			distance_covered -= distance_between_points;
		}
	}
	if (Waypoint >= r->pathPoints) {
		r->CurrentWaypoint = Waypoint;
		r->data.xOrigin = r->EnemyPathX[r->pathPoints - 1];
		r->data.yOrigin = r->EnemyPathY[r->pathPoints - 1];
		return;
	}
	r->CurrentWaypoint = Waypoint;
	r->data.xOrigin = r->EnemyPathX[Waypoint];
	r->data.xOrigin = r->EnemyPathY[Waypoint];
	Direction enemy_dir = direction_to_next_point(r->EnemyPathX, r->EnemyPathY, r);
	Move_enemy(enemy_dir, r, distance_covered);
}

void Move_enemy(Direction enemy_dir, enemy* r,float distance_covered) {
	switch (enemy_dir) {
	case Up:
		r->data.yOrigin -= distance_covered;
		break;
	case Down:
		r->data.yOrigin += distance_covered;
		break;
	case Left:
		r->data.xOrigin -= distance_covered;
		break;
	case Right:
		r->data.xOrigin += distance_covered;
		break;
	}
}

void Update_slow_distance(enemy* r) {
	if (r->slow_amt < 1.0f) {
		r->slowed_distance += (1 - r->slow_amt) * r->speed * CP_System_GetDt();
	}
}*/