#include <math.h>

#include "cprocessing.h"
#include "Samuel.h"
#include "John.h"
#include "Gabriel.h"

#if _DEBUG
#include <stdio.h>
#endif

void turret_init(void)
{
	Vector2 v;

	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		proj[i].isActive = FALSE;
		proj[i].data.xOrigin = 0;
		proj[i].data.yOrigin = 0;
		proj[i].mod.damage = 1.f;
		proj[i].mod.speed = 200.f;
		proj[i].size = 48;
		proj[i].mod.slow_amt = 1.f;
		proj[i].mod.slow_timer = 0.f;
		proj[i].lifetime = 10.f;
	}
	for (int i = 0; i < MAX_TURRET; ++i)
	{
		turret[i].isActive = FALSE;
		turret[i].size = Game.gridHeight;
		turret[i].angle = 0.f;
		//turret[i].mod.cooldown = 0.f;
		turret[i].mod.shoot_rate = 0.6f;
		turret[i].mod.damage = 1.f;
		turret[i].mod.slow_amt = 1.f; //leaving it at 1 means no slow if slow_amt < 1 then slow
		turret[i].mod.slow_timer = 0.f;
		v.x = 0;
		v.y = 1;
		turret[i].dir = v;
		turret[i].turretAnimTimer = 0;
		turret[i].turret_img = slowTurretImageArray[0];
		turret[i].currentAnimState = INACTIVE;
		turret[i].animCounter = 0;
		turret[i].sell_price = 25;
		turret[i].level = 1;
	}
	//init the lcoations of turret placed
	for (int i = 0; i < GAME_GRID_ROWS; ++i)
		for (int j = 0; j < GAME_GRID_COLS; ++j)
			turret_on_grid[i][j] = -1;

	//set price of turrets and stuff
	turret_purchasing[TP_PRICE][T_BASIC] = 25;
	turret_purchasing[TP_PRICE][T_SLOW] = 30;
	turret_purchasing[TP_PRICE][T_HOMING] = 50;
	turret_purchasing[TP_PRICE][T_MINE] = 45;
	turret_purchasing[TP_PRICE][T_WALL] = 10;

	turret_purchasing[TP_UPGRADE_PRICE][T_BASIC] = 20;
	turret_purchasing[TP_UPGRADE_PRICE][T_SLOW] = 25;
	turret_purchasing[TP_UPGRADE_PRICE][T_HOMING] = 40;
	turret_purchasing[TP_UPGRADE_PRICE][T_MINE] = 15;
	turret_purchasing[TP_UPGRADE_PRICE][T_WALL] = 10;

	//set all to 5 level only first change later
	for (int i = 0; i < T_MAX; ++i)
		turret_purchasing[TP_UPGRADE_MAX_LEVEL][i] = 10;

	//place_turret(T_WALL, 2, 1);
	//Level[currentGameLevel].grid[1][2].type = Blocked;
	//place_turret(T_SLOW, 4, 1);
	//Level[0].grid[4][1].type = Blocked;//Hard coded to set turret spot to blocked
	//place_turret(T_BASIC, 0, 4);
	//Level[0].grid[4][0].type = Blocked;//Hard coded to set turret spot to blocked
}

//call this function to place turret (pass in the grid index)
void place_turret(TurretType type, int index_x, int index_y)
{
	for (int i = 0; i < MAX_TURRET; ++i)
	{
		//break from loop if not enough money
		//if (Level[currentGameLevel].phantomQuartz < turret[i].price)
		//	break;
		if (turret[i].isActive)
			continue;

		turret[i].type = type;

		//edit here for the type range and dmg
		switch (turret[i].type)
		{
		case T_BASIC:
			turret[i].mod.range = Game.gridWidth * 2;
			turret[i].mod.damage = 1;
			turret[i].animCounter = 0;
			turret[i].turretAnimTimer = 0;
			turret[i].mod.speed = 200.f;
			break;
		case T_SLOW: // FREEZE TURRET
			turret[i].mod.range = Game.gridWidth * 2;
			turret[i].mod.damage = 0.5f;
			turret[i].mod.slow_amt = 0.6f; //leaving it at 1 means no slow if slow_amt < 1 then slow
			turret[i].mod.slow_timer = 2.f;
			turret[i].animCounter = 0;
			turret[i].turretAnimTimer = 0;
			turret[i].mod.speed = 200.f;
			break;
		case T_HOMING:
			turret[i].mod.range = Game.gridWidth * 2;
			turret[i].mod.damage = 1;
			turret[i].dir.x = -1;
			turret[i].dir.y = -1;
			turret[i].animCounter = 0;
			turret[i].turretAnimTimer = 0;
			turret[i].mod.speed = 100.f;
			break;
		case T_MINE:
			if (Level[currentGameLevel].grid[index_y][index_x].type != Path)
				return;
			turret[i].mod.range = Game.gridWidth * 2;
			turret[i].mod.damage = 2;
			turret[i].data.width = Game.gridWidth * 0.7f;
			turret[i].data.width = Game.gridHeight * 0.7f;
			turret[i].data.objectType = objectCircle;
			turret[i].animCounter = 0;
			turret[i].turretAnimTimer = 0;
			break;
		case T_WALL:

			break;
		default:
			break;
		}
		//sell price back to default
		turret[i].sell_price = (int)(turret_purchasing[TP_PRICE][type] * 0.7f);
		//upgrade price back to default
		turret[i].upgrade_price = turret_purchasing[TP_UPGRADE_PRICE][type];
		//total price accumalated
		turret[i].total_price = turret_purchasing[TP_PRICE][type];
		//shooting rate set
		turret[i].mod.shoot_rate = 0.6f;
		//set to active and the turret type
		turret[i].isActive = TRUE;
		//origin + gridwidth * (index + 0.5); (to place the turret on the grid box)
		turret[i].data.xOrigin = Game.xOrigin + (Game.gridWidth * (index_x + 0.5f));
		turret[i].data.yOrigin = Game.yOrigin + (Game.gridHeight * (index_y + 0.5f));
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
	int index = turret_on_grid[index_x][index_y];
	if (index < 0)
		return;

	//set that grid not in used
	turret_on_grid[index_x][index_y] = -1;
	turret[index].isActive = FALSE;
	//set to clear if is blocked
	if (Level[currentGameLevel].grid[index_y][index_x].type == Blocked)
		Level[currentGameLevel].grid[index_y][index_x].type = Clear;

}

//sell them turrets
void sell_turret(int t_index)
{
	int x = (int)((turret[t_index].data.xOrigin - Game.xOrigin) / Game.gridWidth);
	int y = (int)((turret[t_index].data.yOrigin - Game.yOrigin) / Game.gridHeight);
	float sell_price;

	sell_price = (int)(turret[t_index].total_price * 0.7f);
	//if (turret[t_index].level != 1)
	//	sell_price = (turret[t_index].upgrade_price + turret_purchasing[TP_PRICE][turret[t_index].type]) * 0.7f;
	//else
	//	sell_price = turret_purchasing[TP_PRICE][turret[t_index].type] * 0.7f;

	//printf("sell price: %d", (int)sell_price);
	Level[currentGameLevel].phantomQuartz += (int)sell_price;
	remove_turret(x, y);
}

//upgrade system (for now using index will change depending on usage)
void upgrade_turret(int t_index)
{
	//int x = (int)((turret[t_index].data.xOrigin - Game.xOrigin) / Game.gridWidth);
	//int y = (int)((turret[t_index].data.yOrigin - Game.yOrigin) / Game.gridHeight);

	//dont increase beyond lvl 10
	if (turret[t_index].level >= turret_purchasing[TP_UPGRADE_MAX_LEVEL][turret[t_index].type])
		return;

	turret[t_index].total_price += turret[t_index].upgrade_price;
	turret[t_index].sell_price = (int)((turret[t_index].total_price) * 0.7f);

	turret[t_index].level++;
	switch (turret[t_index].type)
	{
	case T_BASIC:
		turret[t_index].mod.damage += 0.2f;
		turret[t_index].mod.range *= 1.2f;
		turret[t_index].mod.shoot_rate -= 0.02f;
		//increase the price for another upgrade
		turret[t_index].upgrade_price += 25;
		break;
	case T_SLOW:
		turret[t_index].mod.damage += 0.15f;
		turret[t_index].mod.range *= 1.2f;
		turret[t_index].mod.slow_amt -= 0.1f;
		turret[t_index].mod.shoot_rate -= 0.02f;
		//increase the price for another upgrade
		turret[t_index].upgrade_price += 25;
		break;
	case T_HOMING:
		turret[t_index].mod.damage += 0.3f;
		turret[t_index].mod.range *= 1.2f;
		turret[t_index].mod.shoot_rate -= 0.01f;
		//increase the price for another upgrade
		turret[t_index].upgrade_price += 50;
		break;
	case T_MINE:
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
	for (int i = 0; i < MAX_TURRET; ++i)
	{
		if (!turret[i].isActive)
			continue;

		//draw type of turrets
		update_turretAnimation(&turret[i]);
		turret[i].turretAnimTimer += CP_System_GetDt();
		switch (turret[i].type)
		{
		case T_BASIC:
			RenderNormal(basicTurretSpriteSheet, basicTurretArray[turret[i].animCounter],
				turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].size, turret[i].size);
			break;
		case T_SLOW:
			CP_Image_DrawAdvanced(turret[i].turret_img, turret[i].data.xOrigin, turret[i].data.yOrigin,
				turret[i].size, turret[i].size, 255, turret[i].angle + 90.f);//the +90 degree is to offset the atan2
			break;
		case T_HOMING:
			RenderNormal(homingMissleTurretSpriteSheet, homingMissleTurretArray[turret[i].animCounter],
				turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].size, turret[i].size);
			break;
		case T_MINE:
			RenderNormal(mineSpriteSheet, mineArray[turret[i].animCounter],
				turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].size, turret[i].size);
			break;
		case T_WALL:
			RenderNormal(energyWallSpriteSheet, energyWallArray[turret[i].animCounter],
				turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].size, turret[i].size);
			break;
		default:
			break;
		}
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

	for (int i = 0; i < MAX_TURRET; ++i)
	{
		if (!turret[i].isActive || turret[i].type == T_WALL)
			continue;

		//reset variables
		wp = -1;
		e_index = -1;

		//Start of enemy loop
		for (int j = 0; j < MAX_ENEMIES; ++j)
		{
			//skip if dead
			if (Enemy[j].state == Death || Enemy[j].state == Inactive)
				continue;


			if (turret[i].type == T_MINE)
			{
				if (Collision_Detection(turret[i].data, Enemy[j].data))
				{
					//set the highest waypoint
					wp = Enemy[j].CurrentWaypoint;
					//set the index of enemy to target
					e_index = j;
					break;
				}
				else
					continue;
			}

			//find dist
			v1.x = Enemy[j].data.xOrigin - turret[i].data.xOrigin;
			v1.y = Enemy[j].data.yOrigin - turret[i].data.yOrigin;
			//if in range of turret
			if (magnitude_sq(v1) <= turret[i].mod.range * turret[i].mod.range)
			{
				//target the enemy closest to end goal needs refinig
				if (Enemy[j].CurrentWaypoint > wp)
				{
					//set the highest waypoint
					wp = Enemy[j].CurrentWaypoint;
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
			turret[i].currentAnimState = SHOOTING;
			if (turret[i].animCounter <= 2)
			{
				turret[i].animCounter = 3;
			}

			// update the dir of all turret except homing turret
			if (turret[i].type != T_HOMING)
			{
				turret[i].dir = targeted_dir;
				turret[i].dir = normalise(turret[i].dir);
			}

			//get angle to roate sprite for slow turret only
			if (turret[i].type == T_SLOW)
				turret[i].angle = atan2f(turret[i].dir.y, turret[i].dir.x) * 180.f / (float)PI;

			//mine specific updates
			if (turret[i].type == T_MINE)
			{
				//fake shoot for mine, just spawn a proj on it
				shoot(turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].mod, turret[i].type, turret[i].dir);
				//sync the remove with animation later
				remove_turret((int)((turret[i].data.xOrigin - Game.xOrigin) / Game.gridWidth),
					(int)((turret[i].data.yOrigin - Game.yOrigin) / Game.gridHeight));
				continue; //go to next in iter since mine update is done
			}

			//turret[i].mod.cooldown -= 1.f * CP_System_GetDt();
			if (/*turret[i].mod.cooldown <= 0 &&*/ turret[i].turretAnimTimer >= turret[i].mod.shoot_rate && turret[i].animCounter >= 5)
			{
				turret[i].mod.tracked_index = e_index;
				//printf("index: %d\n", e_index);
				shoot(turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].mod, turret[i].type, turret[i].dir);
				//turret[i].mod.cooldown = 2.f;
			}
		}
		else
		{
			turret[i].currentAnimState = INACTIVE;
		}

	}// end of turret loop
}

void shoot(float x, float y, Modifiers mod, ProjectileType type, Vector2 dir)
{
	//takes the pos of turret and dir turret facing
	//loop to find unactive projectile
	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		if (proj[i].isActive)
			continue;

		//set the projectile as active and other stuff
		proj[i].isActive = 1;

		switch (type)
		{
		case P_BASIC:
			proj[i].data.xOrigin = x;
			proj[i].data.yOrigin = y - 10.f;
			break;
		case P_SLOW:
			proj[i].data.xOrigin = x + (float)(PROJ_OFFSET * dir.x);
			proj[i].data.yOrigin = y + (float)(PROJ_OFFSET * dir.y);
			break;
		case P_MINE:
			proj[i].data.width = Game.gridWidth;
			proj[i].data.height = Game.gridHeight;
			proj[i].data.xOrigin = x;
			proj[i].data.yOrigin = y;
			break;
		case P_HOMING:
			proj[i].data.xOrigin = x + (float)(PROJ_OFFSET * dir.x);
			proj[i].data.yOrigin = y + (float)(PROJ_OFFSET * dir.y);
			break;
		default:
			proj[i].data.xOrigin = x;
			proj[i].data.yOrigin = y;
			proj[i].data.width = 10.f;
			proj[i].data.height = 10.f;
			break;
		}
		proj[i].dir = dir;
		proj[i].data.objectType = objectCircle;
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
	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		//bounds check
		if (proj[i].isActive &&
			(proj[i].data.xOrigin < 0 || proj[i].data.xOrigin >(float)CP_System_GetDisplayWidth()
				|| proj[i].data.yOrigin < 0 || proj[i].data.yOrigin >(float)CP_System_GetDisplayHeight()))
		{
			//set to inactive
			proj[i].isActive = FALSE;
			continue;
		}
		if (!proj[i].isActive)
			continue;

		proj[i].lifetime -= dt;
		if (proj[i].lifetime <= 0.f)
		{
			proj[i].isActive = FALSE;
			continue;
		}


		// tracking proj, track if a valid id is provided and state is alive
		if (proj[i].type == P_HOMING)
		{
			if (proj[i].mod.tracked_index >= 0 &&
				Enemy[proj[i].mod.tracked_index].state != Death &&
				Enemy[proj[i].mod.tracked_index].state != Inactive)
			{
				//fake homing projectile
				Vector2 v;
				v.x = proj[i].dir.x - (Enemy[proj[i].mod.tracked_index].data.xOrigin - proj[i].data.xOrigin);
				v.y = proj[i].dir.y - (Enemy[proj[i].mod.tracked_index].data.yOrigin - proj[i].data.yOrigin);

				proj[i].dir.x -= v.x * 0.08f * dt; //gradual change of dir, magic number is the rate of change
				proj[i].dir.y -= v.y * 0.08f * dt;
				proj[i].dir = normalise(proj[i].dir);
				//printf("x:%f y:%f\n", proj[i].dir.x, proj[i].dir.y);
			}
			else//update the projectile targeting
			{
				Vector2 v;
				int dist = Game.width * 10, tmp = 0; //abritrary large number
				for (int j = 0; j < MAX_ENEMIES; ++j)
				{
					if (Enemy[j].state == Death || Enemy[j].state == Inactive)
						continue;

					v.x = proj[i].data.xOrigin - Enemy[j].data.xOrigin;
					v.y = proj[i].data.yOrigin - Enemy[j].data.yOrigin;

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
		if (proj[i].type != P_MINE)
		{
			//proj movement dir * speed * deltatime
			proj[i].data.xOrigin += proj[i].dir.x * proj[i].mod.speed * dt;
			proj[i].data.yOrigin += proj[i].dir.y * proj[i].mod.speed * dt;
		}

	}
}

void render_projectile(void)
{
	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		if (!proj[i].isActive)
			continue;

		//render of the projectile here for now
		//CP_Graphics_DrawCircle(proj[i].data.xOrigin, proj[i].data.yOrigin, proj[i].size);

		switch (proj[i].type)
		{
		case P_BASIC:
			RenderNormal(bulletSpriteSheet, bulletArray[0], proj[i].data.xOrigin, proj[i].data.yOrigin, proj[i].size, proj[i].size);
			break;
		case P_SLOW:
			RenderNormal(bulletSpriteSheet, bulletArray[1], proj[i].data.xOrigin, proj[i].data.yOrigin, proj[i].size, proj[i].size);
			break;
		case P_HOMING:
			RenderNormal(bulletSpriteSheet, bulletArray[2], proj[i].data.xOrigin, proj[i].data.yOrigin, proj[i].size, proj[i].size);
			break;
		case P_MINE:
			CP_Settings_Fill(COLOR_BLUE);
			CP_Graphics_DrawCircle(proj[i].data.xOrigin, proj[i].data.yOrigin, proj[i].size);
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
	case P_SLOW:
	{
		for (int i = 0; i < MAX_ENEMIES; ++i)
		{
			//skip dead or inactive
			if (Enemy[i].state == Inactive || Enemy[i].state == Death)
				continue;

			dif.x = Enemy[i].data.xOrigin - p->data.xOrigin;
			dif.y = Enemy[i].data.yOrigin - p->data.yOrigin;
			dist = magnitude_sq(dif);
			if (dist <= SLOW_RANGE * SLOW_RANGE) // will change range to be able to be upgarded ltr
			{
				//printf("Speed[%d]: %f\n", i, Enemy[i].speed);
				Enemy[i].slow_amt = p->mod.slow_amt;
				Enemy[i].slow_timer = p->mod.slow_timer;
				//printf("A_Speed[%d]: %f\n", i, Enemy[i].speed);
			}

		}
		break;
	}
	case P_MINE: /* no break to go into p_homing*/
		//printf("BOOM\n");
	case P_HOMING:
	{
		for (int i = 0; i < MAX_ENEMIES; ++i)
		{
			//skip dead or inactive
			if (Enemy[i].state == Inactive || Enemy[i].state == Death)
				continue;
			if (p->mod.tracked_index == i) //is gonna be dmg in enemy so skip the targeted one, this case will be changed ltr has flaws
				continue;

			dif.x = Enemy[i].data.xOrigin - p->data.xOrigin;
			dif.y = Enemy[i].data.yOrigin - p->data.yOrigin;
			dist = magnitude_sq(dif);
			if (dist <= EXPLOSION_RANGE * EXPLOSION_RANGE) // will change range to be able to be upgarded ltr
			{
				Enemy[i].health -= p->mod.damage;
				Enemy[i].state = Hurt;
				Enemy[i].timer = 0;
			}

		}
		break;
	}
	default:
		break;
	}
}

void update_turretAnimation(Turret* t)
{
	if (t->type == T_MINE)
	{
		if (t->turretAnimTimer >= 2)
		{
			t->animCounter = !t->animCounter;
			t->turretAnimTimer = 0;
		}
	}

	else
	{
		switch (t->currentAnimState)
		{
		case INACTIVE:
			if (t->turretAnimTimer >= 0.35)
			{
				if (t->animCounter >= 2) //0 1 2
				{
					t->animCounter = 0;
				}

				else
				{
					t->animCounter++;
				}

				if (turret->type == T_SLOW)
				{
					t->turret_img = slowTurretImageArray[t->animCounter];
				}
				t->turretAnimTimer = 0;

			}

			break;
		case SHOOTING:
			//if (t->turretAnimTimer >= 0.60)
			if (t->turretAnimTimer >= t->mod.shoot_rate)
			{
				if (t->animCounter >= 5)
				{
					t->animCounter = 3;
				}

				else
				{
					t->animCounter++;
				}
				if (turret->type == T_SLOW)
				{
					t->turret_img = slowTurretImageArray[t->animCounter];
				}
				t->turretAnimTimer = 0;
			}

			break;
		}
	}


}
