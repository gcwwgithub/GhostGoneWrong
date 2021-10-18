#include "cprocessing.h"

#define CIRCLE_SIZE 30
float gCirclePositionX;
float gCirclePositionY;

void game_init(void)
{
	gCirclePositionX = 0.f;
	gCirclePositionY = 0.f;
}

void game_update(void)
{
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) {
		gCirclePositionX = CP_Input_GetMouseX();
		gCirclePositionY = CP_Input_GetMouseY();
	}
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_Fill(CP_Color_Create((int)gCirclePositionX,(int)gCirclePositionY,0,255));
	CP_Graphics_DrawCircle(gCirclePositionX, gCirclePositionY, CIRCLE_SIZE);
	/********************************************************
	 * TODO: Your implementation here. Steps are as follows:
	 * 1. Update gCirclePosition based on input
	 * 2. Clear the background to any color you choose.
	 * 3. Draw a circle:
	 *    - Its position should be gCirclePosition's x,y value
	 *    - Its radius should be CIRCLE_SIZE
	 *******************************************************/
}


void game_exit(void)
{

}
