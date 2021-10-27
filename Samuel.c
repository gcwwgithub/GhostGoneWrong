#include "cprocessing.h"
#include "Samuel.h"
#include <math.h>
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
		proj[i].isActive = 0;
		proj[i].data.xOrigin = 0;
		proj[i].data.yOrigin = 0;
		proj[i].damage = 1.f;
		proj[i].speed = 100.f;
	}
	for (int i = 0; i < MAX_TURRET; ++i)
	{
		turret[i].isActive = 0;
		turret[i].size = Game.gridHeight;
		turret[i].angle = 0.f;
		turret[i].cooldown = 0.f;
		turret[i].damage = 1.f;
		v.pos_x = 0;
		v.pos_y = 1;
		turret[i].dir = v;
		turret[i].turretAnimTimer = 0;
		turret[i].turret_img = slowTurretImageArray[0];
		turret[i].currentAnimState = INACTIVE;
		turret[i].animCounter = 0;
	}

	//turret[0].data.xOrigin = Game.xOrigin + (Game.gridWidth * (2 + 0.5f));
	//turret[0].data.yOrigin = Game.yOrigin + (Game.gridHeight * (1 + 0.5f));
	//turret[0].size = Game.gridHeight;
	//turret[0].dir = v;
	//turret[0].angle = 0;
	//turret[0].type = T_TRIANGLE;
	//turret[0].cooldown = 0.f;
	//turret[0].isActive = 1;
	//turret[0].range = Game.gridWidth * 2;
	place_turret(T_TRIANGLE, 2, 1);
	place_turret(T_TRIANGLE, 0, 4);
}

//call this function to palce turret (pass in the grid index)
void place_turret(TurretType type, int index_x, int index_y)
{
	for (int i = 0; i < MAX_TURRET; ++i)
	{
		if (turret[i].isActive)
			continue;

		turret[i].isActive = 1;
		turret[i].type = type;
		//origin + gridwidth * (index + 0.5); (to place the turret on the grid box)
		turret[i].data.xOrigin = Game.xOrigin + (Game.gridWidth * (index_x + 0.5f));
		turret[i].data.yOrigin = Game.yOrigin + (Game.gridHeight * (index_y + 0.5f));
		//edit here for the type range and dmg
		switch (turret[i].type)
		{
		case T_TRIANGLE:
			turret[i].range = Game.gridWidth * 2;
			turret[i].damage = 1;
			break;
		case T_CIRCLE: //placeholder
			turret[i].range = Game.gridWidth * 2;
			turret[i].damage = 1;
			break;
		case T_STAR:
			turret[i].range = Game.gridWidth * 2;
			turret[i].damage = 1;
			break;
		case T_PRECENTAGE:
			turret[i].range = Game.gridWidth * 2;
			turret[i].damage = 1;
			break;
		default:
			break;
		}
		//escape from loop once done
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
		switch (turret[i].type)
		{
		case T_TRIANGLE:
			turret[i].turretAnimTimer += CP_System_GetDt();
			update_turretAnimation(&turret[i]);
			RenderTurret(basicTurretSpriteSheet, basicTurretArray[turret[i].animCounter],
				turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].size, turret[i].size);
			//the +90 degree is to offset the atan2
			break;
		case T_CIRCLE:
			turret[i].turretAnimTimer += CP_System_GetDt();
			update_turretAnimation(&turret[i]);
			CP_Image_DrawAdvanced(turret[i].turret_img, turret[i].data.xOrigin, turret[i].data.yOrigin,
				turret[i].size, turret[i].size, 255, turret[i].angle + 90.f);
			break;
		case T_STAR:
			break;
		case T_PRECENTAGE:
			break;
		default:
			break;
		}
	}
}

void update_turret(void)
{
	//highest wp, enemy index
	int wp = -1, e_index = -1;
	//dist check, targeted enemy direction
	Vector2 v1, targeted_dir;
	v1.pos_x = 0;
	v1.pos_y = 0;
	targeted_dir = v1;

	for (int i = 0; i < MAX_TURRET; ++i)
	{
		if (!turret[i].isActive)
			continue;

		//reset variables
		wp = -1;
		e_index = -1;

		//switch (turret[i].type)
		//{
		//case T_TRIANGLE:
		//	break;
		//case T_CIRCLE:
		//	break;
		//case T_STAR:
		//	break;
		//case T_PRECENTAGE:
		//	break;
		//default:
		//	break;
		//}

		//target enemy
		for (int j = 0; j < MAX_ENEMIES; ++j)
		{
			//skip if dead
			if (Enemy[j].state == Death || Enemy[j].state == Inactive)
				continue;

			//find dist
			v1.pos_x = Enemy[j].data.xOrigin - turret[i].data.xOrigin;
			v1.pos_y = Enemy[j].data.yOrigin - turret[i].data.yOrigin;

			//if in range
			if (magnitude_sq(v1) <= turret[i].range * turret[i].range)
			{
				//target the enemy closest to end goal
				if (Enemy[j].CurrentWaypoint > wp)
				{
					//set the highest waypoint
					wp = Enemy[j].CurrentWaypoint;
					//set the index of enemy to target
					e_index = j;
					//set the targeted enemy dir
					targeted_dir.pos_x = v1.pos_x;
					targeted_dir.pos_y = v1.pos_y;
				}
				else
					continue;

			}
		}

		//if there is a targeted enemy shoot him
		if (e_index >= 0)
		{
			turret[i].currentAnimState = SHOOTING;
			if (turret[i].animCounter <= 2)
			{
				turret[i].animCounter = 3;
			}
			turret[i].dir = targeted_dir;
			turret[i].dir = normalise(turret[i].dir);
			turret[i].angle = atan2f(turret[i].dir.pos_y, turret[i].dir.pos_x) * 180.f / (float)PI;
			turret[i].cooldown -= 1.f * CP_System_GetDt();
			if (turret[i].cooldown <= 0)
			{
				shoot(turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].dir);
				turret[i].cooldown = 2.f;
			}
		}
		else
		{
			turret[i].currentAnimState = INACTIVE;
		}


#if _DEBUG
		//single enemy
		/*if (magnitude_sq(v1) <= turret[i].range * turret[i].range && test.state != Death)
		{
			turret[i].currentAnimState = SHOOTING;
			if (turret[i].animCounter <= 2)
			{
				turret[i].animCounter = 3;
			}
			turret[i].dir = v1;
			turret[i].dir = normalise(turret[i].dir);
			turret[i].angle = atan2f(turret[i].dir.pos_y, turret[i].dir.pos_x) * 180.f / (float)PI;
			turret[i].cooldown -= 1.f * CP_System_GetDt();
			if (turret[i].cooldown <= 0)
			{
				shoot(turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].dir);
				turret[i].cooldown = 2.f;
			}
		}
		else
		{
			turret[i].currentAnimState = INACTIVE;
		}*/
#endif


	}
}

void shoot(float x, float y, Vector2 dir)
{
	//takes the pos of turret and dir turret facing
	//loop to find unactive projectile
	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		if (proj[i].isActive)
			continue;

		//set the projectile as active and other stuff
		proj[i].isActive = 1;
		proj[i].data.xOrigin = x + (float)(PROJ_OFFSET * dir.pos_x);
		proj[i].data.yOrigin = y + (float)(PROJ_OFFSET * dir.pos_y);
		proj[i].dir = dir;
		proj[i].data.width = 10.f;
		proj[i].data.height = 10.f;
		proj[i].data.objectType = objectCircle;
		break;
	}

}

void update_projectile(void)
{
	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		//bounds check
		if (proj[i].isActive &&
			(proj[i].data.xOrigin < 0 || proj[i].data.xOrigin >(float)CP_System_GetDisplayWidth()
				|| proj[i].data.yOrigin < 0 || proj[i].data.yOrigin >(float)CP_System_GetDisplayHeight()))
		{
			//set to inactive
			proj[i].isActive = 0;
			continue;
		}
		if (!proj[i].isActive)
			continue;

		//proj movement dir * speed * deltatime
		proj[i].data.xOrigin += proj[i].dir.pos_x * proj[i].speed * CP_System_GetDt();
		proj[i].data.yOrigin += proj[i].dir.pos_y * proj[i].speed * CP_System_GetDt();

		//collision check here with enemy or enemy can be done in enemy update
#if _DEBUG
		////Test with john's ai
		//Coordinates tmp;
		//tmp.width = Game.gridWidth * 0.25f;
		//tmp.height = Game.gridWidth * 0.25f;
		//tmp.xOrigin = test.data.xOrigin;
		//tmp.yOrigin = test.data.yOrigin;
		//tmp.objectType = objectCircle;
		////if (Collision_Detection(tmp, proj[i].data))
		//if(collision_detection(proj[i]))
		//{
		//	//enemy hit do magical stuff here
		//	proj[i].isActive = 0;
		//}		
#endif
	}
}

void render_projectile(void)
{
	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		if (!proj[i].isActive)
			continue;

		//render of the projectile here for now
		CP_Graphics_DrawCircle(proj[i].data.xOrigin, proj[i].data.yOrigin, 10);
	}
}

void update_turretAnimation(Turret* t)
{
	if (t->type == T_PRECENTAGE)
	{

	}

	else
	{
		switch (t->currentAnimState)
		{
		case INACTIVE:
			if (t->turretAnimTimer >= 0.35)
			{
				if (t->animCounter >= 2)
				{
					t->animCounter = 0;
				}

				else
				{
					t->animCounter++;
				}

				if (turret->type == T_TRIANGLE)
				{
					t->turret_img = slowTurretImageArray[t->animCounter];
				}
				t->turretAnimTimer = 0;

			}

			break;
		case SHOOTING:
			if (t->turretAnimTimer >= 0.65)
			{
				if (t->animCounter >= 5)
				{
					t->animCounter = 3;
				}

				else
				{
					t->animCounter++;
				}
				if (turret->type == T_TRIANGLE)
				{
					t->turret_img = slowTurretImageArray[t->animCounter];
				}
				t->turretAnimTimer = 0;
			}

			break;
		}
	}


}
