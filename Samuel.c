#include "cprocessing.h"
#include "Samuel.h"
#include <math.h>
#include <stdio.h>
#include "game.h"


void turret_init(void)
{
	//hard code test values
	test.pos_x = GAME_X_ORIGIN - 10;
	test.pos_y = GAME_Y_ORIGIN - 10;
	test.size = Game_Grid_Height;
	Vector2 v;
	v.pos_x = -3;
	v.pos_y = 1;
	test.dir = v;
	test.angle = 0;
	test.type = TRIANGLE;
	test.cooldown = 0.f;
	turret_img = CP_Image_Load("./Assets/Turret.png");
	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		proj[i].isActive = 0;
		proj[i].x = 0;
		proj[i].y = 0;
	}
}

void render_turret(Turret* t)
{
	//draw type of turrets
	switch (t->type)
	{
	case TRIANGLE:
		CP_Image_DrawAdvanced(turret_img, t->pos_x, t->pos_y, t->size, t->size, 255, t->angle);
		/*CP_Graphics_DrawTriangleAdvanced(t.pos_x - t.size, t.pos_y, t.pos_x + t.size, t.pos_y,
			t.pos_x, t.pos_y + t.size, t.angle);*/
		break;
	case CIRCLE:
		//CP_Image_DrawAdvanced(turret_img, t->pos_x, t->pos_y, t->size, t->size, 255, t->angle);
		break;
	case STAR:
		//CP_Image_DrawAdvanced(turret_img, t->pos_x, t->pos_y, t->size, t->size, 255, t->angle);
		break;
	case PRECENTAGE:
		//CP_Image_DrawAdvanced(turret_img, t->pos_x, t->pos_y, t->size, t->size, 255, t->angle);
		break;
	default:
		break;
	}
}

//void render_turret(void)
//{
//	for (int i = 0; i < MAX_TURRET; ++i)
//	{
//		//draw type of turrets
//		switch (turret[i].type)
//		{
//		case TRIANGLE:
//			CP_Image_DrawAdvanced(turret_img, turret[i].pos_x, turret[i].pos_y, turret[i].size, turret[i].size, 255, turret[i].angle);
//			break;
//		case CIRCLE:
//			break;
//		case STAR:
//			break;
//		default:
//			break;
//		}
//	}
//}

void update_turret(Turret* t) //take in enemy array or some stuff
{
	/*
		check if in range
		looktoenemy = tpos - epos
		normalise(looktoenemy)
		[all turr intial vector points (0,1)]
		shoot (bullet render,update ==)
	*/
	//Vector2 v;
	//v.pos_x = CP_Input_GetMouseX() - t.pos_x; //mouse pos x - tri pos x
	//v.pos_y = CP_Input_GetMouseY() - t.pos_y; //mouse pos y - tri pos y (turr pos - enmy pos)
	//bullet(t.pos_x, t.pos_y, t.dir);

	//rotate base off mouse for now (switch to base on enemy once in range ltr) 
	
	t->dir.pos_x = CP_Input_GetMouseX() - t->pos_x;
	t->dir.pos_y = CP_Input_GetMouseY() - t->pos_y;
	//normalise the vector
	t->dir = normalise(t->dir);
	//get angle to rotate
	t->angle = atan2f(t->dir.pos_y, t->dir.pos_x) * 180.f / (float)PI;

	/*float angle = dot(normalise(t->dir), t->n_dir);
	t->angle = acosf(angle) * 180.f / (float)PI;*/

	//printf("%f\n", t->angle);
 
	//shooting here is placeholder
	//if(CP_Input_MouseTriggered(MOUSE_BUTTON_1)) //for now click to shoot
	t->cooldown -= 1.f * CP_System_GetDt();
	if (t->cooldown <= 0)
	{
		shoot(t->pos_x, t->pos_y, t->dir);
		t->cooldown = 2.f;
	}
}

//void update_turret(void)
//{
//	for (int i = 0; i < MAX_TURRET; ++i)
//	{
//		//rotate base off mouse for now (switch to base on enemy once in range ltr) 
//		turret[i].dir.pos_x = CP_Input_GetMouseX() - turret[i].pos_x;
//		turret[i].dir.pos_y = CP_Input_GetMouseY() - turret[i].pos_y;
//		//normalise the vector
//		turret[i].dir = normalise(turret[i].dir);
//		//get angle to rotate
//		turret[i].angle = atan2f(turret[i].dir.pos_y, turret[i].dir.pos_x) * 180.f / (float)PI + 90.f; //magic 90 will fix  
//
//	}
//}

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
		proj[i].x = x;
		proj[i].y = y;
		proj[i].dir = dir;
		break;
	}

}

void projectile_update()
{
	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		//bounds check
		if (proj[i].isActive && 
			(proj[i].x < 0 || proj[i].x > (float)CP_System_GetDisplayWidth() 
				|| proj[i].y < 0 || proj[i].y >(float)CP_System_GetDisplayHeight()))
		{
			//set to inactive
			proj[i].isActive = 0;
			continue;
		}
		if (!proj[i].isActive)
			continue;

		//proj movement dir * speed * deltatime
		proj[i].x += proj[i].dir.pos_x * 100.f * CP_System_GetDt();
		proj[i].y += proj[i].dir.pos_y * 100.f * CP_System_GetDt();

		//render of the projectile here for now
		CP_Graphics_DrawCircle(proj[i].x, proj[i].y, 10);
		//draw_projectile(proj[i]);
	}
}

//void draw_projectile(Projectile p)
//{
//	CP_Graphics_DrawCircle(p.x, p.y, 10);
//}
