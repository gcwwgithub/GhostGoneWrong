/*!
@file       Samuel.c
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)\
@co-author
@course     CSD 1400
@section    C
@date       28/11/2021
@brief    	Contains function definition remove_turret, render_turret,
			update_turret, shoot, update_projectile, render_projectile,
			update_turretAnimation, create_particle, update_particle and
			render_particle.
*//*__________________________________________________________________________*/

#if _DEBUG
#include <stdio.h>
#endif
#include <math.h>

#include "cprocessing.h"
#include "game.h"
#include "Samuel.h"
#include "Gabriel.h"


void turret_init(void)
{
	Vector2 v;

	turret_on_grid = (int**)calloc(level_grid_cols, sizeof(int*));
	//using size of pointers instead of static 8 so that it takes less memory on devices that uses smaller memory address pool like 32 bit system

	for (int i = 0; i < level_grid_cols; i++) {
		if (turret_on_grid != NULL) {
			turret_on_grid[i] = (int*)calloc(level_grid_rows, sizeof(int));
			if (turret_on_grid[i] == NULL) {
				exit_to_desktop();// calling anderson exit in case calloc fail
			}
		}
		else {
			exit_to_desktop();// calling anderson exit in case calloc fail
		}
	}
	//Array turret_on_grid[gameGridCols][gameGridRows]


	for (int i = 0; i < kMaxProjectile; ++i)
	{
		proj[i].is_active = kFalse;
		proj[i].data.x_origin = 0;
		proj[i].data.y_origin = 0;
		proj[i].mod.damage = 1.f;
		proj[i].mod.speed = game.grid_width;
		proj[i].size = 48;
		proj[i].mod.slow_amt = 1.f;
		proj[i].mod.slow_timer = 0.f;
		proj[i].lifetime = 10.f;
	}
	for (int i = 0; i < kMaxTurret; ++i)
	{
		turret[i].is_active = kFalse;
		turret[i].size = game.grid_height;
		turret[i].angle = 0.f;
		//turret[i].mod.cooldown = 0.f;
		turret[i].mod.shoot_rate = 0.5f;
		turret[i].mod.damage = 1.f;
		turret[i].mod.slow_amt = 1.f; //leaving it at 1 means no slow if slow_amt < 1 then slow
		turret[i].mod.slow_timer = 0.f;
		v.x = 0;
		v.y = 1;
		turret[i].dir = v;
		turret[i].turret_anim_timer = 0;
		turret[i].turret_img = slow_turret_image_array[0];
		turret[i].current_aim_state = kTurretInactive;
		turret[i].anim_counter = 0;
		turret[i].sell_price = 25;
		turret[i].level = 1;
	}
	//init the lcoations of turret placed
	if (turret_on_grid != NULL) { // Redundant if statement because intellisense is flagging possible null pointers
		for (int i = 0; i < level_grid_cols; ++i) {
			if (turret_on_grid[i] != NULL) { // Redundant if statement because intellisense is flagging possible null pointers
				for (int j = 0; j < level_grid_rows; ++j) {
					turret_on_grid[i][j] = -1;
				}
			}
		}
	}

	//set price of turrets and stuff
	turret_purchasing[kTPPrice][kTBasic] = 100;
	turret_purchasing[kTPPrice][kTSlow] = 200;
	turret_purchasing[kTPPrice][kTHoming] = 200;
	turret_purchasing[kTPPrice][kTMine] = 100;

	turret_purchasing[kTPUpgradePrice][kTBasic] = 50;
	turret_purchasing[kTPUpgradePrice][kTSlow] = 50;
	turret_purchasing[kTPUpgradePrice][kTHoming] = 50;
	turret_purchasing[kTPUpgradePrice][kTMine] = 15;

	//set all to 5 level only first change later
	for (int i = 0; i < kTMax; ++i)
		turret_purchasing[kTPUpgradeMaxLevel][i] = 10;


	for (int i = 0; i < sizeof(particles) / sizeof(particles[0]); ++i)
	{
		v.x = 0.f;
		v.y = 0.f;
		particles[i].dir = v;
		particles[i].pos = v;
		particles[i].is_active = kFalse;
		particles[i].timer = 0.f;
		particles[i].duration = 5.f;
		particles[i].size = 10.f;
	}
}

//call this function to place turret (pass in the grid index)
void place_turret(TurretType type, int index_x, int index_y)
{
	for (int i = 0; i < kMaxTurret; ++i)
	{
		//break from loop if not enough money
		//if (Level.phantomQuartz < turret[i].price)
		//	break;
		if (turret[i].is_active)
			continue;

		turret[i].type = type;

		//edit here for the type range and dmg
		switch (turret[i].type)
		{
		case kTBasic:
			turret[i].mod.range = game.grid_width * 2;
			turret[i].mod.damage = 1.0f;
			turret[i].anim_counter = 0;
			turret[i].turret_anim_timer = 0;
			//turret[i].mod.speed = 300.f;
			turret[i].mod.speed = game.grid_width * 5.5f;
			break;
		case kTSlow: // FREEZE TURRET
			turret[i].mod.range = game.grid_width * 2;
			turret[i].mod.damage = 0.5f;
			turret[i].mod.slow_amt = 0.9f; //leaving it at 1 means no slow if slow_amt < 1 then slow
			turret[i].mod.slow_timer = 0.8f;
			turret[i].anim_counter = 0;
			turret[i].turret_anim_timer = 0;
			//turret[i].mod.speed = 200.f;
			turret[i].mod.speed = game.grid_width * 4.f;
			break;
		case kTHoming:
			turret[i].mod.range = game.grid_width * 2;
			turret[i].mod.damage = 1.0f;
			turret[i].dir.x = -1;
			turret[i].dir.y = -1;
			turret[i].anim_counter = 0;
			turret[i].turret_anim_timer = 0;
			//turret[i].mod.speed = 100.f;
			turret[i].mod.speed = game.grid_width * 2.f;
			break;
		case kTMine:
			if (Level.grid[index_y][index_x].type != kPath)
				return;
			turret[i].mod.range = game.grid_width * 0.7f;
			turret[i].mod.damage = (float)(10 + Level.current_power_up_level.increased_mine_damage * 20);
			turret[i].anim_counter = 0;
			turret[i].turret_anim_timer = 0;
			break;
		default:
			break;
		}
		//Set turret range collision
		turret[i].data.width = turret[i].mod.range * 1.8f;
		turret[i].data.height = turret[i].mod.range * 1.8f;
		turret[i].data.object_type = kObjectCircle;
		//sell price back to default
		turret[i].sell_price = (int)(turret_purchasing[kTPPrice][type] * 0.7f);
		//upgrade price back to default
		turret[i].upgrade_price = turret_purchasing[kTPUpgradePrice][type];
		//total price accumalated
		turret[i].total_price = turret_purchasing[kTPPrice][type];
		//shooting rate set
		turret[i].mod.shoot_rate = 0.5f;
		//set to active and the turret type
		turret[i].is_active = kTrue;
		//origin + gridwidth * (index + 0.5); (to place the turret on the grid box)
		turret[i].data.x_origin = game.x_origin + (game.grid_width * (index_x + 0.5f));
		turret[i].data.y_origin = game.y_origin + (game.grid_height * (index_y + 0.5f));
		// where on grid turret placed storing the index of placed turret
		turret_on_grid[index_x][index_y] = i;
		//where u place u block
		turret[i].level = 1;
		//escape from loop once done
		break;
	}
}

void remove_turret(int index_x, int index_y)
{
	if (turret_on_grid == NULL)
		return;

	int index = turret_on_grid[index_x][index_y];
	if (index < 0)
		return;

	turret_on_grid[index_x][index_y] = -1;
	turret[index].is_active = kFalse;
}

//sell them turrets
void sell_turret(int t_index)
{
	int x = (int)((turret[t_index].data.x_origin - game.x_origin) / game.grid_width);
	int y = (int)((turret[t_index].data.y_origin - game.y_origin) / game.grid_height);
	float sell_price;
	sell_price = (turret[t_index].total_price * 0.7f);
	Level.phantom_quartz += (int)sell_price;
	remove_turret(x, y);
}

//upgrade system (for now using index will change depending on usage)
void upgrade_turret(int t_index)
{
	//int x = (int)((turret[t_index].data.xOrigin - Game.xOrigin) / Game.gridWidth);
	//int y = (int)((turret[t_index].data.yOrigin - Game.yOrigin) / Game.gridHeight);

	//dont increase beyond lvl 10
	if (turret[t_index].level >= turret_purchasing[kTPUpgradeMaxLevel][turret[t_index].type])
		return;

	turret[t_index].total_price += turret[t_index].upgrade_price;
	turret[t_index].sell_price = (int)((turret[t_index].total_price) * 0.7f);

	turret[t_index].level++;
	switch (turret[t_index].type)
	{
	case kTBasic:
		turret[t_index].mod.damage += 0.011f * turret[t_index].upgrade_price;
		//increase the price for another upgrade
		turret[t_index].upgrade_price += 50;
		break;
	case kTSlow:
		turret[t_index].mod.damage += 0.15f;
		turret[t_index].mod.range += turret[t_index].mod.range * 0.05f;
		turret[t_index].mod.slow_amt -= 0.05f;
		//increase the price for another upgrade
		turret[t_index].upgrade_price += 50;
		break;
	case kTHoming:
		turret[t_index].mod.damage += 0.005f * turret[t_index].upgrade_price;
		turret[t_index].upgrade_price += 50;
		break;
	case kTMine:
		turret[t_index].mod.damage += 1.f;
		//increase the price for another upgrade
		turret[t_index].upgrade_price += 25;
		break;
	default:
		break;
	}

}

void render_turret(void)
{
	for (int i = 0; i < kMaxTurret; ++i)
	{
		if (!turret[i].is_active)
			continue;

		//draw type of turrets
		update_turretAnimation(&turret[i]);
		turret[i].turret_anim_timer += CP_System_GetDt();
		switch (turret[i].type)
		{
		case kTBasic:
			RenderImageFromSpriteSheet(basic_turret_spritesheet, basic_turret_spritesheet_array[turret[i].anim_counter],
				turret[i].data.x_origin, turret[i].data.y_origin, turret[i].size, turret[i].size);
			break;
		case kTSlow:
			CP_Image_DrawAdvanced(turret[i].turret_img, turret[i].data.x_origin, turret[i].data.y_origin,
				turret[i].size, turret[i].size, 255, turret[i].angle + 90.f);//the +90 degree is to offset the atan2
			break;
		case kTHoming:
			RenderImageFromSpriteSheet(homing_missle_turret_spritesheet, homing_missle_turret_spritesheet_array[turret[i].anim_counter],
				turret[i].data.x_origin, turret[i].data.y_origin, turret[i].size, turret[i].size);
			break;
		case kTMine:
			RenderImageFromSpriteSheet(mine_spritesheet, mine_spritesheet_array[turret[i].anim_counter],
				turret[i].data.x_origin, turret[i].data.y_origin, turret[i].size, turret[i].size);
			break;
		default:
			break;
		}
	}

	if (turretSelectedToUpgrade != kNoTurretSelected)
	{
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255 / 2));
		CP_Graphics_DrawCircle(turret[turretSelectedToUpgrade].data.x_origin,
			turret[turretSelectedToUpgrade].data.y_origin, turret[turretSelectedToUpgrade].mod.range * 2);
	}
}

void update_turret(void)
{
	//highest wp, enemy index, enemy dist to turret
	int wp = -1, e_index = -1;
	//dist check, targeted enemy direction
	Vector2 v1, targeted_dir;
	v1.x = 0;
	v1.y = 0;
	targeted_dir = v1;

	for (int i = 0; i < kMaxTurret; ++i)
	{
		if (!turret[i].is_active)
			continue;

		//reset variables
		wp = -1;
		e_index = -1;

		//Start of enemy loop
		for (int j = 0; j < kMaxEnemies; ++j)
		{
			//skip if dead
			if (enemy[j].state == kEnemyDeath || enemy[j].state == kEnemyInactive)
				continue;


			if (turret[i].type == kTMine)
			{
				turret[i].mod.damage = (float)(10 + Level.current_power_up_level.increased_mine_damage * 20);
				if (CollisionDetection(turret[i].data, enemy[j].data))
				{
					//set the highest waypoint
					wp = enemy[j].current_way_point;
					//set the index of enemy to target
					e_index = j;
					break;
				}
				else
					continue;
			}

			//find dist
			v1.x = enemy[j].data.x_origin - turret[i].data.x_origin;
			v1.y = enemy[j].data.y_origin - turret[i].data.y_origin;
			//if in range of turret
			//if (magnitude_sq(v1) <= turret[i].mod.range * turret[i].mod.range)
			if (CollisionDetection(enemy[j].data, turret[i].data))
			{
				//target the enemy closest to end goal needs refinig
				if (enemy[j].current_way_point > wp)
				{
					//set the highest waypoint
					wp = enemy[j].current_way_point;
					//set the index of enemy to target
					e_index = j;
					//set the targeted enemy dir
					targeted_dir.x = v1.x;
					targeted_dir.y = v1.y;
				}
				else
					continue;
			}
		}// end of enemy loop

		//if there is a targeted enemy shoot him
		if (e_index >= 0)
		{
			turret[i].current_aim_state = kTurretShooting;
			if (turret[i].anim_counter <= 2)
			{
				turret[i].anim_counter = 3;
			}

			// update the dir of all turret except homing turret
			if (turret[i].type != kTHoming)
			{
				turret[i].dir = targeted_dir;
				turret[i].dir = normalise(turret[i].dir);
			}

			//get angle to roate sprite for slow turret only
			if (turret[i].type == kTSlow)
				turret[i].angle = atan2f(turret[i].dir.y, turret[i].dir.x) * 180.f / (float)PI;

			//mine specific updates
			if (turret[i].type == kTMine)
			{
				//set mine dmg to power temp
				turret[i].mod.damage += Level.current_power_up_level.increased_mine_damage;
				turret[i].mod.tracked_index = e_index;
				//fake shoot for mine, just spawn a proj on it
				shoot(turret[i].data.x_origin, turret[i].data.y_origin, turret[i].mod, turret[i].type, turret[i].dir);
				//sync the remove with animation later
				remove_turret((int)((turret[i].data.x_origin - game.x_origin) / game.grid_width),
					(int)((turret[i].data.y_origin - game.y_origin) / game.grid_height));
				continue; //go to next in iter since mine update is done
			}

			if (/*turret[i].mod.cooldown <= 0 &&*/ turret[i].turret_anim_timer >= turret[i].mod.shoot_rate && turret[i].anim_counter >= 5)
			{
				turret[i].mod.tracked_index = e_index;
				//printf("index: %d\n", e_index);
				shoot(turret[i].data.x_origin, turret[i].data.y_origin, turret[i].mod, turret[i].type, turret[i].dir);
				//turret[i].mod.cooldown = 2.f;
			}
		}
		else
		{
			turret[i].current_aim_state = kTurretInactive;
		}

	}// end of turret loop
}

void shoot(float x, float y, Modifiers mod, ProjectileType type, Vector2 dir)
{
	//takes the pos of turret and dir turret facing
	//loop to find unactive projectile
	for (int i = 0; i < kMaxProjectile; ++i)
	{
		if (proj[i].is_active)
			continue;

		//set the projectile as active and other stuff
		proj[i].is_active = 1;

		switch (type)
		{
		case kPBasic:
			proj[i].data.x_origin = x;
			proj[i].data.y_origin = y - 10.f;
			break;
		case kPSlow:
			proj[i].data.x_origin = x + (float)(PROJ_OFFSET * dir.x);
			proj[i].data.y_origin = y + (float)(PROJ_OFFSET * dir.y);
			break;
		case kPMine:
			proj[i].data.width = game.grid_width;
			proj[i].data.height = game.grid_height;
			proj[i].data.x_origin = x;
			proj[i].data.y_origin = y;
			break;
		case kPHoming:
			proj[i].data.x_origin = x + (float)(PROJ_OFFSET * dir.x);
			proj[i].data.y_origin = y + (float)(PROJ_OFFSET * dir.y);
			break;
		default:
			proj[i].data.x_origin = x;
			proj[i].data.y_origin = y;
			proj[i].data.width = 10.f;
			proj[i].data.height = 10.f;
			break;
		}
		proj[i].dir = dir;
		proj[i].data.object_type = kObjectCircle;
		proj[i].mod.damage = mod.damage;
		proj[i].mod.slow_amt = mod.slow_amt;
		proj[i].mod.slow_timer = mod.slow_timer;
		proj[i].type = type;
		proj[i].mod.tracked_index = mod.tracked_index;
		proj[i].mod.speed = mod.speed;
		proj[i].lifetime = 10.f;
		break;
	}

}

void update_projectile(void)
{
	float dt = CP_System_GetDt();
	for (int i = 0; i < kMaxProjectile; ++i)
	{
		//bounds check
		if (proj[i].is_active &&
			(proj[i].data.x_origin < 0 || proj[i].data.x_origin >(float)CP_System_GetDisplayWidth()
				|| proj[i].data.y_origin < 0 || proj[i].data.y_origin >(float)CP_System_GetDisplayHeight()))
		{
			//set to inactive
			proj[i].is_active = kFalse;
			continue;
		}
		if (!proj[i].is_active)
			continue;

		proj[i].lifetime -= dt;
		if (proj[i].lifetime <= 0.f)
		{
			proj[i].is_active = kFalse;
			continue;
		}


		// tracking proj, track if a valid id is provided and state is alive
		if (proj[i].type == kPHoming)
		{
			if (proj[i].mod.tracked_index >= 0 &&
				enemy[proj[i].mod.tracked_index].state != kEnemyDeath &&
				enemy[proj[i].mod.tracked_index].state != kEnemyInactive)
			{
				//fake homing projectile
				Vector2 v;
				v.x = proj[i].dir.x - (enemy[proj[i].mod.tracked_index].data.x_origin - proj[i].data.x_origin);
				v.y = proj[i].dir.y - (enemy[proj[i].mod.tracked_index].data.y_origin - proj[i].data.y_origin);


				Vector2 e_dir = { .x = enemy[proj[i].mod.tracked_index].data.x_origin,
								.y = enemy[proj[i].mod.tracked_index].data.y_origin };
				e_dir.x -= proj[i].data.x_origin;
				e_dir.y -= proj[i].data.y_origin;
				e_dir = normalise(e_dir);
				//printf("%f\n", acosf(dot(e_dir, proj[i].dir)) * (180.f / PI));
				float turn_rate = 0.08f;
				if (dot(e_dir, proj[i].dir) <= 0.2f)
					turn_rate = 1.5f;

				proj[i].dir.x -= v.x * turn_rate * dt; //gradual change of dir, magic number is the rate of change
				proj[i].dir.y -= v.y * turn_rate * dt;
				proj[i].dir = normalise(proj[i].dir);
				//printf("x:%f y:%f\n", proj[i].dir.x, proj[i].dir.y);
			}
			else//update the projectile targeting
			{
				Vector2 v;
				float dist = game.width * 10, tmp = 0; //abritrary large number
				for (int j = 0; j < kMaxEnemies; ++j)
				{
					if (enemy[j].state == kEnemyDeath || enemy[j].state == kEnemyInactive)
						continue;

					v.x = proj[i].data.x_origin - enemy[j].data.x_origin;
					v.y = proj[i].data.y_origin - enemy[j].data.y_origin;

					//dont track when out of range
					if (magnitude_sq(v) > HOMING_RANGE * HOMING_RANGE)
						continue;

					tmp = magnitude_sq(v);
					float sqrdst = dist * dist;
					if (tmp < sqrdst)
					{
						dist = tmp;
						proj[i].mod.tracked_index = j;
					}
				}
			}
		}


		//projectile of mine dont move
		if (proj[i].type != kPMine)
		{
			//proj movement dir * speed * deltatime
			proj[i].data.x_origin += proj[i].dir.x * proj[i].mod.speed * dt;
			proj[i].data.y_origin += proj[i].dir.y * proj[i].mod.speed * dt;
		}

	}
}

void render_projectile(void)
{
	for (int i = 0; i < kMaxProjectile; ++i)
	{
		if (!proj[i].is_active)
			continue;

		//render of the projectile here for now
		//CP_Graphics_DrawCircle(proj[i].data.xOrigin, proj[i].data.yOrigin, proj[i].size);

		switch (proj[i].type)
		{
		case kPBasic:
			RenderImageFromSpriteSheet(turret_bullet_spritesheet, turret_bullet_spritesheet_array[0], proj[i].data.x_origin, proj[i].data.y_origin, proj[i].size, proj[i].size);
			break;
		case kPSlow:
			RenderImageFromSpriteSheet(turret_bullet_spritesheet, turret_bullet_spritesheet_array[1], proj[i].data.x_origin, proj[i].data.y_origin, proj[i].size, proj[i].size);
			break;
		case kPHoming:
			RenderImageFromSpriteSheet(turret_bullet_spritesheet, turret_bullet_spritesheet_array[2], proj[i].data.x_origin, proj[i].data.y_origin, proj[i].size, proj[i].size);
			break;
		case kPMine:
			CP_Settings_Fill(COLOR_RED);
			CP_Graphics_DrawCircle(proj[i].data.x_origin, proj[i].data.y_origin, proj[i].size);
			break;
		}
	}
}

void col_type_projectile(Projectile* p)
{
	float dist;
	Vector2 dif;
	switch (p->type) //will refactor
	{
	case kPSlow:
	{
		for (int i = 0; i < kMaxEnemies; ++i)
		{
			//skip dead or inactive
			if (enemy[i].state == kEnemyInactive || enemy[i].state == kEnemyDeath)
				continue;

			dif.x = enemy[i].data.x_origin - p->data.x_origin;
			dif.y = enemy[i].data.y_origin - p->data.y_origin;
			dist = magnitude_sq(dif);
			if (dist <= SLOW_RANGE * SLOW_RANGE) // will change range to be able to be upgarded ltr
			{
				enemy[i].slow_amt = p->mod.slow_amt;
				enemy[i].slow_timer = p->mod.slow_timer;
			}

		}
		break;
	}
	case kPMine: /* empty and no break is intended to flow to homing*/
	case kPHoming:
	{
		for (int i = 0; i < kMaxEnemies; ++i)
		{
			//skip dead or inactive
			if (enemy[i].state == kEnemyInactive || enemy[i].state == kEnemyDeath)
				continue;
			if (p->mod.tracked_index == i) //is gonna be dmg in enemy so skip the targeted one, this case will be changed ltr has flaws
				continue;

			dif.x = enemy[i].data.x_origin - p->data.x_origin;
			dif.y = enemy[i].data.y_origin - p->data.y_origin;
			dist = magnitude_sq(dif);
			if (dist <= EXPLOSION_RANGE * EXPLOSION_RANGE) // will change range to be able to be upgarded ltr
			{
				enemy[i].health -= p->mod.damage;
				enemy[i].state = kEnemyHurt;
				enemy[i].timer = 0;
			}
		
		}
		break;
	}
	default:
		break;
	}

	//test particles
	int r_num = (CP_Random_GetInt() % (3)) + 3; //3 to 5 particle
	Vector2 pos = { .x = p->data.x_origin, .y = p->data.y_origin };
	Vector2 dir = { .x = CP_Random_GetFloat(), .y = 0.f };
	for (int i = 0; i < r_num; ++i)
	{
		create_particle(pos, dir, 15.f, 1.5f, p->type);
		dir.x = -CP_Random_GetFloat();
		dir.y = CP_Random_GetFloat();
		create_particle(pos, dir, 15.f, 1.5f, p->type);
		dir.x = CP_Random_GetFloat();
		dir.y = CP_Random_GetFloat();
		create_particle(pos, dir, 15.f, 1.5f, p->type);
	}
}

void update_turretAnimation(Turret* t)
{
	if (t->type == kTMine)
	{
		if (t->turret_anim_timer >= 2)
		{
			t->anim_counter = !t->anim_counter;
			t->turret_anim_timer = 0;
		}
	}

	else
	{
		switch (t->current_aim_state)
		{
		case kTurretInactive:
			if (t->turret_anim_timer >= 0.35)
			{
				if (t->anim_counter >= 2) //0 1 2
				{
					t->anim_counter = 0;
				}

				else
				{
					t->anim_counter++;
				}

				if (turret->type == kTSlow)
				{
					t->turret_img = slow_turret_image_array[t->anim_counter];
				}
				t->turret_anim_timer = 0;

			}

			break;
		case kTurretShooting:
			//if (t->turretAnimTimer >= 0.60)
			if (t->turret_anim_timer >= t->mod.shoot_rate)
			{
				if (t->anim_counter >= 5)
				{
					t->anim_counter = 3;
				}

				else
				{
					t->anim_counter++;
				}
				if (turret->type == kTSlow)
				{
					t->turret_img = slow_turret_image_array[t->anim_counter];
				}
				t->turret_anim_timer = 0;
			}

			break;
		}
	}

}

void create_particle(Vector2 pos, Vector2 dir, float size, float duration, ParticleType type)
{
	for (int i = 0; i < sizeof(particles) / sizeof(particles[0]); ++i)
	{
		if (particles[i].is_active)
			continue;

		particles[i].is_active = kTrue;
		particles[i].pos = pos;
		particles[i].dir = dir;
		particles[i].size = size;
		particles[i].timer = 0.f;
		particles[i].duration = duration;
		particles[i].alpha = 255;
		particles[i].type = type;
		break;
	}
}

void update_particle()
{
	float dt = CP_System_GetDt();
	for (int i = 0; i < sizeof(particles) / sizeof(particles[0]); ++i)
	{
		if (!particles[i].is_active)
			continue;

		particles[i].timer += dt;
		particles[i].alpha -= (int)(255 / particles[i].duration * dt); //(max alpha / duration) to get rate of change
		if (particles[i].timer >= particles[i].duration)
		{
			particles[i].timer = 0.f;
			particles[i].is_active = kFalse;
			continue;
		}


		particles[i].dir.y -= 1.5f * dt;
		particles[i].dir.x -= particles[i].dir.x * (particles[i].timer / particles[i].duration) * dt;
		particles[i].dir = normalise(particles[i].dir);

		particles[i].pos.x += particles[i].dir.x * 25.f * dt;
		particles[i].pos.y += particles[i].dir.y * 25.f * dt;
	}
}

void render_particle()
{
	for (int i = 0; i < sizeof(particles) / sizeof(particles[0]); ++i)
	{
		if (!particles[i].is_active)
			continue;

		switch (particles[i].type)
		{
		case kParBasic:
			RenderImageFromSpriteSheetWithAlpha(turret_bullet_spritesheet, turret_bullet_spritesheet_array[0], particles[i].pos.x, particles[i].pos.y,
				particles[i].size, particles[i].size, particles[i].alpha);
			break;
		case kParSlow:
			RenderImageFromSpriteSheetWithAlpha(turret_bullet_spritesheet, turret_bullet_spritesheet_array[1], particles[i].pos.x, particles[i].pos.y,
				particles[i].size, particles[i].size, particles[i].alpha);
			break;
		case kParHoming:
			RenderImageFromSpriteSheetWithAlpha(turret_bullet_spritesheet, turret_bullet_spritesheet_array[2], particles[i].pos.x, particles[i].pos.y,
				particles[i].size, particles[i].size, particles[i].alpha);
			break;
		}
	}
}
