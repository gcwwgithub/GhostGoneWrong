#include "cprocessing.h"

#define grid_Cols 10
#define grid_Rows 10

void grid(void);

void game_init(void)
{
	CP_Color colorBlack = CP_Color_Create(255, 255, 255, 255), colorGrey = CP_Color_Create(128, 128, 128, 255);
	float gridRowsLength = CP_System_GetDisplayWidth() / grid_Rows, gridColsLength = CP_System_GetDisplayHeight() / grid_Cols;
	extern colorBlack, colorGrey, gridRowsLength, gridColsLength;
	CP_System_Fullscreen();
}

void game_update(void)
{
}


void game_exit(void)
{

}
