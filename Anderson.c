#include "cprocessing.h"
#include "game.h"

CP_Font debugSquareFont;

ObjectData pauseButton;

int redSquareClicked = 0;
int blueSquareClicked = 0;
int whiteSquareClicked = 0;
int greySquareClicked = 0;

int turretButton0Clicked = 0;
int turretButton1Clicked = 0;
int turretButton2Clicked = 0;
int turretButton3Clicked = 0;

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
// This is dependent on the level set input.
void click_on_square(LevelData level)
{
	int color = 4; // max value of GridState + 1
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		whiteSquareClicked = 0;
		greySquareClicked = 0;
		redSquareClicked = 0;
		blueSquareClicked = 0;

		turretButton0Clicked = 0;
		turretButton1Clicked = 0;
		turretButton2Clicked = 0;
		turretButton3Clicked = 0;

		float distFromXOriginToMouseX = CP_Input_GetMouseX() - Game.xOrigin;
		float distFromYOriginToMouseY = CP_Input_GetMouseY() - Game.yOrigin;

		float clickedCol = distFromXOriginToMouseX / Game.gridWidth;
		float clickedRow = distFromYOriginToMouseY / Game.gridHeight;

		// clicked within the grid
		if ((clickedCol >= 0 && clickedCol < GAME_GRID_COLS) && (clickedRow >= 0 && clickedRow < GAME_GRID_ROWS))
		{
			color = level.gridColor[(int)clickedRow][(int)clickedCol];
			switch (color)
			{
			case (Grid_Color_White):
			{
				whiteSquareClicked = 1;
				// if (playerIsBuildingTurret) // i.e. clicked on turret to buy and now hovering over this grid cell
				//		if  // there are no more white-colored grid cells left. || (!turret_space_occupied) 
				//			// put new turret at center of white grid cell.
				//			// and remove the cost of said turret from current points
				//		else // display error message
				//			

				// else if (turret_space_occupied) // there is turret here
				//		// select turret - maybe put something on it as visual feedback
				//		// enable turret window,upgrade menu,etc.

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
		// if clicked outside the grid
		else if (withinBoundaries(TurretButton0.xOrigin, TurretButton0.yOrigin, TurretButton0.xOrigin + TurretButton0.width, TurretButton0.yOrigin + TurretButton0.height))
		{
			turretButton0Clicked = 1;
		}
		else if (withinBoundaries(TurretButton1.xOrigin, TurretButton1.yOrigin, TurretButton1.xOrigin + TurretButton1.width, TurretButton1.yOrigin + TurretButton1.height))
		{
			turretButton1Clicked = 1;
		}
		else if (withinBoundaries(TurretButton2.xOrigin, TurretButton2.yOrigin, TurretButton2.xOrigin + TurretButton2.width, TurretButton2.yOrigin + TurretButton2.height))
		{
			turretButton2Clicked = 1;
		}
		else if (withinBoundaries(TurretButton3.xOrigin, TurretButton3.yOrigin, TurretButton3.xOrigin + TurretButton3.width, TurretButton3.yOrigin + TurretButton3.height))
		{
			turretButton3Clicked = 1;
		}
	}

	debugSquareFont = GAME_FONT;
	CP_Font_Set(debugSquareFont);
	CP_Settings_Fill(COLOR_RED);
	CP_Settings_TextSize(32.0f);
	if (redSquareClicked)
	{
		CP_Font_DrawText("Red square", CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.1f);
	}
	if (blueSquareClicked)
	{
		CP_Font_DrawText("Blue square", CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.1f);
	}
	if (whiteSquareClicked)
	{
		CP_Font_DrawText("White square", CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.1f);
	}
	if (greySquareClicked)
	{
		CP_Font_DrawText("Grey square", CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.1f);
	}

	if (turretButton0Clicked)
	{
		CP_Font_DrawText("Turret button 0", CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.1f);
	}
	else if (turretButton1Clicked)
	{
		CP_Font_DrawText("Turret button 1", CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.1f);
	}
	else if (turretButton2Clicked)
	{
		CP_Font_DrawText("Turret button 2", CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.1f);
	}
	else if (turretButton3Clicked)
	{
		CP_Font_DrawText("Turret button 3", CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.1f);
	}
}

/*
void draw_pause_button(float buttonPosX, float buttonPosY)
{
	CP_Settings_Fill(COLOR_BLACK);
	CP_Graphics_DrawRect(buttonPosX, buttonPosY, buttonPosX + 25.0f, buttonPosY + 15.0f);
	CP_Settings_Fill(COLOR_WHITE);
	CP_Graphics_DrawRect(buttonPosX * 1.4f, buttonPosY * 1.1f, buttonPosX * 0.25f, 45.0f);
	CP_Graphics_DrawRect(buttonPosX * 1.9f, buttonPosY * 1.1f, buttonPosX * 0.25f, 45.0f);
}
*/

void click_on_pause(float x, float y)
{
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		//if (withinBoundaries(imagePosX, imagePosY, imagePosX + CP_Image_GetWidth(pauseButton), imagePosY + CP_Image_GetHeight(pauseButton)))
		{
			// game is paused.
		}
	}
}

// Terminates game.
void exit_game(void)
{
	CP_Engine_Terminate();
}

#pragma region UI

void text_button_constructor(float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[])
{
	CP_Graphics_DrawRect(buttonPosX, buttonPosY, buttonWidth, buttonHeight);
	CP_Font_DrawText(string, textPosX, textPosY);
}

void main_menu_buttons(void)
{
	// functionalities of the 3 main menu buttons
}

void render_mmenu_buttons(void)
{
	// render the main menu buttons

	//CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 7 * 2, )
	//text_button_constructor(CP_System_GetWindowWidth() / 7 * 2.0f, CP_System_GetWindowHeight() * 0.75f, 50.0f, 20.0f, CP_System_GetWindowWidth() / 7 * 2 + 25.0f, CP_System_GetWindowHeight() * 0.75f + 10.0f, "Play");

}

void level_select_buttons(void)
{
	// functionalities of the 5 level buttons
}

void render_level_select_buttons(void)
{
	// render the 5 level buttons
}

void credits_screen(void)
{
	// just render text and other things here
}

void main_game_screen(void)
{
	// render game screen here
}

#pragma endregion