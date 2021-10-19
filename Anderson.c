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
		blueSquareClicked = 0;
		redSquareClicked = 0;
		greySquareClicked = 0;
		whiteSquareClicked = 0;

		float distFromXOriginToMouseX = CP_Input_GetMouseX() - X_ORIGIN;
		float distFromYOriginToMouseY = CP_Input_GetMouseY() - Y_ORIGIN;

		float clickedCol = distFromXOriginToMouseX / gridWidth;
		float clickedRow = distFromYOriginToMouseY / gridHeight;

		if ( clickedCol >= 0 && clickedRow >= 0)
		{
			if ((int)clickedCol == Level1.exitCol && (int)clickedRow == Level1.exitRow)
			{
				blueSquareClicked = 1;
			}
			else if ((int)clickedCol == Level1.spawnCol && (int)clickedRow == Level1.spawnRow)
			{
				redSquareClicked = 1;
			}
			else if (((int)clickedRow < Level1.spawnRow && (int)clickedRow > Level1.exitRow) && ((int)clickedCol <= Level1.spawnCol && (int)clickedCol >= Level1.exitCol))
			{
				greySquareClicked = 1;
			}
			else if (withinBoundaries(X_ORIGIN, Y_ORIGIN, X_ORIGIN + gridWidth * Grid_Cols, Y_ORIGIN + gridHeight * Grid_Rows))

			{
				whiteSquareClicked = 1;
			}
		}


		// Tutorial level
		// If click was within red square's two corners ...
		//if (withinBoundaries(X_ORIGIN + gridWidth, Y_ORIGIN, X_ORIGIN + gridWidth * 2, Y_ORIGIN + gridHeight))
		//{
		//	redSquareClicked = 1;
		//}
		// ... or the blue square's ...
		//if (withinBoundaries(X_ORIGIN + gridWidth, Y_ORIGIN + gridHeight * (Grid_Rows - 1), X_ORIGIN + gridWidth * 2, Y_ORIGIN + gridHeight * Grid_Rows))
		//{
		//	blueSquareClicked = 1;
		//}

		//  White area boundaries
		//if (withinBoundaries(X_ORIGIN, Y_ORIGIN, X_ORIGIN + gridWidth, Y_ORIGIN + gridHeight * Grid_Rows) ||
		//	withinBoundaries(X_ORIGIN + gridWidth * 2, Y_ORIGIN, X_ORIGIN + gridWidth * 3, Y_ORIGIN + gridHeight * Grid_Rows))
		//{
		//	whiteSquareClicked = 1;
		//}
		//if (withinBoundaries(X_ORIGIN + gridWidth, Y_ORIGIN + gridHeight, X_ORIGIN + gridWidth * (Grid_Cols - 1), Y_ORIGIN + gridHeight * (Grid_Rows - 1)))
		//{
		//	greySquareClicked = 1;
		//}
	}

	debugSquareFont = GAME_FONT;
	CP_Font_Set(debugSquareFont);
	CP_Settings_TextSize(32.0f);
	if (redSquareClicked)
	{
		CP_Font_DrawText("Clicked a red square", 100.0f, 100.0f);
	}
	if (blueSquareClicked)
	{
		CP_Font_DrawText("Clicked a blue square", 100.0f, 100.0f);
	}
	if (whiteSquareClicked)
	{
		CP_Font_DrawText("Clicked a white square", 100.0f, 100.0f);
	}
	if (greySquareClicked)
	{
		CP_Font_DrawText("Clicked a grey square", 100.0f, 100.0f);
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
