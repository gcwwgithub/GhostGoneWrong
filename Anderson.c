#include "cprocessing.h"
#include "game.h"
#include "Anderson.h"

CP_Font debugSquareFont;

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
void detect_grid_square_color(LevelData level)
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
		//else if (withinBoundaries(TurretButton0.xOrigin, TurretButton0.yOrigin, TurretButton0.xOrigin + TurretButton0.width, TurretButton0.yOrigin + TurretButton0.height))
		//{
		//	turretButton0Clicked = 1;
		//}
		//else if (withinBoundaries(TurretButton1.xOrigin, TurretButton1.yOrigin, TurretButton1.xOrigin + TurretButton1.width, TurretButton1.yOrigin + TurretButton1.height))
		//{
		//	turretButton1Clicked = 1;
		//}
		//else if (withinBoundaries(TurretButton2.xOrigin, TurretButton2.yOrigin, TurretButton2.xOrigin + TurretButton2.width, TurretButton2.yOrigin + TurretButton2.height))
		//{
		//	turretButton2Clicked = 1;
		//}
		//else if (withinBoundaries(TurretButton3.xOrigin, TurretButton3.yOrigin, TurretButton3.xOrigin + TurretButton3.width, TurretButton3.yOrigin + TurretButton3.height))
		//{
		//	turretButton3Clicked = 1;
		//}
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

// Terminates game.
void exit_game(void)
{
	CP_Engine_Terminate();
}

#pragma region UI


// Assuming all buttons are rectangles
void init_text_button(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[])
{
	button.buttonData.xOrigin = buttonPosX;
	button.buttonData.yOrigin = buttonPosY;
	button.buttonData.width = buttonWidth;
	button.buttonData.height = buttonHeight;
	button.buttonData.objectType = objectRectangle;
	button.textPositionX = textPosX;
	button.textPositionY = textPosY;
	strcpy_s(button.textString, sizeof(button.textString), string);
}

void render_game_title(void)
{
	debugSquareFont = GAME_FONT;
	CP_Font_Set(debugSquareFont);
	CP_Settings_Fill(COLOR_BLUE);
	CP_Settings_TextSize(20.0f); // also affects the two button text sizes
	CP_Font_DrawText("Vector Defence", CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.25f);
}

void init_play_button(void)
{
	PlayButton.buttonData.xOrigin = CP_System_GetWindowWidth() / 7 * 1.0f;
	PlayButton.buttonData.yOrigin = CP_System_GetWindowHeight() * 0.5f;
	PlayButton.buttonData.width = BUTTON_WIDTH;
	PlayButton.buttonData.height = BUTTON_HEIGHT;

	PlayButton.textPositionX = PlayButton.buttonData.xOrigin + BUTTON_WIDTH / 4;
	PlayButton.textPositionY = PlayButton.buttonData.yOrigin + BUTTON_HEIGHT * 0.8f;
	strcpy_s(PlayButton.textString, sizeof(PlayButton.textString), "Play");
}

void init_quit_button(void)
{
	QuitButton.buttonData.xOrigin = CP_System_GetWindowWidth() / 7 * 5.0f;
	QuitButton.buttonData.yOrigin = CP_System_GetWindowHeight() * 0.5f;
	QuitButton.buttonData.width = BUTTON_WIDTH;
	QuitButton.buttonData.height = BUTTON_HEIGHT;

	QuitButton.textPositionX = QuitButton.buttonData.xOrigin + BUTTON_WIDTH / 4;
	QuitButton.textPositionY = QuitButton.buttonData.yOrigin + BUTTON_HEIGHT * 0.8f;
	strcpy_s(QuitButton.textString, sizeof(QuitButton.textString), "Quit");
}

void init_back_button(void)
{
	BackButton.buttonData.xOrigin = CP_System_GetWindowWidth() / 7 * 3.1f;
	BackButton.buttonData.yOrigin = CP_System_GetWindowHeight() * 0.9f;
	BackButton.buttonData.width = BUTTON_WIDTH;
	BackButton.buttonData.height = BUTTON_HEIGHT;

	BackButton.textPositionX = BackButton.buttonData.xOrigin + BUTTON_WIDTH / 5;
	BackButton.textPositionY = BackButton.buttonData.yOrigin + BUTTON_HEIGHT * 0.8f;
	strcpy_s(BackButton.textString, sizeof(BackButton.textString), "Back");
}

// number of levels is hardcoded.
// LSelect buttons seperated by pure vertical gap that is BUTTON_HEIGHT units long.
void init_level_select_buttons(void)
{
	// should prob move this next line somewhere else
	//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	int c = 0;
	for (int i = 0; i < 5; i++)
	{
		levelButtons[i].buttonData.xOrigin = CP_System_GetWindowWidth() / 2 - BUTTON_WIDTH / 2.0f;
		levelButtons[i].buttonData.yOrigin = (CP_System_GetWindowHeight() / 3 + i * BUTTON_HEIGHT) + i * 25.0f;
		levelButtons[i].buttonData.width = BUTTON_WIDTH + 6.0f;
		levelButtons[i].buttonData.height = BUTTON_HEIGHT;

		levelButtons[i].textPositionX = levelButtons[i].buttonData.xOrigin + BUTTON_WIDTH * 0.0f;
		levelButtons[i].textPositionY = levelButtons[i].buttonData.yOrigin + BUTTON_HEIGHT / 1.2f;
		char levelNumberText[8];
		c = snprintf(levelNumberText, 8, "Level %d", i + 1); // write text
		strcpy_s(levelButtons[i].textString, sizeof(levelButtons[i].textString), levelNumberText);
	}
}

void render_ui_button(Button button)
{
	CP_Settings_Fill(COLOR_BLACK);
	CP_Graphics_DrawRect(button.buttonData.xOrigin, button.buttonData.yOrigin, button.buttonData.width, button.buttonData.height);
	CP_Settings_Fill(COLOR_WHITE);
	CP_Font_DrawText(button.textString, button.textPositionX, button.textPositionY);
}

void render_level_select_buttons(void)
{
	// render the 5 level buttons
	for (int i = 0; i < 5; i++)
	{
		render_ui_button(levelButtons[i]);
	}
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