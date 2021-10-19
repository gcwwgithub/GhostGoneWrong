#include "cprocessing.h"
#include"game.h"

float gridRowsLength, gridColsLength;
void grid(void);

void game_init(void)
{
	CP_System_Fullscreen();
	gridRowsLength = (float)CP_System_GetDisplayHeight() / grid_Rows;
	gridColsLength = (float)CP_System_GetDisplayWidth() / grid_Cols;
}

void game_update(void)
{
	grid();
}


void game_exit(void)
{

}