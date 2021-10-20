#include <stdio.h>
#include "cprocessing.h"
#include "game.h"

CP_Font debugSquareFont;
int redSquareClicked = 0;
int blueSquareClicked = 0;
int whiteSquareClicked = 0;
int greySquareClicked = 0;

// Taking top left & bottom right corners.
int withinBoundaries(float minX, float minY, float maxX, float maxY)
{
	if (CP_Input_GetMouseX() > minX && CP_Input_GetMouseX() < maxX)
	{
		if (CP_Input_GetMouseY() > minY && CP_Input_GetMouseY() < maxY)
		{
			return 1;
		}
	}
	return 0;
}

// This should output the color of the square that is clicked.
// This is somewhat hard coded to the grid layout.
void click_on_square(void)
{
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		whiteSquareClicked = 0;
		greySquareClicked = 0;
		redSquareClicked = 0;
		blueSquareClicked = 0;

		float distFromXOriginToMouseX = CP_Input_GetMouseX() - Game.xOrigin;
		float distFromYOriginToMouseY = CP_Input_GetMouseY() - Game.yOrigin;

		float clickedCol = distFromXOriginToMouseX / Game.gridWidth;
		float clickedRow = distFromYOriginToMouseY / Game.gridHeight;

		// Only able to account for straight paths
		if ((clickedCol >= 0 && clickedCol < GAME_GRID_COLS) && (clickedRow >= 0 && clickedRow < GAME_GRID_ROWS))
		{
			int color = Tutorial.gridColor[(int)clickedRow][(int)clickedCol];
			switch (color)
			{
			case (Grid_Color_White):
			{
				whiteSquareClicked = 1;
				break;
			}
			case (Grid_Color_Grey):
			{
				greySquareClicked = 1;
				break;
			}
			case (Grid_Color_Red):
			{
				redSquareClicked = 1;
				break;
			}
			case (Grid_Color_Blue):
			{
				blueSquareClicked = 1;
				break;
			}
			}

		}


		// Tutorial level
		// If click was within red square's two corners ...
		//if (withinBoundaries(Game.xOrigin + Game.gridWidth, Game.yOrigin, Game.xOrigin + Game.gridWidth * 2, Game.yOrigin + Game.gridHeight))
		//{
		//	redSquareClicked = 1;
		//}
		// ... or the blue square's ...
		//if (withinBoundaries(Game.xOrigin + Game.gridWidth, Game.yOrigin + Game.gridHeight * (Grid_Rows - 1), Game.xOrigin + Game.gridWidth * 2, Game.yOrigin + Game.gridHeight * Grid_Rows))
		//{
		//	blueSquareClicked = 1;
		//}

		//  White area boundaries
		//if (withinBoundaries(Game.xOrigin, Game.yOrigin, Game.xOrigin + Game.gridWidth, Game.yOrigin + Game.gridHeight * Grid_Rows) ||
		//	withinBoundaries(Game.xOrigin + Game.gridWidth * 2, Game.yOrigin, Game.xOrigin + Game.gridWidth * 3, Game.yOrigin + Game.gridHeight * Grid_Rows))
		//{
		//	whiteSquareClicked = 1;
		//}
		//if (withinBoundaries(Game.xOrigin + Game.gridWidth, Game.yOrigin + Game.gridHeight, Game.xOrigin + Game.gridWidth * (Grid_Cols - 1), Game.yOrigin + Game.gridHeight * (Grid_Rows - 1)))
		//{
		//	greySquareClicked = 1;
		//}
	}

	debugSquareFont = GAME_FONT;
	CP_Font_Set(debugSquareFont);
	CP_Settings_TextSize(32.0f);
	if (redSquareClicked)
	{
		CP_Font_DrawText("Red square", 100.0f, 100.0f);
	}
	if (blueSquareClicked)
	{
		CP_Font_DrawText("Blue square", 100.0f, 100.0f);
	}
	if (whiteSquareClicked)
	{
		CP_Font_DrawText("White square", 100.0f, 100.0f);
	}
	if (greySquareClicked)
	{
		CP_Font_DrawText("Grey square", 100.0f, 100.0f);
	}
}

// This assumes the pause button's a square.
void click_on_pause(CP_Image pauseButton, float imagePosX, float imagePosY)
{
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		if (withinBoundaries(imagePosX, imagePosY, imagePosX + CP_Image_GetWidth(pauseButton), imagePosY + CP_Image_GetHeight(pauseButton)))
		{
			// game is paused.
		}
	}
}
