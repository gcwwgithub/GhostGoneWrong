#include "cprocessing.h"
#include <math.h>
#include "John.h"
#include "Gabriel.h"

void Basic_Ghost(Enemy* r) { // setup variable for basic ghost enemy
	r->health = 4;
	r->max_health = 4;
	r->speed = 30*scaling_factor;
	r->current_way_point = 0;
	r->data.x_origin = Xarray[0];
	r->data.y_origin = Yarray[0];
	r->enemy_width = game.grid_width;
	r->enemy_height = game.grid_height;
	r->angle = 0;
	r->type = kBasic;
	r->alpha = 255;
	r->data.object_type = kObjectCircle;
	r->data.width = game.grid_width;
	r->data.height = game.grid_width;
	r->state = kEnemyInactive;
	r->timer = 0;
	r->points = 25;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
	r->current_aim_state = 0;
	reset_enemy_path(r);
}

void Fast_Ghost_init(Enemy* r) { // setup variable for fast ghost enemy
	r->health = 2;
	r->max_health = 2;
	r->speed = 60 * scaling_factor;
	r->current_way_point = 0;
	r->data.x_origin = Xarray[0];
	r->data.y_origin = Yarray[0];
	r->enemy_width = game.grid_width;
	r->enemy_height = game.grid_height;
	r->angle = 0;
	r->type = kFastGhost;
	r->alpha = 255;
	r->data.object_type = kObjectCircle;
	r->data.width = game.grid_width;
	r->data.height = game.grid_width;
	r->state = kEnemyInactive;
	r->timer = 0;
	r->points = 25;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
	r->current_aim_state = 0;
	reset_enemy_path(r);
}

void Fat_Ghost_init(Enemy* r) {
	r->health = 30;
	r->max_health = 30;
	r->speed = 25 * scaling_factor;
	r->current_way_point = 0;
	r->data.x_origin = Xarray[0];
	r->data.y_origin = Yarray[0];
	r->enemy_width = game.grid_width;
	r->enemy_height = game.grid_height;
	r->angle = 0;
	r->type = kFatGhost;
	r->alpha = 255;
	r->data.object_type = kObjectCircle;
	r->data.width = game.grid_width;
	r->data.height = game.grid_width;
	r->state = kEnemyInactive;
	r->timer = 0;
	r->points = 250;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
	r->current_aim_state = 0;
	reset_enemy_path(r);
}

void Reaper_minion_init(Enemy* r) {
	int a = 0;
	for (int i = MAX_SPAWNING_ENEMIES; i < kMaxEnemies; i++) {
		if (a == 0) {
			if (enemy[i].state == kEnemyInactive) {
				Fast_Ghost_init(&enemy[i]);
				enemy[i].data.x_origin = Xarray[r->current_way_point];
				enemy[i].data.y_origin = Yarray[r->current_way_point];
				enemy[i].current_way_point = r->current_way_point;
				enemy[i].state = kEnemyMoving;
				enemies_left++;
				enemy[i].points = 0;
				a++;
			}
		}
		else if (a == 1) {
			if (enemy[i].state == kEnemyInactive) {
				Fast_Ghost_init(&enemy[i]);
				enemy[i].data.x_origin = Xarray[r->current_way_point + 1];
				enemy[i].data.y_origin = Yarray[r->current_way_point + 1];
				enemy[i].current_way_point = r->current_way_point + 1;
				enemy[i].state = kEnemyMoving;
				enemies_left++;
				enemy[i].points = 0;
				a++;
			}
		}
		else if (a == 2) {
			r->charges = kUsed;
			return;
		}
	}
}

void grimReaper_init(Enemy* r) {
	r->health = 50;
	r->max_health = 50;
	r->speed = 40 * scaling_factor;
	r->current_way_point = 0;
	r->data.x_origin = Xarray[0];
	r->data.y_origin = Yarray[0];
	r->enemy_width = game.grid_width;
	r->enemy_height = game.grid_height;
	r->angle = 0;
	r->type = kGrimReaper;
	r->alpha = 255;
	r->data.object_type = kObjectCircle;
	r->data.width = game.grid_width;
	r->data.height = game.grid_width;
	r->state = kEnemyInactive;
	r->timer = 0;
	r->points = 400;
	//for the freeze turret & enemy interaction
	r->slow_amt = 1;
	r->slow_timer = 0;
	r->current_aim_state = 0;
	r->charges = kCharges1;
	reset_enemy_path(r);
}

void Draw_enemy(Enemy* r) { //Draws the enemy
	EnemyAnimationState(r);
	switch (r->type) {
	case kBasic:
		CP_Image_DrawAdvanced(r->render_enemy, r->data.x_origin, r->data.y_origin, r->enemy_width, r->enemy_height, r->alpha, r->angle);
		r->timer += CP_System_GetDt();
		break;
	case kFastGhost:
		CP_Image_DrawAdvanced(r->render_enemy, r->data.x_origin, r->data.y_origin, r->enemy_width, r->enemy_height, r->alpha, r->angle);
		r->timer += CP_System_GetDt();
		break;
	}
}

void EnemyAnimationState(Enemy* r)
{
	int i = Check_state(r);
	if (i == 1) {
		if (r->timer >= 0.25) {
			r->state = kEnemyMoving;
		}
	}
	r->current_aim_state = i;

}
int Check_state(Enemy* r) {
	switch (r->state) {
	case kEnemyMoving:
		return 0;
	case kEnemyAdjusting:
		return 0;
	case kEnemyHurt:
		return 1;
	case kEnemyDeath:
		return 2;
	}
	return 0;
}

void enemy_move(Enemy* r, float Enemy_PathpointsX[], float Enemy_PathpointsY[], int number_of_points) { //Enemy movement
	float Speed = (r->speed) * r->slow_amt * CP_System_GetDt();
	update_point_num(Enemy_PathpointsX, Enemy_PathpointsY, r);
	if (r->current_way_point >= 2 && r->state == kEnemyAdjusting) {
		r->state = kEnemyMoving;
		reset_enemy_path(r);
		r->current_way_point = r->adjusting_waypoint;
	}
	if (r->current_way_point + 1 == number_of_points && r->state != kEnemyAdjusting) {
		if (r->state != kEnemyDeath && r->state != kEnemyInactive && r->state != kEnemyReached) {
			Level.health -= 10;
			InsertNewNodePortalEffect(&portal_enter_head_node, r->data.x_origin,
				r->data.y_origin, 0);
		}
		r->state = kEnemyReached;
	}

	Direction direction_now = direction_to_next_point(Enemy_PathpointsX, Enemy_PathpointsY, r);
	switch (direction_now) {
	case Up:
		r->data.y_origin -= Speed;
		break;
	case Down:
		r->data.y_origin += Speed;
		break;
	case Left:
		r->data.x_origin -= Speed;
		break;
	case Right:
		r->data.x_origin += Speed;
		break;
	case NoMove:
		break;
	}

}


Direction direction_to_next_point(float Enemy_PathpointsX[], float Enemy_PathpointsY[], Enemy* r) {   //Which direction to move depending on points
	float Xdistance_between_points = (Enemy_PathpointsX[r->current_way_point + 1] - Enemy_PathpointsX[r->current_way_point]);
	float Ydistance_between_points = (Enemy_PathpointsY[r->current_way_point + 1] - Enemy_PathpointsY[r->current_way_point]);
	if (r->state == kEnemyDeath || r->state == kEnemyReached) {
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

int update_point_num(float Enemy_PathpointsX[], float Enemy_PathpointsY[], Enemy* r) { //Update position to move towards next point 
	float covered_distanceX = (float)fabs((double)r->data.x_origin - (Enemy_PathpointsX[r->current_way_point]));
	float distance_between_pointsX = (float)fabs((double)Enemy_PathpointsX[r->current_way_point + 1] - (Enemy_PathpointsX[r->current_way_point]));
	float covered_distanceY = (float)fabs((double)r->data.y_origin - (Enemy_PathpointsY[r->current_way_point]));
	float distance_between_pointsY = (float)fabs((double)Enemy_PathpointsY[r->current_way_point + 1] - (Enemy_PathpointsY[r->current_way_point]));

	if (distance_between_pointsX <= covered_distanceX) {
		if (distance_between_pointsY <= covered_distanceY) {

			r->current_way_point++;
		}
	}
	return 0;
}



void EnemyDeath(Enemy* r) {  //function updates and checks for collision or death also what happens upon death
	for (int i = 0; i < kMaxProjectile; ++i) {
		if (proj[i].is_active) {
			Coordinates a = r->data;
			a.width *= 1;
			a.height *= 1;
			if (CollisionDetection(a, proj[i].data) == 1) {
				proj[i].is_active = 0;
				col_type_projectile(&proj[i]);
				if (r->state != kEnemyDeath)
				{
					if (proj[i].type != kTSlow)
					{
						r->health -= proj[i].mod.damage;
						r->state = kEnemyHurt;
						r->timer = 0;
					}

					InsertNewNodeBulletRadius(&bullet_radius_head_node, r->data.x_origin, r->data.y_origin, proj[i].type);
				}

			}
		}
	}

	if (r->health <= 0) {
		if (r->state == kEnemyHurt) {
			Level.phantom_quartz += r->points;
		}
		r->state = kEnemyDeath;

	}
	if (r->state == kEnemyDeath || r->state == kEnemyReached) {
		if (r->alpha >= 0) {
			r->alpha -= 10;
		}
		if (r->alpha <= 0) {
			r->state = kEnemyInactive;
			r->health = 0; //do not delete this line
			if (!r->is_token) {
				enemies_left--;
			}
			switch (r->type)
			{
			case kBasic:
				basicEnemyNum--;
				break;
			case kFastGhost:
				fastEnemyNum--;
				break;
			case kFatGhost:
				fatEnemyNum--;
				break;
			}
		}

	}
}

void Enemies_init(void) {
	Enemy_timer = 0;
	Enemy_node = NULL;
	wave_timer = 0;
	Array_count = 1;
	Number_of_points = 0;


	//test path
	for (int i = 0; i < kMaxEnemies; i++) {
		empty_enemy_init(&enemy[i]);
	}
}

void update_enemy(void) {
	Enemy_timer += CP_System_GetDt();
	for (int i = 0; i < kMaxEnemies; i++) {
		int spawn_timer = 2;
		if ((enemy[i].state == kEnemyInactive) && (enemy[i].health > 0) && ((int)Enemy_timer / spawn_timer <= kMaxEnemies)) {
			int state_check = 0;
			int b = (int)Enemy_timer;
			for (int j = 0; j < kMaxEnemies; j++) {
				if (enemy[j].state == kEnemyInactive) {
					state_check++;
				}
			}
			if (state_check == kMaxEnemies) {
				wave_timer = b - spawn_timer;
			}
			if (b - wave_timer >= spawn_timer) {
				enemy[i].state = kEnemyMoving;
				wave_timer = (int)Enemy_timer;
				InsertNewNodePortalEffect(&portal_spawn_head_node, enemy[i].data.x_origin,
					enemy[i].data.y_origin, 0);


			}
		}
		if (enemy[i].state == kEnemyInactive) //dont check if inactive
			continue;

		if (enemy[i].slow_timer > 0.f)
		{
			enemy[i].slow_timer -= CP_System_GetDt();
			if (enemy[i].slow_timer <= 0.f)
				enemy[i].slow_amt = 1.f;
		}

		Update_Path_Array(current_game_level);
		Check_pathAdjustment(&enemy[i]);
		enemy_move(&enemy[i], enemy[i].enemy_path_x, enemy[i].enemy_path_y, Number_of_points, current_game_level);
		EnemyDeath(&enemy[i], current_game_level);
		Reaper_ability(&enemy[i]);
		Environment_check(current_game_level);
		Current_wave_check(&enemy[i]);
		Power_Up_check(&enemy[i]);
	}
}
void draw_multiple_enemies(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].state == kEnemyInactive) {
			continue;
		}
		EnemyAnimationState(&enemy[i]);
		switch (enemy[i].type) {
		case kBasic:
			RenderImageFromSpriteSheetWithAlpha(basic_ghost_spritesheet, basic_ghost_spritesheet_array[enemy[i].current_aim_state],
				enemy[i].data.x_origin, enemy[i].data.y_origin, enemy[i].enemy_width, enemy[i].enemy_height, enemy[i].alpha);

			enemy[i].timer += CP_System_GetDt();
			break;
		case kFastGhost:
			RenderImageFromSpriteSheetWithAlpha(fast_ghost_spritesheet, fast_ghost_spritesheet_array[enemy[i].current_aim_state],
				enemy[i].data.x_origin, enemy[i].data.y_origin, enemy[i].enemy_width, enemy[i].enemy_height, enemy[i].alpha);
			enemy[i].timer += CP_System_GetDt();
			break;
		case kFatGhost:
			RenderImageFromSpriteSheetWithAlpha(fat_ghost_spritesheet, fat_ghost_spritesheet_array[enemy[i].current_aim_state],
				enemy[i].data.x_origin, enemy[i].data.y_origin, enemy[i].enemy_width, enemy[i].enemy_height, enemy[i].alpha);
			enemy[i].timer += CP_System_GetDt();
			break;
		case kGrimReaper:
			RenderImageFromSpriteSheetWithAlpha(grim_reaper_spritesheet, grim_reaper_spritesheet_array[enemy[i].current_aim_state],
				enemy[i].data.x_origin, enemy[i].data.y_origin, enemy[i].enemy_width, enemy[i].enemy_height, enemy[i].alpha);
			enemy[i].timer += CP_System_GetDt();
			break;
		}
		update_enemy_health_bar(&enemy[i]);

	}
}

void update_enemy_health_bar(Enemy* r)
{
	if (r->health > 0)
	{
		if (r->health <= 0)
		{
			r->health = 0;
		}

		float newWidth = r->health / r->max_health;
		CP_Settings_Fill(COLOR_RED);
		CP_Graphics_DrawRect(r->data.x_origin - r->enemy_width, r->data.y_origin - r->enemy_height, r->enemy_width * 2, r->enemy_height / 4);

		CP_Settings_Fill(COLOR_GREEN);
		CP_Graphics_DrawRect(r->data.x_origin - r->enemy_width, r->data.y_origin - r->enemy_height, r->enemy_width * 2 * newWidth, r->enemy_height / 4);
	}


}

void Update_Path_Array(void) {
	int nextPathRow = 1, nextPathCol = 1;
	for (int currentCost = 1; currentCost <= Level.grid[Level.exit_row][Level.exit_col].cost; currentCost++) {
		for (int currentRow = 0; currentRow < level_grid_rows; currentRow++) {
			for (int currentCol = 0; currentCol < level_grid_cols; currentCol++) {
				if (Level.grid[currentRow][currentCol].cost == currentCost && (Level.grid[currentRow][currentCol].type == kPath || Level.grid[currentRow][currentCol].type == kExit)) {
					nextPathRow = currentRow;
					nextPathCol = currentCol;
					break;
				}
			}
		}
		Xarray[Array_count] = (float)(game.x_origin + game.grid_width * (0.5 + nextPathCol));
		Yarray[Array_count] = (float)(game.y_origin + game.grid_height * (0.5 + nextPathRow));
		Array_count++;
	}
	Number_of_points = Array_count;
	Array_count = 1;
}


void Reaper_ability(Enemy* r) {
	if (r->type == kGrimReaper) {
		if (r->health <= 0.5 * r->max_health) {
			if (r->charges == kCharges1) {
				Reaper_minion_init(r);
			}
		}
	}
}

void empty_enemy_init(Enemy* r) {
	r->health = 0;
	r->max_health = 0;
	r->speed = 0;
	r->current_way_point = 0;
	r->data.x_origin = 0;
	r->data.y_origin = 0;
	r->enemy_width = game.grid_width;
	r->enemy_height = game.grid_height;
	r->angle = 0;
	r->type = kBasic;
	r->alpha = 255;
	r->data.object_type = kObjectCircle;
	r->data.width = game.grid_width;
	r->data.height = game.grid_width;
	r->state = kEnemyInactive;
	r->timer = 0;
	r->points = 0;
	//for the freeze turret & enemy interaction
	r->slow_amt = 0;
	r->slow_timer = 0;
	r->current_aim_state = 0;
	r->charges = kUsed;
	r->env_eff = Applying;
	//r->Enemy_pow_up.Less_HP = 0;
	//r->Enemy_pow_up.More_Points = 0;
	//r->Enemy_pow_up.SpeedDown = 0;
	r->wave_pow_up_is_active = 0;
	r->is_token = 0;
	//int pathPoints;
	//float slowed_distance;
	for (int i = 0; i < 50; i++) {
		r->enemy_path_x[i] = 0;
		r->enemy_path_y[i] = 0;
	}
	for (int j = 0; j < 3; j++) {
		r->enemy_pow_up[j] = 0;
	}
}




void wave_enemy_init(int Basic_Ghost_count, int Fast_Ghost_count, int Fat_Ghost_count, int Grim_Reaper_count, LevelData E_Level) {
	Enemy_timer = 0;
	Enemy_node = NULL;
	wave_timer = 0;
	Xarray[0] = (float)(game.x_origin + game.grid_width * (0.5 + E_Level.spawn_col));
	Yarray[0] = (float)(game.y_origin + game.grid_height * (0.5 + E_Level.spawn_row));

	for (int i = 0; i < kMaxEnemies; i++) {
		empty_enemy_init(&enemy[i]);
	}
	int a = Basic_Ghost_count + Fast_Ghost_count;
	int b = a + Fat_Ghost_count;
	int c = b + Grim_Reaper_count;
	Xarray[0] = (float)(game.x_origin + game.grid_width * (0.5 + E_Level.spawn_col));
	Yarray[0] = (float)(game.y_origin + game.grid_height * (0.5 + E_Level.spawn_row));
	for (int i = 0; i < Basic_Ghost_count; i++) {
		Basic_Ghost(&enemy[i]);
	}
	for (int i = Basic_Ghost_count; i < a && i < MAX_SPAWNING_ENEMIES; i++) {
		Fast_Ghost_init(&enemy[i]);
	}
	for (int i = a; i < b && i < MAX_SPAWNING_ENEMIES; i++) {
		Fat_Ghost_init(&enemy[i]);
	}
	for (int i = b; i < MAX_SPAWNING_ENEMIES && i < c; i++) {
		grimReaper_init(&enemy[i]);
	}
}

void Reset_enemies(void) {
	if (current_game_state == kBuilding) {
		Update_Path_Array(current_game_level);
		if (building_time > 0.05f && Level.current_wave < kMaxNumberOfWave) {
			int BasicCount = Level.wave_enemies[Level.current_wave][kBasic];
			int FastCount = Level.wave_enemies[Level.current_wave][kFastGhost];
			int FatCount = Level.wave_enemies[Level.current_wave][kFatGhost];
			int ReaperCount = Level.wave_enemies[Level.current_wave][kGrimReaper];
			wave_enemy_init(BasicCount, FastCount, FatCount, ReaperCount, Level);
			enemies_left = BasicCount + FastCount + FatCount + ReaperCount;
		}
	}
}


void Check_pathAdjustment(Enemy* r) {
	int XorY = 0;
	int check = 0;
	if (r->state != kEnemyInactive && r->state != kEnemyDeath && r->state != kEnemyReached) {
		for (int j = 0; j < Number_of_points; j++) {
			if (r->state != kEnemyAdjusting && (r->enemy_path_x[r->current_way_point + 1] == Xarray[j] && r->enemy_path_y[r->current_way_point + 1] == Yarray[j])) {
				check++;
			}
			else if (r->state == kEnemyAdjusting) {
				if (r->enemy_path_x[2] == Xarray[j] && r->enemy_path_y[2] == Yarray[j]) {
					check++;
				}
			}
		}
		if (check < 1) {
			int ClosestWaypoint = 0;
			float d = 10000;
			for (int i = 0; i < Number_of_points; i++) {
				float a = (float)fabs((double)r->data.x_origin - Xarray[i]);
				float b = (float)fabs((double)r->data.y_origin - Yarray[i]);
				float c = a + b;
				if (c < d) {
					d = c;
					ClosestWaypoint = i;
					r->adjusting_waypoint = i;
					if (a - b < 0) {
						XorY = 2;
					}
					else {
						XorY = 1;
					}
				}
			}
			if (XorY == 1) {
				r->enemy_path_x[0] = r->data.x_origin;
				r->enemy_path_y[0] = r->data.y_origin;
				r->enemy_path_x[1] = Xarray[ClosestWaypoint];
				r->enemy_path_y[1] = r->data.y_origin;
				r->enemy_path_x[2] = Xarray[ClosestWaypoint];
				r->enemy_path_y[2] = Yarray[ClosestWaypoint];
				r->current_way_point = 0;
			}
			else if (XorY == 2) {
				r->enemy_path_x[0] = r->data.x_origin;
				r->enemy_path_y[0] = r->data.y_origin;
				r->enemy_path_x[1] = r->data.x_origin;
				r->enemy_path_y[1] = Yarray[ClosestWaypoint];
				r->enemy_path_x[2] = Xarray[ClosestWaypoint];
				r->enemy_path_y[2] = Yarray[ClosestWaypoint];

				r->current_way_point = 0;
			}
			r->state = kEnemyAdjusting;
		}
	}
}

void reset_enemy_path(Enemy* r) {
	for (int i = 0; i < 50; i++) {
		r->enemy_path_x[i] = Xarray[i];
		r->enemy_path_y[i] = Yarray[i];
	}
}



void Env_eff_IncreasedTurretDamage(void) {
	static float damage_increase[kMaxTurret];
	static int level_check[kMaxTurret];
	for (int i = 0; i < kMaxTurret; i++) {
		if (turret[i].is_active) {
			if (Level.current_effect == kIncreasedTurretDamage && turret[i].env_effects == kTEnvironmentNoEffect) {
				damage_increase[i] = turret[i].mod.damage * 0.2f;
				turret[i].mod.damage += damage_increase[i];
				level_check[i] = turret[i].level;
				turret[i].env_effects = kTEnvironmentIncreasedDamage;
			}
			else if (Level.current_effect == kIncreasedTurretDamage && turret[i].env_effects == kTEnvironmentIncreasedDamage) {
				if (turret[i].level != level_check[i]) {
					turret[i].mod.damage -= damage_increase[i];
					damage_increase[i] = 0;
					turret[i].env_effects = kTEnvironmentNoEffect;
				}
			}
			else if (Level.current_effect != kIncreasedTurretDamage && turret[i].env_effects == kTEnvironmentIncreasedDamage) {
				turret[i].mod.damage -= damage_increase[i];
				damage_increase[i] = 0;
				turret[i].env_effects = kTEnvironmentNoEffect;
			}
		}
	}
}

void Env_eff_DecreasedTurretDamage(void) {
	static float damage_decrease[kMaxTurret];
	static int level_check[kMaxTurret];
	for (int i = 0; i < kMaxTurret; i++) {
		if (turret[i].is_active) {
			if (Level.current_effect == kDecreasedTurretDamage && turret[i].env_effects == kTEnvironmentNoEffect) {
				if (turret[i].mod.damage == 1.0f) {
					turret[i].env_effects = kTEnvironmentDecreasedDamage;
					level_check[i] = turret[i].level;
					damage_decrease[i] = 0;
					continue;
				}
				damage_decrease[i] = turret[i].mod.damage * 0.2f;
				turret[i].mod.damage -= damage_decrease[i];
				turret[i].env_effects = kTEnvironmentDecreasedDamage;
				level_check[i] = turret[i].level;
			}
			else if (Level.current_effect == kDecreasedTurretDamage && turret[i].env_effects == kTEnvironmentDecreasedDamage) {
				if (turret[i].level != level_check[i]) {
					turret[i].mod.damage += damage_decrease[i];
					damage_decrease[i] = 0;
					turret[i].env_effects = kTEnvironmentNoEffect;
				}
			}
			else if (Level.current_effect != kDecreasedTurretDamage && turret[i].env_effects == kTEnvironmentDecreasedDamage) {
				turret[i].mod.damage += damage_decrease[i];
				damage_decrease[i] = 0;
				turret[i].env_effects = kTEnvironmentNoEffect;
			}
		}
	}
}

void Env_eff_IncreasedTurretAttackSpeed(void) {
	static float atk_spd_increase[kMaxTurret];
	static int level_check[kMaxTurret];
	for (int i = 0; i < kMaxTurret; i++) {
		if (Level.current_effect == kIncreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmentNoEffect) {
			atk_spd_increase[i] = turret[i].mod.shoot_rate * 0.2f;
			turret[i].mod.shoot_rate -= atk_spd_increase[i];
			level_check[i] = turret[i].level;
			turret[i].env_effects = kTEnvironmentIncreasedAttackSpeed;
		}
		else if (Level.current_effect == kIncreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmentIncreasedAttackSpeed) {
			if (turret[i].level != level_check[i]) {
				turret[i].mod.shoot_rate += atk_spd_increase[i];
				atk_spd_increase[i] = 0;
				turret[i].env_effects = kTEnvironmentNoEffect;
			}
		}
		else if (Level.current_effect != kIncreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmentIncreasedAttackSpeed) {
			turret[i].mod.shoot_rate += atk_spd_increase[i];
			atk_spd_increase[i] = 0;
			turret[i].env_effects = kTEnvironmentNoEffect;
		}
	}
}

void Env_eff_DecreasedTurretAttackSpeed(void) {
	static float atk_spd_decrease[kMaxTurret];
	static float level_check[kMaxTurret];
	for (int i = 0; i < kMaxTurret; i++) {
		if (Level.current_effect == kDecreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmentNoEffect) {
			atk_spd_decrease[i] = turret[i].mod.shoot_rate * 0.2f;
			turret[i].mod.shoot_rate += atk_spd_decrease[i];
			level_check[i] = (float)turret[i].level;
			turret[i].env_effects = kTEnvironmnetDecreasedAttackSpeed;
		}
		else if (Level.current_effect == kDecreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmnetDecreasedAttackSpeed) {
			if (turret[i].level != level_check[i]) {
				turret[i].mod.shoot_rate -= atk_spd_decrease[i];
				atk_spd_decrease[i] = 0;
				turret[i].env_effects = kTEnvironmentNoEffect;
			}
		}
		else if (Level.current_effect != kDecreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmnetDecreasedAttackSpeed) {
			turret[i].mod.shoot_rate -= atk_spd_decrease[i];
			atk_spd_decrease[i] = 0;
			turret[i].env_effects = kTEnvironmentNoEffect;
		}
	}
}

void Env_eff_More_HP(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == Applying && enemy[i].health > 1) {
			int increase_hp = (int)(enemy[i].max_health * 0.3);
			enemy[i].max_health += increase_hp;
			enemy[i].health += increase_hp;
			enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Less_HP(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == Applying && enemy[i].health > 1) {
			int decrease_hp = (int)(enemy[i].max_health * 0.2);
			enemy[i].max_health -= decrease_hp;
			enemy[i].health -= decrease_hp;
			enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Faster_Enemies(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == Applying && enemy[i].health > 1) {
			enemy[i].speed *= 1.2f;
			enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Slower_Enemies(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == Applying && enemy[i].health > 1) {
			enemy[i].speed /= 1.2f;
			enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Increased_Phantom_quartz(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == Applying && enemy[i].health > 1) {
			enemy[i].points = (int)(enemy[i].points*1.2);
			enemy[i].env_eff = Effected;
		}
	}
}

void Env_eff_Decreased_Phantom_quartz(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == Applying && enemy[i].health > 1) {
			enemy[i].points /= (int)(enemy[i].points / 1.2);
			enemy[i].env_eff = Effected;
		}
	}
}
void Env_eff_No_Phantom_quartz(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == Applying && enemy[i].health > 1) {
			enemy[i].points = 0;
			enemy[i].env_eff = Effected;
		}
	}
}

void Environment_check(void) {
	Env_eff_IncreasedTurretAttackSpeed();
	Env_eff_DecreasedTurretAttackSpeed();
	Env_eff_IncreasedTurretDamage();
	Env_eff_DecreasedTurretDamage();

	switch (Level.current_effect) {
	case kNoEnvironmentalEffects:
		break;
	case kIncreasedPhantomQuartz:
		Env_eff_Increased_Phantom_quartz();
		break;
	case kDecreasedPhantomQuartz:
		Env_eff_Decreased_Phantom_quartz();
		break;
	case kFasterEnemies:
		Env_eff_Faster_Enemies();
		break;
	case kSlowerEnemies:
		Env_eff_Slower_Enemies();
		break;
	case kMoreHP:
		Env_eff_More_HP();
		break;
	case kLessHP:
		Env_eff_Less_HP();
		break;
	case kNoPhantomQuartz:
		Env_eff_No_Phantom_quartz();
		break;
	}
}

void Current_wave_check(Enemy* r) {
	if (r->wave_pow_up_is_active == 0) {
		int a = Level.current_wave;
		r->max_health = (float)(r->max_health * (1 + (0.2 * a)));
		r->health = (float)(r->health*(1 + (0.2 * a)));
		r->speed += 1 * a;
		r->wave_pow_up_is_active = 1;
	}
}

void Power_Up_check(Enemy* r) {
	if (r->enemy_pow_up[0] == 0) {
		r->health *= (1 - (Level.current_power_up_level.reduce_enemy_health * 0.05f));
		r->enemy_pow_up[0] = 1;
	}
	if (r->enemy_pow_up[1] == 0) {
		r->speed *= (1 - (Level.current_power_up_level.reduce_enemy_speed * 0.05f));
		r->enemy_pow_up[1] = 1;
	}
	if (r->enemy_pow_up[2] == 0) {
		r->points = (int)(r->points *( 1+(Level.current_power_up_level.more_phantom_quartz *0.10)));
		r->enemy_pow_up[2] = 1;
	}
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