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
		redSquareClicked = 0;
		blueSquareClicked = 0;
		whiteSquareClicked = 0;
		greySquareClicked = 0;

		// If click was within red square's two corners ...
		if (withinBoundaries(X_ORIGIN + gridWidth, Y_ORIGIN, X_ORIGIN + gridWidth * 2, Y_ORIGIN + gridHeight))
		{
			redSquareClicked = 1;
		}
		// ... or the blue square's ...
		if (withinBoundaries(X_ORIGIN + gridWidth, Y_ORIGIN + gridHeight * (GRID_ROWS - 1), X_ORIGIN + gridWidth * 2, Y_ORIGIN + gridHeight * GRID_ROWS))
		{
			blueSquareClicked = 1;
		}
		// ... or the left and right white areas' ...
		if (withinBoundaries(X_ORIGIN, Y_ORIGIN, X_ORIGIN + gridWidth, Y_ORIGIN + gridHeight * GRID_ROWS) ||
			withinBoundaries(X_ORIGIN + gridWidth * 2, Y_ORIGIN, X_ORIGIN + gridWidth * 3, Y_ORIGIN + gridHeight * GRID_ROWS))
		{
			whiteSquareClicked = 1;
		}
		if (withinBoundaries(X_ORIGIN + gridWidth, Y_ORIGIN + gridHeight, X_ORIGIN + gridWidth * (GRID_COLS - 1), Y_ORIGIN + gridHeight * (GRID_ROWS - 1)))
		{
			greySquareClicked = 1;
		}
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

