/*!
All content ? 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       John.h
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author  Chiok Wei Wen Gabriel (chiok.w@digipen.edu) Did all animation functions
			Ng Zheng Wei (zhengwei.ng@digipen.edu) Helped with pathfinding
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu) Helped adjust functions for turret and projectile variables
@course     CSD 1400
@section    C
@date       4/12/2021
@brief    	Contains function definition for
EnemyAnimationState, CheckEnemyState, ReaperAbility, EnemyDeath,

RenderAllEnemies, RenderEnemyHealth

ResetEnemyInit, EnemyWavePowUp, EnemyVariableChangeforPowUps,
EnemyInitforWaves, BasicGhostInit, FastGhostInit, FatGhostInit, GrimReaperInit, ReaperMinionInit, EmptyEnemyInit

EnemyMovement, UpdateEnemyPathWaypointArray, CheckEnemyPathAdjustment, ResetEnemyPathWaypoints, DirectionToNextPoint, UpdateEnemyCurrentWaypoint

EnvironmentEffCheck, EnvEffMoreHP, EnvEffLessHP, EnvEffFasterEnemies, EnvEffSlowerEnemies, EnvEffIncreasedPhantomQuartz,
EnvEffDecreasedPhantomQuartz, EnvEffNoPhantomQuartz, EnvEffIncreasedTurretDamage, EnvEffDecreasedTurretDamage, EnvEffDecreasedTurretAttackSpeed, EnvEffIncreasedTurretAttackSpeed
*//*__________________________________________________________________________*/

#include <math.h>
#include "game.h"
#include "John.h"
#include "Gabriel.h"

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions initialises all required variables for all enemies
@param		void
@return		void
*//*_____________________________________________________________*/
void Enemies_init(void) {
	enemy_timer = 0;
	Enemy_node = NULL;
	wave_timer = 0;
	array_counter = 1;
	number_of_points = 0;

	//test path
	for (int i = 0; i < kMaxEnemies; i++) {
		EmptyEnemyInit(&enemy[i]);
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author	
@brief		This functions initialises variables for a Basic Ghost Enemy
@param		Pointer to a enemy struct
@return		void
*//*_____________________________________________________________*/
void BasicGhostInit(Enemy* r) {
	r->health = 4;
	r->max_health = 4;
	r->speed = 30*scaling_factor;
	r->current_way_point = 0;
	r->data.x_origin = global_enemy_path_X_array[0];
	r->data.y_origin = global_enemy_path_Y_array[0];
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
	ResetEnemyPathWaypoints(r);
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions initialises variables for a Fast Ghost Enemy
@param		Pointer to a enemy struct
@return		void
*//*_____________________________________________________________*/
void FastGhostInit(Enemy* r) { 
	r->health = 2;
	r->max_health = 2;
	r->speed = 60 * scaling_factor;
	r->current_way_point = 0;
	r->data.x_origin = global_enemy_path_X_array[0];
	r->data.y_origin = global_enemy_path_Y_array[0];
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
	ResetEnemyPathWaypoints(r);
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions initialises variables for a Fat Ghost Enemy
@param		Pointer to a enemy struct
@return		void
*//*_____________________________________________________________*/
void FatGhostInit(Enemy* r) {
	r->health = 30;
	r->max_health = 30;
	r->speed = 25 * scaling_factor;
	r->current_way_point = 0;
	r->data.x_origin = global_enemy_path_X_array[0];
	r->data.y_origin = global_enemy_path_Y_array[0];
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
	ResetEnemyPathWaypoints(r);
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions initialises variables for a Grim Reaper Minion Summons Enemy
@param		Pointer to a enemy struct
@return		void
*//*_____________________________________________________________*/
void ReaperMinionInit(Enemy* r) {
	int which_minion_init = 0;
	for (int i = MAX_SPAWNING_ENEMIES; i < kMaxEnemies; i++) {
		if (which_minion_init == 0) {
			if (enemy[i].state == kEnemyInactive) {
				FastGhostInit(&enemy[i]);
				enemy[i].data.x_origin = global_enemy_path_X_array[r->current_way_point];
				enemy[i].data.y_origin = global_enemy_path_Y_array[r->current_way_point];
				enemy[i].current_way_point = r->current_way_point;
				enemy[i].state = kEnemyMoving;
				enemies_left++;
				enemy[i].points = 0;
				which_minion_init++;
			}
		}
		else if (which_minion_init == 1) {
			if (enemy[i].state == kEnemyInactive) {
				FastGhostInit(&enemy[i]);
				enemy[i].data.x_origin = global_enemy_path_X_array[r->current_way_point + 1];
				enemy[i].data.y_origin = global_enemy_path_Y_array[r->current_way_point + 1];
				enemy[i].current_way_point = r->current_way_point + 1;
				enemy[i].state = kEnemyMoving;
				enemies_left++;
				enemy[i].points = 0;
				which_minion_init++;
			}
		}
		else if (which_minion_init == 2) {
			r->charges = kUsed;
			return;
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions initialises variables for a Grim Reaper Enemy
@param		Pointer to a enemy struct
@return		void
*//*_____________________________________________________________*/
void GrimReaperInit(Enemy* r) {
	r->health = 50;
	r->max_health = 50;
	r->speed = 40 * scaling_factor;
	r->current_way_point = 0;
	r->data.x_origin = global_enemy_path_X_array[0];
	r->data.y_origin = global_enemy_path_Y_array[0];
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
	ResetEnemyPathWaypoints(r);
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions initialises a enemy variable with 0
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void EmptyEnemyInit(Enemy* r) {
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
	r->env_eff = kApplying;
	r->wave_pow_up_is_active = 0;
	r->is_token = 0;
	for (int i = 0; i < 50; i++) {
		r->enemy_path_x[i] = 0;
		r->enemy_path_y[i] = 0;
	}
	for (int j = 0; j < 3; j++) {
		r->enemy_pow_up[j] = 0;
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions initialises all enemies during building phase and prepares all variables
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void EnemyInitforWaves(int basic_ghost_count, int fast_ghost_count, int fat_ghost_count, int grim_reaper_count, LevelData e_level) {
	enemy_timer = 0;
	Enemy_node = NULL;
	wave_timer = 0;
	global_enemy_path_X_array[0] = (float)(game.x_origin + game.grid_width * (0.5 + e_level.spawn_col));
	global_enemy_path_Y_array[0] = (float)(game.y_origin + game.grid_height * (0.5 + e_level.spawn_row));

	for (int i = 0; i < kMaxEnemies; i++) {
		EmptyEnemyInit(&enemy[i]);
	}
	int a = basic_ghost_count + fast_ghost_count;
	int b = a + fat_ghost_count;
	int c = b + grim_reaper_count;
	global_enemy_path_X_array[0] = (float)(game.x_origin + game.grid_width * (0.5 + e_level.spawn_col));
	global_enemy_path_Y_array[0] = (float)(game.y_origin + game.grid_height * (0.5 + e_level.spawn_row));
	for (int i = 0; i < basic_ghost_count; i++) {
		BasicGhostInit(&enemy[i]);
	}
	for (int i = basic_ghost_count; i < a && i < MAX_SPAWNING_ENEMIES; i++) {
		FastGhostInit(&enemy[i]);
	}
	for (int i = a; i < b && i < MAX_SPAWNING_ENEMIES; i++) {
		FatGhostInit(&enemy[i]);
	}
	for (int i = b; i < MAX_SPAWNING_ENEMIES && i < c; i++) {
		GrimReaperInit(&enemy[i]);
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions checks the level wave for required enemies and initialises them
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void ResetEnemyInit(void) {
	if (current_game_state == kBuilding) {
		UpdateEnemyPathWaypointArray();
		if (building_time > 0.05f && level.current_wave < kMaxNumberOfWave) {
			int BasicCount = level.wave_enemies[level.current_wave][kBasic];
			int FastCount = level.wave_enemies[level.current_wave][kFastGhost];
			int FatCount = level.wave_enemies[level.current_wave][kFatGhost];
			int ReaperCount = level.wave_enemies[level.current_wave][kGrimReaper];
			EnemyInitforWaves(BasicCount, FastCount, FatCount, ReaperCount, level);
			enemies_left = BasicCount + FastCount + FatCount + ReaperCount;
		}
	}
}


/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author	Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@brief		This functions renders all enemies according to their state and type from the array of enemy structs as long as they are active
@param		void
@return		void
*//*_____________________________________________________________*/
void RenderAllEnemies(void) {
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
		RenderEnemyHealth(&enemy[i]);

	}
}

/*!
@author     Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@co-author	Lim Jing Rui John (l.jingruijohn@digipen.edu)
@brief		This functions renders enemy health over max health
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void RenderEnemyHealth(Enemy* r)
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

/*!
@author     Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@co-author
@brief		This functions changes enemy state from Hurting back to Moving after 0.25 secs
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void EnemyAnimationState(Enemy* r)
{
	int EnemyState = CheckEnemyState(r);
	if (EnemyState == 1) {
		if (r->timer >= 0.25) {
			r->state = kEnemyMoving;
		}
	}
	r->current_aim_state = EnemyState;

}
/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author	Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@brief		This functions checks enemy state and returns a interger of 0 if moving or adjusting a 1 if its Hurt and a 2 if its dead
@param		Pointer to enemy struct
@return		Integer according to state
*//*_____________________________________________________________*/
int CheckEnemyState(Enemy* r) {
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

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions moves the enemy using waypoints where it will check current waypoint and move in the direction towards the next waypoint and will update current waypoint when distance
			covered is larger than distance between waypoints
			It also ensures that the enemy will change states upon reaching the portal
@param		1st Param Pointer to enemy struct
			2nd Param Enemy Waypoints in X coordinates
			3rd Param Enemy Waypoints in Y coordinates
			4th Param Number of waypoints in X and Y arrays for enemy waypoints
@return		void
*//*_____________________________________________________________*/
void EnemyMovement(Enemy* r, float enemy_pathpoints_X[], float enemy_pathpoints_Y[], int number_of_waypoints) { //Enemy movement
	float Speed = (r->speed) * r->slow_amt * CP_System_GetDt();
	UpdateEnemyCurrentWaypoint(enemy_pathpoints_X, enemy_pathpoints_Y, r);
	if (r->current_way_point >= 2 && r->state == kEnemyAdjusting) {
		r->state = kEnemyMoving;
		ResetEnemyPathWaypoints(r);
		r->current_way_point = r->adjusting_waypoint;
	}
	if (r->current_way_point + 1 == number_of_waypoints && r->state != kEnemyAdjusting) {
		if (r->state != kEnemyDeath && r->state != kEnemyInactive && r->state != kEnemyReached) {
			level.health -= 10;
			InsertNewNodePortalEffect(&portal_enter_head_node, r->data.x_origin,
				r->data.y_origin, 0);
		}
		r->state = kEnemyReached;
	}

	Direction direction_now = DirectionToNextPoint(enemy_pathpoints_X, enemy_pathpoints_Y, r);
	switch (direction_now) {
	case kUp:
		r->data.y_origin -= Speed;
		break;
	case kDown:
		r->data.y_origin += Speed;
		break;
	case kLeft:
		r->data.x_origin -= Speed;
		break;
	case kRight:
		r->data.x_origin += Speed;
		break;
	case kNoMove:
		break;
	}

}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions checks which direction the enemy needs to move towards between waypoints
@param		1st Param Enemy Waypoints in X coordinates
			2nd Param Enemy Waypoints in Y coordinates
			3rd Param Pointer to enemy struct
@return		Direction enum
*//*_____________________________________________________________*/
Direction DirectionToNextPoint(float enemy_Pathpoints_X[], float enemy_pathpoints_Y[], Enemy* r) {   //Which direction to move depending on points
	float Xdistance_between_points = (enemy_Pathpoints_X[r->current_way_point + 1] - enemy_Pathpoints_X[r->current_way_point]);
	float Ydistance_between_points = (enemy_pathpoints_Y[r->current_way_point + 1] - enemy_pathpoints_Y[r->current_way_point]);
	if (r->state == kEnemyDeath || r->state == kEnemyReached) {
		return kNoMove;
	}
	if (Xdistance_between_points == 0) {
		if (Ydistance_between_points < 0) {
			return kUp; //Upwards movement
		}
		else {
			return kDown; //Downwards movement
		}
	}
	else if (Ydistance_between_points == 0) {
		if (Xdistance_between_points < 0) {
			return kLeft; //Left movement
		}
		else {
			return kRight; //Right movement
		}
	}
	return 0;
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions updates enemy current way point when the distance covered is larger than the distance between the waypoints
@param		1st Param Enemy Waypoints in X coordinates
			2nd Param Enemy Waypoints in Y coordinates
			3rd Param Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void UpdateEnemyCurrentWaypoint(float enemy_pathpoints_X[], float enemy_pathpoints_Y[], Enemy* r) { //Update position to move towards next point 
	float covered_distanceX = (float)fabs((double)r->data.x_origin - (enemy_pathpoints_X[r->current_way_point]));
	float distance_between_pointsX = (float)fabs((double)enemy_pathpoints_X[r->current_way_point + 1] - (enemy_pathpoints_X[r->current_way_point]));
	float covered_distanceY = (float)fabs((double)r->data.y_origin - (enemy_pathpoints_Y[r->current_way_point]));
	float distance_between_pointsY = (float)fabs((double)enemy_pathpoints_Y[r->current_way_point + 1] - (enemy_pathpoints_Y[r->current_way_point]));

	if (distance_between_pointsX <= covered_distanceX) {
		if (distance_between_pointsY <= covered_distanceY) {

			r->current_way_point++;
		}
	}
	return;
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author	Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		This functions updates the global path array for the X and Y enemy paths so that it changes when the current path is blocked by a turret
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void UpdateEnemyPathWaypointArray(void) {
	int nextPathRow = 1, nextPathCol = 1;
	for (int currentCost = 1; currentCost <= level.grid[level.exit_row][level.exit_col].cost; currentCost++) {
		for (int currentRow = 0; currentRow < level_grid_rows; currentRow++) {
			for (int currentCol = 0; currentCol < level_grid_cols; currentCol++) {
				if (level.grid[currentRow][currentCol].cost == currentCost && (level.grid[currentRow][currentCol].type == kPath || level.grid[currentRow][currentCol].type == kExit)) {
					nextPathRow = currentRow;
					nextPathCol = currentCol;
					break;
				}
			}
		}
		global_enemy_path_X_array[array_counter] = (float)(game.x_origin + game.grid_width * (0.5 + nextPathCol));
		global_enemy_path_Y_array[array_counter] = (float)(game.y_origin + game.grid_height * (0.5 + nextPathRow));
		array_counter++;
	}
	number_of_points = array_counter;
	array_counter = 1;
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author	Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		This functions checks for collision between turret projectile and enemy and changes the variables accordingly in enemy struct
			It also checks for Death and updates enemy state accordingly and reduces alpha upon death or reached to create a dying effect
			It also changes the number of enemies left accordingly when a enemy state is changed to death
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void EnemyDeath(Enemy* r) {  //function updates and checks for collision or death also what happens upon death
	for (int i = 0; i < kMaxProjectile; ++i) {
		if (proj[i].is_active) {
			Coordinates a = r->data;
			a.width *= 1;
			a.height *= 1;
			if (CollisionDetection(a, proj[i].data) == 1) {
				proj[i].is_active = 0;
				ColTypeProjectile(&proj[i]);
				if (r->state != kEnemyDeath)
				{
					if (proj[i].type != kTSlow)
					{
						r->health -= proj[i].mod.damage;
						r->state = kEnemyHurt;
						r->timer = 0;
						CP_Sound_PlayAdvanced(hit_sfx, sfx_volume, 1.0f, FALSE, CP_SOUND_GROUP_0);
					}

					InsertNewNodeBulletRadius(&bullet_radius_head_node, r->data.x_origin, r->data.y_origin, proj[i].type);
				}

			}
		}
	}

	if (r->health <= 0) {
		if (r->state == kEnemyHurt) {
			level.phantom_quartz += r->points;
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
				basic_enemy_count--;
				break;
			case kFastGhost:
				fast_enemy_count--;
				break;
			case kFatGhost:
				fat_enemy_count--;
				break;
			}
		}

	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author	
@brief		This functions activates the reaper summons when its health is 50%
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void ReaperAbility(Enemy* r) {
	if (r->type == kGrimReaper) {
		if ((r->health <= 0.5 * r->max_health)&&(r->health>0)) {
			if (r->charges == kCharges1) {
				ReaperMinionInit(r);
			}
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions checks if the enemy path no longer fits to global path and changes enemy to adjust towards the closest waypoint
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void CheckEnemyPathAdjustment(Enemy* r) {
	int XorY = 0;
	int check = 0;
	if (r->state != kEnemyInactive && r->state != kEnemyDeath && r->state != kEnemyReached) {
		for (int j = 0; j < number_of_points; j++) {
			if (r->state != kEnemyAdjusting && (r->enemy_path_x[r->current_way_point + 1] == global_enemy_path_X_array[j] && r->enemy_path_y[r->current_way_point + 1] == global_enemy_path_Y_array[j])) {
				check++;
			}
			else if (r->state == kEnemyAdjusting) {
				if (r->enemy_path_x[2] == global_enemy_path_X_array[j] && r->enemy_path_y[2] == global_enemy_path_Y_array[j]) {
					check++;
				}
			}
		}
		if (check < 1) {
			int ClosestWaypoint = 0;
			float d = 10000;
			for (int i = 0; i < number_of_points; i++) {
				float a = (float)fabs((double)r->data.x_origin - global_enemy_path_X_array[i]);
				float b = (float)fabs((double)r->data.y_origin - global_enemy_path_Y_array[i]);
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
				r->enemy_path_x[1] = global_enemy_path_X_array[ClosestWaypoint];
				r->enemy_path_y[1] = r->data.y_origin;
				r->enemy_path_x[2] = global_enemy_path_X_array[ClosestWaypoint];
				r->enemy_path_y[2] = global_enemy_path_Y_array[ClosestWaypoint];
				r->current_way_point = 0;
			}
			else if (XorY == 2) {
				r->enemy_path_x[0] = r->data.x_origin;
				r->enemy_path_y[0] = r->data.y_origin;
				r->enemy_path_x[1] = r->data.x_origin;
				r->enemy_path_y[1] = global_enemy_path_Y_array[ClosestWaypoint];
				r->enemy_path_x[2] = global_enemy_path_X_array[ClosestWaypoint];
				r->enemy_path_y[2] = global_enemy_path_Y_array[ClosestWaypoint];

				r->current_way_point = 0;
			}
			r->state = kEnemyAdjusting;
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions resets the path for enemy when they have finished adjusting back to the global path
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void ResetEnemyPathWaypoints(Enemy* r) {
	for (int i = 0; i < 50; i++) {
		r->enemy_path_x[i] = global_enemy_path_X_array[i];
		r->enemy_path_y[i] = global_enemy_path_Y_array[i];
	}
}


/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions increases the turret damage by 20% when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffIncreasedTurretDamage(void) {
	static float damage_increase[kMaxTurret];
	static int level_check[kMaxTurret];
	for (int i = 0; i < kMaxTurret; i++) {
		if (turret[i].is_active) {
			if (level.current_effect == kIncreasedTurretDamage && turret[i].env_effects == kTEnvironmentNoEffect) {
				damage_increase[i] = turret[i].mod.damage * 0.2f;
				turret[i].mod.damage += damage_increase[i];
				level_check[i] = turret[i].level;
				turret[i].env_effects = kTEnvironmentIncreasedDamage;
			}
			else if (level.current_effect == kIncreasedTurretDamage && turret[i].env_effects == kTEnvironmentIncreasedDamage) {
				if (turret[i].level != level_check[i]) {
					turret[i].mod.damage -= damage_increase[i];
					damage_increase[i] = 0;
					turret[i].env_effects = kTEnvironmentNoEffect;
				}
			}
			else if (level.current_effect != kIncreasedTurretDamage && turret[i].env_effects == kTEnvironmentIncreasedDamage) {
				turret[i].mod.damage -= damage_increase[i];
				damage_increase[i] = 0;
				turret[i].env_effects = kTEnvironmentNoEffect;
			}
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions decreases the turret damage by 20% when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffDecreasedTurretDamage(void) {
	static float damage_decrease[kMaxTurret];
	static int level_check[kMaxTurret];
	for (int i = 0; i < kMaxTurret; i++) {
		if (turret[i].is_active) {
			if (level.current_effect == kDecreasedTurretDamage && turret[i].env_effects == kTEnvironmentNoEffect) {
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
			else if (level.current_effect == kDecreasedTurretDamage && turret[i].env_effects == kTEnvironmentDecreasedDamage) {
				if (turret[i].level != level_check[i]) {
					turret[i].mod.damage += damage_decrease[i];
					damage_decrease[i] = 0;
					turret[i].env_effects = kTEnvironmentNoEffect;
				}
			}
			else if (level.current_effect != kDecreasedTurretDamage && turret[i].env_effects == kTEnvironmentDecreasedDamage) {
				turret[i].mod.damage += damage_decrease[i];
				damage_decrease[i] = 0;
				turret[i].env_effects = kTEnvironmentNoEffect;
			}
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions increases the turret attack speed by 20% when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffIncreasedTurretAttackSpeed(void) {
	static float atk_spd_increase[kMaxTurret];
	static int level_check[kMaxTurret];
	for (int i = 0; i < kMaxTurret; i++) {
		if (turret[i].is_active) {
			if (level.current_effect == kIncreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmentNoEffect) {
				atk_spd_increase[i] = turret[i].mod.shoot_rate * 0.2f;
				turret[i].mod.shoot_rate -= atk_spd_increase[i];
				level_check[i] = turret[i].level;
				turret[i].env_effects = kTEnvironmentIncreasedAttackSpeed;
			}
			else if (level.current_effect == kIncreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmentIncreasedAttackSpeed) {
				if (turret[i].level != level_check[i]) {
					turret[i].mod.shoot_rate += atk_spd_increase[i];
					atk_spd_increase[i] = 0;
					turret[i].env_effects = kTEnvironmentNoEffect;
				}
			}
			else if (level.current_effect != kIncreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmentIncreasedAttackSpeed) {
				turret[i].mod.shoot_rate += atk_spd_increase[i];
				atk_spd_increase[i] = 0;
				turret[i].env_effects = kTEnvironmentNoEffect;
			}
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions decreases the turret attack speed by 20% when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffDecreasedTurretAttackSpeed(void) {
	static float atk_spd_decrease[kMaxTurret];
	static float level_check[kMaxTurret];
	for (int i = 0; i < kMaxTurret; i++) {
		if (turret[i].is_active) {
			if (level.current_effect == kDecreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmentNoEffect) {
				atk_spd_decrease[i] = turret[i].mod.shoot_rate * 0.2f;
				turret[i].mod.shoot_rate += atk_spd_decrease[i];
				level_check[i] = (float)turret[i].level;
				turret[i].env_effects = kTEnvironmnetDecreasedAttackSpeed;
			}
			else if (level.current_effect == kDecreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmnetDecreasedAttackSpeed) {
				if (turret[i].level != level_check[i]) {
					turret[i].mod.shoot_rate -= atk_spd_decrease[i];
					atk_spd_decrease[i] = 0;
					turret[i].env_effects = kTEnvironmentNoEffect;
				}
			}
			else if (level.current_effect != kDecreasedTurretAttackSpeed && turret[i].env_effects == kTEnvironmnetDecreasedAttackSpeed) {
				turret[i].mod.shoot_rate -= atk_spd_decrease[i];
				atk_spd_decrease[i] = 0;
				turret[i].env_effects = kTEnvironmentNoEffect;
			}
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions increases enemy health by 30% when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffMoreHP(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == kApplying && enemy[i].health > 1) {
			int increase_hp = (int)(enemy[i].max_health * 0.3);
			enemy[i].max_health += increase_hp;
			enemy[i].health += increase_hp;
			enemy[i].env_eff = kEffected;
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions decreases enemy health by 20% when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffLessHP(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == kApplying && enemy[i].health > 1) {
			int decrease_hp = (int)(enemy[i].max_health * 0.2);
			enemy[i].max_health -= decrease_hp;
			enemy[i].health -= decrease_hp;
			enemy[i].env_eff = kEffected;
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions increases enemy speed by 20% when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffFasterEnemies(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == kApplying && enemy[i].health > 1) {
			enemy[i].speed *= 1.2f;
			enemy[i].env_eff = kEffected;
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions decreases enemy speed by 20% when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffSlowerEnemies(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == kApplying && enemy[i].health > 1) {
			enemy[i].speed = (enemy[i].speed/5)*4;
			enemy[i].env_eff = kEffected;
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions increases enemy points by 20% given when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffIncreasedPhantomQuartz(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == kApplying && enemy[i].health > 1) {
			enemy[i].points = (int)(enemy[i].points*1.2);
			enemy[i].env_eff = kEffected;
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions decreases enemy points by 20% given when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffDecreasedPhantomQuartz(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == kApplying && enemy[i].health > 1) {
			enemy[i].points = (int)((enemy[i].points/5)*4);
			enemy[i].env_eff = kEffected;
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions decreases enemy points to 0 given when the env eff is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvEffNoPhantomQuartz(void) {
	for (int i = 0; i < kMaxEnemies; i++) {
		if (enemy[i].env_eff == kApplying && enemy[i].health > 1) {
			enemy[i].points = 0;
			enemy[i].env_eff = kEffected;
		}
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions applies the correct env eff when it is active
@param		void
@return		void
*//*_____________________________________________________________*/
void EnvironmentEffCheck(void) {
	EnvEffIncreasedTurretAttackSpeed();
	EnvEffDecreasedTurretAttackSpeed();
	EnvEffIncreasedTurretDamage();
	EnvEffDecreasedTurretDamage();

	switch (level.current_effect) {
	case kIncreasedPhantomQuartz:
		EnvEffIncreasedPhantomQuartz();
		break;
	case kDecreasedPhantomQuartz:
		EnvEffDecreasedPhantomQuartz();
		break;
	case kFasterEnemies:
		EnvEffFasterEnemies();
		break;
	case kSlowerEnemies:
		EnvEffSlowerEnemies();
		break;
	case kMoreHP:
		EnvEffMoreHP();
		break;
	case kLessHP:
		EnvEffLessHP();
		break;
	case kNoPhantomQuartz:
		EnvEffNoPhantomQuartz();
		break;
	default:
		break;
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions powers up the enemies the more waves you have survived
@param		Pointer to Enemy struct
@return		void
*//*_____________________________________________________________*/
void EnemyWavePowUp(Enemy* r) {
	if (r->wave_pow_up_is_active == 0) {
		int a = level.current_wave;
		r->max_health = (float)(r->max_health * (1 + (0.2 * a)));
		r->health = (float)(r->health*(1 + (0.2 * a)));
		r->speed += 1 * a;
		r->wave_pow_up_is_active = 1;
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions reduces enemy pow based on power ups bought
@param		Pointer to enemy struct
@return		void
*//*_____________________________________________________________*/
void EnemyVariableChangeforPowUps(Enemy* r) {
	if (r->enemy_pow_up[0] == 0) {
		r->health *= (1 - (level.current_power_up_level.reduce_enemy_health * 0.05f));
		r->enemy_pow_up[0] = 1;
	}
	if (r->enemy_pow_up[1] == 0) {
		r->speed *= (1 - (level.current_power_up_level.reduce_enemy_speed * 0.05f));
		r->enemy_pow_up[1] = 1;
	}
	if (r->enemy_pow_up[2] == 0) {
		r->points = (int)(r->points *( 1+(level.current_power_up_level.more_phantom_quartz *0.10)));
		r->enemy_pow_up[2] = 1;
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions updates enemies to spawn on a timer or if there are no active enemies during a wave
			It also contains all other functions to update enemy variables
@param		void
@return		void
*//*_____________________________________________________________*/
void UpdateEnemies(void) {
	enemy_timer += CP_System_GetDt();
	for (int i = 0; i < kMaxEnemies; i++) {
		int spawn_timer = 2;
		if ((enemy[i].state == kEnemyInactive) && (enemy[i].health > 0) && ((int)enemy_timer / spawn_timer <= kMaxEnemies)) {
			int state_check = 0;
			int b = (int)enemy_timer;
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
				wave_timer = (int)enemy_timer;
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

		UpdateEnemyPathWaypointArray();
		CheckEnemyPathAdjustment(&enemy[i]);
		EnemyMovement(&enemy[i], enemy[i].enemy_path_x, enemy[i].enemy_path_y, number_of_points);
		EnemyDeath(&enemy[i]);
		ReaperAbility(&enemy[i]);
		EnemyWavePowUp(&enemy[i]);
		EnemyVariableChangeforPowUps(&enemy[i]);
	}
}

/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		This functions initialises all sounds used in the game
@param		void
@return		void
*//*_____________________________________________________________*/
void MusicInit(void) {
	button_click_sfx = CP_Sound_Load("./Assets/sfx/btn.wav");
	turret_place_sfx = CP_Sound_Load("./Assets/sfx/place-turret.mp3");
	mine_explosion_sfx = CP_Sound_Load("./Assets/sfx/Explosion.wav");
	win_sfx = CP_Sound_Load("./Assets/sfx/WinSFX.wav");
	lose_sfx = CP_Sound_Load("./Assets/sfx/LoseSFX.wav");
	spawn_exit_sfx = CP_Sound_Load("./Assets/sfx/Zap.wav");
	shoot_sfx = CP_Sound_Load("./Assets/sfx/shoot.mp3");
	hit_sfx = CP_Sound_Load("./Assets/sfx/HitSFX.wav");
	sfx_volume = 0.3f;

	wave_bgm = CP_Sound_Load("./Assets/sfx/SPHERES & STARS - NewAge MSCNEW1_17.wav");
	building_bgm = CP_Sound_Load("./Assets/sfx/INPUT - NewAge MSCNEW2_01.wav");
	main_menu_bgm = CP_Sound_Load("./Assets/sfx/THE NINTH HOUR - Jazz MSCJAZ1_46.wav");
	main_menu_music = CP_Sound_LoadMusic("./Assets/sfx/THE NINTH HOUR - Jazz MSCJAZ1_46.wav");
	bgm_volume = 0.5f;
}