#include "cprocessing.h"
#include "game.h"
#include "Anderson.h"

// text currently aligned to horizontal_center, vertical_center of text box.
// init_play_button(...)

int enemiesOnScreen = 0;

CP_Font debugSquareFont;

int redSquareClicked = 0;
int blueSquareClicked = 0;
int whiteSquareClicked = 0;
int greySquareClicked = 0;

int turretButton0Clicked = 0;
float buildingTime = BUILDING_PHASE_TIME;


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

		float distFromXOriginToMouseX = CP_Input_GetMouseX() - Game.xOrigin;
		float distFromYOriginToMouseY = CP_Input_GetMouseY() - Game.yOrigin;

		float clickedCol = distFromXOriginToMouseX / Game.gridWidth;
		float clickedRow = distFromYOriginToMouseY / Game.gridHeight;

		// clicked within the grid
		if ((clickedCol >= 0 && clickedCol < GAME_GRID_COLS) && (clickedRow >= 0 && clickedRow < GAME_GRID_ROWS))
		{
			color = level.grid[(int)clickedRow][(int)clickedCol].type;
			switch (color)
			{
			case (Clear):
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
			case (Path):
			{
				greySquareClicked = 1;
				break;
			}
			case (Spawn):
			{
				redSquareClicked = 1;
				break;
			}
			case (Exit):
			{
				blueSquareClicked = 1;
				break;
			}
			}
		}
		// if clicked outside the grid
		//else if (withinBoundaries(GameMenuObject[0].xOrigin, GameMenuObject[0].yOrigin, GameMenuObject[0].xOrigin + GameMenuObject[0].width, GameMenuObject[0].yOrigin + GameMenuObject[0].height))
		//{
		//	turretButton0Clicked = 1;
		//}
	}

	debugSquareFont = GAME_FONT;
	CP_Font_Set(debugSquareFont);
	CP_Settings_Fill(COLOR_RED);
	CP_Settings_TextSize(FONT_SIZE);
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
}

void reduce_building_phase_time()
{
	if (buildingTime < 0.0f) currentGameState = Wave;
	buildingTime -= CP_System_GetDt();
}

#pragma region UI

// Assuming all buttons are rectangles
// Attempted soft coding of UI button initialisation - somewhat didn't work so is now just used for reference.
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

void init_play_button(void)
{
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	PlayButton.buttonData.xOrigin = CP_System_GetWindowWidth() / 5.0f;
	PlayButton.buttonData.yOrigin = CP_System_GetWindowHeight() / 2.0f;
	PlayButton.buttonData.width = BUTTON_WIDTH;
	PlayButton.buttonData.height = BUTTON_HEIGHT;

	PlayButton.textPositionX = PlayButton.buttonData.xOrigin + BUTTON_WIDTH / 2;
	PlayButton.textPositionY = PlayButton.buttonData.yOrigin + BUTTON_HEIGHT / 2;
	strcpy_s(PlayButton.textString, sizeof(PlayButton.textString), "Play");
}

void init_quit_button(void)
{
	QuitButton.buttonData.xOrigin = CP_System_GetWindowWidth() / 1.5f;
	QuitButton.buttonData.yOrigin = CP_System_GetWindowHeight() / 2.0f;
	QuitButton.buttonData.width = BUTTON_WIDTH;
	QuitButton.buttonData.height = BUTTON_HEIGHT;

	QuitButton.textPositionX = QuitButton.buttonData.xOrigin + BUTTON_WIDTH / 2;
	QuitButton.textPositionY = QuitButton.buttonData.yOrigin + BUTTON_HEIGHT / 2;
	strcpy_s(QuitButton.textString, sizeof(QuitButton.textString), "Quit");
}

void init_back_button(void)
{
	BackButton.buttonData.xOrigin = CP_System_GetWindowWidth() / 2 + BUTTON_WIDTH / 0.5f;
	BackButton.buttonData.yOrigin = CP_System_GetWindowHeight() * 0.8f;
	BackButton.buttonData.width = BUTTON_WIDTH;
	BackButton.buttonData.height = BUTTON_HEIGHT;

	BackButton.textPositionX = BackButton.buttonData.xOrigin + BUTTON_WIDTH / 2;
	BackButton.textPositionY = BackButton.buttonData.yOrigin + BUTTON_HEIGHT / 2;
	strcpy_s(BackButton.textString, sizeof(BackButton.textString), "Back");
}

void init_pause_quit_button(void)
{
	PauseQuitButton.buttonData.xOrigin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH / 2;
	PauseQuitButton.buttonData.yOrigin = CP_System_GetWindowHeight() * 0.4f - BUTTON_HEIGHT / 2;
	PauseQuitButton.buttonData.width = BUTTON_WIDTH;
	PauseQuitButton.buttonData.height = BUTTON_HEIGHT;

	PauseQuitButton.textPositionX = PauseQuitButton.buttonData.xOrigin + BUTTON_WIDTH / 2;
	PauseQuitButton.textPositionY = PauseQuitButton.buttonData.yOrigin + BUTTON_HEIGHT / 2;
	strcpy_s(PauseQuitButton.textString, sizeof(PauseQuitButton.textString), "Quit");
}

void init_pause_back_button(void)
{
	PauseBackButton.buttonData.xOrigin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH / 2;
	PauseBackButton.buttonData.yOrigin = CP_System_GetWindowHeight() * 0.3f - BUTTON_HEIGHT / 2;
	PauseBackButton.buttonData.width = BUTTON_WIDTH;
	PauseBackButton.buttonData.height = BUTTON_HEIGHT;

	PauseBackButton.textPositionX = PauseBackButton.buttonData.xOrigin + BUTTON_WIDTH / 2;
	PauseBackButton.textPositionY = PauseBackButton.buttonData.yOrigin + BUTTON_HEIGHT / 2;
	strcpy_s(PauseBackButton.textString, sizeof(PauseBackButton.textString), "Back");
}

// number of levels is hardcoded.
// LSelect buttons seperated by pure vertical gap that is 25.0f units long.
void init_level_select_buttons(void)
{
	int c = 0;
	for (int i = 0; i < 5; i++)
	{
		levelButtons[i].buttonData.xOrigin = CP_System_GetWindowWidth() / 2 - BUTTON_WIDTH / 2.0f;
		levelButtons[i].buttonData.yOrigin = CP_System_GetWindowHeight() / 3 + i * (BUTTON_HEIGHT + 25.0f);
		levelButtons[i].buttonData.width = BUTTON_WIDTH + 6.0f;
		levelButtons[i].buttonData.height = BUTTON_HEIGHT;

		levelButtons[i].textPositionX = levelButtons[i].buttonData.xOrigin + BUTTON_WIDTH / 2.0f;
		levelButtons[i].textPositionY = levelButtons[i].buttonData.yOrigin + BUTTON_HEIGHT / 2.0f;
		char levelNumberText[8];
		c = snprintf(levelNumberText, 8, "Level %d", i + 1); // write text
		strcpy_s(levelButtons[i].textString, sizeof(levelButtons[i].textString), levelNumberText);
	}
}

void init_pause_screen(void)
{
	init_pause_back_button();
	init_pause_quit_button();
}

void render_wave_timer_text(void)
{
	CP_Settings_Fill(COLOR_BLUE);
	CP_Settings_TextSize(FONT_SIZE);

	CP_Font_DrawText("Math Gone Wrong", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.25f);

}

//TODO
void init_skip_wave_button(void)
{
	// Init skip wave button's position.
}

void init_win_screen(void)
{

}

void init_lose_screen(void)
{

}



// Currently the score is really just a text output
void render_score_text(int score)
{
	const char scoreString[] = { 0 };
	sprintf_s(scoreString, sizeof(scoreString), "Score: %d", score);
	CP_Font_DrawText(scoreString, CP_System_GetWindowWidth() * 0.75f, CP_System_GetWindowHeight() * 0.1f);
}

void render_game_title(void)
{
	RenderWithAlphaChanged(backgroundSpriteSheet, backgroundArray[0], CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 150);
	debugSquareFont = GAME_FONT;
	CP_Font_Set(debugSquareFont);
	//CP_Settings_Fill(COLOR_BLUE);
	CP_Settings_TextSize(FONT_SIZE);
	CP_Image_Draw(titleWordImage, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.2f, 256, 256, 255);
	//CP_Font_DrawText("Math Gone Wrong", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.25f);
	
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

// Currently intended to be just text. Though I do have plans for this to be more than that.
void render_credits_screen(void)
{
	// just render text and other things here
}

void render_pause_screen(void)
{
	// The following shall be rendered here:
	//	Restart button tbd

	CP_Settings_Fill(COLOR_GREY);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.25f, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.2f);
	render_ui_button(PauseBackButton);
	render_ui_button(PauseQuitButton);

}

void render_win_screen(void)
{
	// should have: statistics (e.g. enemies defeated, waves cleared, bonus(es) etc.) 
	//				next level button | main menu button | quit button
	//				score display (high score if time permits)




}

void render_lose_screen(void)
{
	// should have: statistics
	//				restart button | main menu button | quit button
	//				score display (high score if time permits)






}

// Terminates game.
void exit_game(void)
{
	CP_Engine_Terminate();
}

#pragma endregion

#pragma region Building / Wave Phase System

void wave_system_enemy_check(void)
{
	// check for no. of enemies left

	// If no enemies left on screen, return to building phase
}

void fast_forward_building_phase(void)
{
	// set building phase time to 0.0f
	buildingTime = 0.0f;
}

#pragma endregion

#pragma region Win / Lose Conditions

void GameWinLoseCheck(void)
{
	// checks 
}


#pragma endregion

void phantom_quartz_change(int changeInQuartz)
{
	Level[0].cash2 += changeInQuartz;
}

void gold_quartz_add(int changeInQuartz)
{
	Level[0].cash1 += changeInQuartz;
}

void phantom_gold_quartz_conversion(int phantomAmtToConvert, int conversionRate)
{
	Level[0].cash2 -= phantomAmtToConvert;
	Level[0].cash1 += phantomAmtToConvert / conversionRate;
}
