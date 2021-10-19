#include "cprocessing.h"

#define grid_Cols 10
#define grid_Rows 10

#define color_Black  CP_Color_Create(255, 255, 255, 255)
#define color_Grey CP_Color_Create(128, 128, 128, 255)

float gridRowsLength, gridColsLength;
void grid(void);

void game_init(void)
{
	CP_System_Fullscreen();
	gridRowsLength = CP_System_GetDisplayWidth() / grid_Rows;
	gridColsLength = CP_System_GetDisplayHeight() / grid_Cols;
}

void game_update(void)
{
}


void game_exit(void)
{

}
