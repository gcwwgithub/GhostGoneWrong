#include "cprocessing.h"
#include "Samuel.h"
#include <math.h>

#if _DEBUG
#include <stdio.h>
#include "John.h"
#endif

void turret_init(void)
{
	//hard code test values
	Vector2 v;
	turret_img = CP_Image_Load("./Assets/Turret.png");

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
		turret[i].cooldown = 0.f;
		turret[i].angle = 0.f;
		v.pos_x = 0;
		v.pos_y = 1;
		turret[i].dir = v;
		turret[i].size = Game.gridHeight;
		turret[i].damage = 1.f;
	}
								
	turret[0].data.xOrigin = Game.xOrigin + (Game.gridWidth * (2 + 0.5f));
	turret[0].data.yOrigin = Game.yOrigin + (Game.gridHeight * (1 + 0.5f));
	turret[0].size = Game.gridHeight;
	turret[0].dir = v;
	turret[0].angle = 0;
	turret[0].type = T_TRIANGLE;
	turret[0].cooldown = 0.f;
	turret[0].isActive = 1;
	turret[0].range = Game.gridWidth * 2;

	#if _DEBUG
	//test stuff with john's code
	enemy_test_init();
#endif
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
		turret[i].data.xOrigin = Game.xOrigin + Game.gridWidth * (index_x + 0.5f);
		turret[i].data.yOrigin = Game.xOrigin + Game.gridWidth * (index_y + 0.5f);
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

//void render_turret(Turret* t)
//{
//	//draw type of turrets
//	switch (t->type)
//	{
//	case T_TRIANGLE:
//		CP_Image_DrawAdvanced(turret_img, t->data.xOrigin, t->data.yOrigin, t->size, t->size, 255, t->angle);
//		/*CP_Graphics_DrawTriangleAdvanced(t.pos_x - t.size, t.pos_y, t.pos_x + t.size, t.pos_y,
//			t.pos_x, t.pos_y + t.size, t.angle);*/
//		break;
//	case T_CIRCLE:
//		//CP_Image_DrawAdvanced(turret_img, t->pos_x, t->pos_y, t->size, t->size, 255, t->angle);
//		break;
//	case T_STAR:
//		//CP_Image_DrawAdvanced(turret_img, t->pos_x, t->pos_y, t->size, t->size, 255, t->angle);
//		break;
//	case T_PRECENTAGE:
//		//CP_Image_DrawAdvanced(turret_img, t->pos_x, t->pos_y, t->size, t->size, 255, t->angle);
//		break;
//	default:
//		break;
//	}
//}

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
			CP_Image_DrawAdvanced(turret_img, turret[i].data.xOrigin, turret[i].data.yOrigin,
				turret[i].size, turret[i].size, 255, turret[i].angle);
			break;
		case T_CIRCLE:
			break;
		case T_STAR:
			break;
		case T_PRECENTAGE:
			break;
		default:
			break;
		}
	}
#if _DEBUG
	//test stuff with john's code
	Draw_enemy(&test);
#endif
}

/*
void update_turret(Turret* t) //take in enemy array or some stuff
{
	//check if in range
	//looktoenemy = tpos - epos
	//normalise(looktoenemy)
	//[all turr intial vector points (0,1)]
	//shoot (bullet render,update ==)

	//Vector2 v;
	//v.pos_x = CP_Input_GetMouseX() - t.pos_x; //mouse pos x - tri pos x
	//v.pos_y = CP_Input_GetMouseY() - t.pos_y; //mouse pos y - tri pos y (turr pos - enmy pos)
	//bullet(t.pos_x, t.pos_y, t.dir);

	//rotate base off mouse for now (switch to base on enemy once in range ltr) 
	
	t->dir.pos_x = CP_Input_GetMouseX() - t->data.xOrigin;
	t->dir.pos_y = CP_Input_GetMouseY() - t->data.yOrigin;
	//normalise the vector
	t->dir = normalise(t->dir);
	//get angle to rotate
	t->angle = atan2f(t->dir.pos_y, t->dir.pos_x) * 180.f / (float)PI;

	//float angle = dot(normalise(t->dir), t->n_dir);
	//t->angle = acosf(angle) * 180.f / (float)PI;

	//printf("%f\n", t->angle);
 
	//shooting here is placeholder
	//if(CP_Input_MouseTriggered(MOUSE_BUTTON_1)) //for now click to shoot
	t->cooldown -= 1.f * CP_System_GetDt();
	if (t->cooldown <= 0)
	{
		shoot(t->data.xOrigin, t->data.yOrigin, t->dir);
		t->cooldown = 2.f;
	}
}
*/

void update_turret(void)
{
	for (int i = 0; i < MAX_TURRET; ++i)
	{
		if (!turret[i].isActive)
			continue;

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

#if _DEBUG
		//Debug test code with enemy
		Vector2 v1;
		v1.pos_x = test.posX - turret[i].data.xOrigin;
		v1.pos_y = test.posY - turret[i].data.yOrigin;
		if (magnitude_sq(v1) <= turret[i].range * turret[i].range)
		{
			turret[i].dir = v1;;
			turret[i].dir = normalise(turret[i].dir);
			turret[i].angle = atan2f(turret[i].dir.pos_y, turret[i].dir.pos_x) * 180.f / (float)PI;
			turret[i].cooldown -= 1.f * CP_System_GetDt();
			if (turret[i].cooldown <= 0)
			{
				shoot(turret[i].data.xOrigin, turret[i].data.yOrigin, turret[i].dir);
				turret[i].cooldown = 2.f;
			}
		}
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
		proj[i].data.xOrigin = x;
		proj[i].data.yOrigin = y;
		proj[i].dir = dir;
		proj[i].data.width = 5.f;
		proj[i].data.height = 5.f;
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
			(proj[i].data.xOrigin < 0 || proj[i].data.xOrigin > (float)CP_System_GetDisplayWidth()
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
		//Test with john's ai
		Coordinates tmp;
		tmp.width = Game.gridWidth * 0.25f;
		tmp.height = Game.gridWidth * 0.25f;
		tmp.xOrigin = test.posX;
		tmp.yOrigin = test.posY;
		tmp.objectType = objectCircle;
		if (Collision_Detection(tmp,proj[i].data))
		{
			//enemy hit do magical stuff here
			proj[i].isActive = 0;
		}
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
