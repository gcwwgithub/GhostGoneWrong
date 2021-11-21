#include "cprocessing.h"
#include "game.h"
#include "Anderson.h"
#include "ZhengWei.h"
#include "John.h"
#include "Samuel.h"

CP_Font pixelFont;

buildingTime = BUILDING_PHASE_TIME;
score = 0;

#pragma region UI

#pragma region Initialisations

void init_game_font(void)
{
	pixelFont = GAME_FONT;
	CP_Font_Set(pixelFont);
	CP_Settings_TextSize(FONT_SIZE);
}

// Assuming all buttons are rectangles
Button init_text_button(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[])
{
	button.buttonData.xOrigin = buttonPosX;
	button.buttonData.yOrigin = buttonPosY;
	button.buttonData.width = buttonWidth;
	button.buttonData.height = buttonHeight;
	button.buttonData.objectType = objectRectangle;
	button.interpolationTime = 0.0f;
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	button.textPositionX = button.buttonData.xOrigin + textPosX;
	button.textPositionY = button.buttonData.yOrigin + textPosY;
	strcpy_s(button.textString, sizeof(button.textString), string);

	return button;
}

void init_how_to_play_button(void)
{
	// middle of title screen(?)
	HowToPlayButton.buttonData.xOrigin = CP_System_GetWindowWidth() * 0.5f;
	HowToPlayButton.buttonData.yOrigin = CP_System_GetWindowHeight() * 0.5f;
	HowToPlayButton.buttonData.width = BUTTON_WIDTH * 2;
	HowToPlayButton.buttonData.height = BUTTON_HEIGHT;
	HowToPlayButton.buttonData.objectType = objectRectangle;
	HowToPlayButton.textPositionX = HowToPlayButton.buttonData.xOrigin + BUTTON_WIDTH * 0.5f;
	HowToPlayButton.textPositionY = HowToPlayButton.buttonData.yOrigin + BUTTON_HEIGHT * 0.5f;
	strcpy_s(HowToPlayButton.textString, sizeof(HowToPlayButton.textString), "How To Play");
}

void init_how_to_play_screen(void)
{
	// display turret sprites, descriptions

	// display ghost sprites, descriptions

	// display win / lose conditions && rules

}

void init_main_menu(void)
{
	PlayButton = init_text_button(PlayButton, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Play");
	QuitButton = init_text_button(QuitButton, CP_System_GetWindowWidth() * 0.65f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Quit");
}

// number of levels is hardcoded.
// LSelect buttons seperated by pure vertical gap that is 25.0f units long.
void init_level_select_buttons(void)
{
	int c = 0; char levelNumberText[8];
	for (int i = 0; i < 5; i++)
	{
		c = snprintf(levelNumberText, 8, "Level %d", i + 1);
		strcpy_s(levelButtons[i].textString, sizeof(levelButtons[i].textString), levelNumberText);
		levelButtons[i] = init_text_button(levelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
			CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f), BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, levelNumberText);
	}
	BackButton = init_text_button(BackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 1.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
}

void init_pause_screen(void)
{
	PauseBackButton = init_text_button(PauseBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.3f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
	PauseQuitButton = init_text_button(PauseQuitButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.4f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Quit");
}

void init_end_screen(void)
{
	// Back to Main Menu
	EndScreenButtons[0].buttonData.xOrigin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f;
	EndScreenButtons[0].buttonData.yOrigin = CP_System_GetWindowHeight() * 0.5f - BUTTON_HEIGHT * 0.5f;
	EndScreenButtons[0].buttonData.width = BUTTON_WIDTH;
	EndScreenButtons[0].buttonData.height = BUTTON_HEIGHT;
	EndScreenButtons[0].buttonData.objectType = objectRectangle;
	EndScreenButtons[0].textPositionX = EndScreenButtons[0].buttonData.xOrigin + BUTTON_WIDTH * 0.5f;
	EndScreenButtons[0].textPositionY = EndScreenButtons[0].buttonData.yOrigin + BUTTON_HEIGHT * 0.5f;
	strcpy_s(EndScreenButtons[0].textString, sizeof(EndScreenButtons[0].textString), "Back");

	// Restart
	EndScreenButtons[1].buttonData.xOrigin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f;
	EndScreenButtons[1].buttonData.yOrigin = CP_System_GetWindowHeight() * 0.6f - BUTTON_HEIGHT * 0.5f;
	EndScreenButtons[1].buttonData.width = BUTTON_WIDTH;
	EndScreenButtons[1].buttonData.height = BUTTON_HEIGHT;
	EndScreenButtons[1].buttonData.objectType = objectRectangle;
	EndScreenButtons[1].textPositionX = EndScreenButtons[1].buttonData.xOrigin + BUTTON_WIDTH * 0.5f;
	EndScreenButtons[1].textPositionY = EndScreenButtons[1].buttonData.yOrigin + BUTTON_HEIGHT * 0.5f;
	strcpy_s(EndScreenButtons[1].textString, sizeof(EndScreenButtons[1].textString), "Restart");

	// Next Level
	EndScreenButtons[2].buttonData.xOrigin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f;
	EndScreenButtons[2].buttonData.yOrigin = CP_System_GetWindowHeight() * 0.7f - BUTTON_HEIGHT * 0.5f;
	EndScreenButtons[2].buttonData.width = BUTTON_WIDTH;
	EndScreenButtons[2].buttonData.height = BUTTON_HEIGHT;
	EndScreenButtons[2].buttonData.objectType = objectRectangle;
	EndScreenButtons[2].textPositionX = EndScreenButtons[2].buttonData.xOrigin + BUTTON_WIDTH * 0.5f;
	EndScreenButtons[2].textPositionY = EndScreenButtons[2].buttonData.yOrigin + BUTTON_HEIGHT * 0.5f;
	strcpy_s(EndScreenButtons[2].textString, sizeof(EndScreenButtons[2].textString), "Next");
}

#pragma endregion

#pragma region Rendering

int cursor_over_button(Coordinates buttonCoord)
{
	return ((CP_Input_GetMouseX() > buttonCoord.xOrigin) && (CP_Input_GetMouseX() < buttonCoord.xOrigin + buttonCoord.width)
		&& (CP_Input_GetMouseY() > buttonCoord.yOrigin) && (CP_Input_GetMouseY() < buttonCoord.yOrigin + buttonCoord.height));
}

// Draws a background image, background is drawn with code, rest with pixel art
void render_title_screen(void)
{
	RenderWithAlphaChanged(backgroundSpriteSheet, backgroundArray[0], CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f, CP_System_GetWindowWidth(), CP_System_GetWindowHeight(), 150);
	CP_Image_Draw(titleWordImage, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.2f, 256 * scalingFactor, 256 * scalingFactor, 255);
}

void render_ui_button(Button button)
{
	CP_Settings_Fill(COLOR_BLACK);

	double mouseOverSizeModifier = cursor_over_button(button.buttonData) ? 1.1 : 1;

	// Adjusting position to account for enlarged button when moused over.
	CP_Graphics_DrawRect(button.buttonData.xOrigin - (mouseOverSizeModifier - 1) * button.buttonData.width / 2,
		button.buttonData.yOrigin - (mouseOverSizeModifier - 1) * button.buttonData.height / 2,
		button.buttonData.width * mouseOverSizeModifier, button.buttonData.height * mouseOverSizeModifier);

	(mouseOverSizeModifier - 1) ? CP_Settings_Fill(COLOR_GREY) : CP_Settings_Fill(COLOR_WHITE);

	CP_Settings_TextSize(FONT_SIZE);
	CP_Font_DrawText(button.textString, button.textPositionX, button.textPositionY);
}

void render_how_to_play_screen(void)
{

}

void render_start_menu(void)
{
	render_ui_button(PlayButton);
	render_ui_button(QuitButton);
}

void render_level_select_buttons(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	// render the 5 level buttons
	for (int i = 0; i < 5; i++)
	{
		render_ui_button(levelButtons[i]);
	}
	render_ui_button(BackButton);
}

void render_credits_screen(void)
{
	// just render text and other things here
}

void render_pause_screen(void)
{
	CP_Settings_Fill(COLOR_GREY);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.25f, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.2f);
	render_ui_button(PauseBackButton);
	render_ui_button(PauseQuitButton);
}

#pragma endregion

// Terminates game.
void exit_to_desktop(void)
{
	CP_Engine_Terminate();
}

#pragma endregion

#pragma region Building / Wave Phase System

void init_skip_wave_button(void)
{
	// Init skip wave button's position.
	SkipWaveButton.buttonData.xOrigin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f;
	SkipWaveButton.buttonData.yOrigin = CP_System_GetWindowHeight() * 0.1f;
	SkipWaveButton.buttonData.width = BUTTON_WIDTH;
	SkipWaveButton.buttonData.height = BUTTON_HEIGHT;

	SkipWaveButton.textPositionX = SkipWaveButton.buttonData.xOrigin + BUTTON_WIDTH * 0.5f;
	SkipWaveButton.textPositionY = SkipWaveButton.buttonData.yOrigin + BUTTON_HEIGHT * 0.5f;
	strcpy_s(SkipWaveButton.textString, sizeof(SkipWaveButton.textString), "Skip");
}

void render_wave_timer_bar(float timeLeft, float maxTime, float barWidth, float barHeight)
{
	double percentage = timeLeft / maxTime;
	// draw red, then green
	CP_Settings_Fill(COLOR_RED);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth, barHeight);
	CP_Settings_Fill(COLOR_GREEN);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth * percentage, barHeight);
}

void render_wave_timer(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	render_wave_timer_bar(buildingTime, BUILDING_PHASE_TIME, CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.1f);
	CP_Settings_Fill(COLOR_BLACK);
	char buffer[25] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "Time Left: %.1f", buildingTime);
	CP_Font_DrawText(buffer, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.05f);
}

void reduce_building_phase_time()
{
	if (buildingTime < 0.05f)
	{
		set_building_time(0.0f);
		Level[currentGameLevel].currentWave += 1;
		currentGameState = Wave;
		if (Level[currentGameLevel].currentWave < 5) {
			Level[currentGameLevel].currentEffect = CP_Random_RangeInt(0, 10);
		}
		else {
			Level[currentGameLevel].currentEffect = CP_Random_RangeInt(0, 11);
		}
	}
	else
	{
		set_building_time(buildingTime - CP_System_GetDt());
	}
}

void set_building_time(float newBuildingTime)
{
	// set building phase time to 0.0f
	buildingTime = newBuildingTime;
}

void add_score_bonus(int bonus)
{
	score += bonus;
}

#pragma endregion

#pragma region Win / Lose Conditions

void render_end_screen(void)
{
	// should have: statistics
	//				score display (high score if time permits)
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(COLOR_GREY);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.5f);

	CP_Settings_Fill(COLOR_BLACK);
	if (currentGameState == Lose)
	{
		CP_Font_DrawText("Game Lost!", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.4f);
	}
	else if (currentGameState == Win)
	{
		CP_Font_DrawText("Game Won!", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.4f);
	}
	CP_Settings_RectMode(CP_POSITION_CORNER);

	render_ui_button(EndScreenButtons[0]);
	render_ui_button(EndScreenButtons[1]);

	if (currentGameLevel < 4)
	{
		render_ui_button(EndScreenButtons[2]);
	}
}

void game_win_lose_check(void)
{
	// checks portal health && number of enemies left.
	if (Level[currentGameLevel].health <= 0)
	{
		// game lost
		currentGameState = Lose;
	}
	else if (0 == enemiesLeft)
	{
		if (Level[currentGameLevel].currentWave == MAX_NUMBER_OF_WAVES)
		{
			currentGameState = Win;
		}
		else // if still in the midst of the current level
		{
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
		}
	}
}

#pragma endregion

//void quartz_change(int quartzTypeQuantity, int changeInQuartz)
//{
//	quartzTypeQuantity += changeInQuartz;
//}
//
//void gold_to_phantom_quartz_conversion(int goldAmtToConvert, int conversionRate)
//{
//	Level[currentGameLevel].goldQuartz -= goldAmtToConvert;
//	Level[currentGameLevel].phantomQuartz += goldAmtToConvert / conversionRate;
//}

float linear(float start, float end, float value)
{
	// taken from easing.h, credit goes to prof gerald
	return (1.f - value) * start + value * end;
}

Button ui_button_movement(Button button, float destPosX, float destPosY)
{
	if (button.interpolationTime <= MOVE_DURATION)
	{
		button.interpolationTime += CP_System_GetDt();
		button.buttonData.xOrigin = linear(button.buttonData.xOrigin, destPosX, button.interpolationTime / MOVE_DURATION);
		button.buttonData.yOrigin = linear(button.buttonData.yOrigin, destPosY, button.interpolationTime / MOVE_DURATION);
		button.textPositionX = linear(button.textPositionX, destPosX + BUTTON_WIDTH * 0.5f, button.interpolationTime / MOVE_DURATION);
		button.textPositionY = linear(button.textPositionY, destPosY + BUTTON_HEIGHT * 0.5f, button.interpolationTime / MOVE_DURATION);
	}
	else
	{
		// Most buttons take ~3-4s to finish moving.
		// May be good to try basing this on something else instead like distance.
		button.interpolationTime = 0.0f;
	}
	return button;
}

void move_level_select(void)
{
	for (int i = 0; i < MAX_NUMBER_OF_LEVEL; i++)
	{
		if (currentGameState == LevelSelect) // going to main menu
		{
			levelButtons[i] = ui_button_movement(levelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f)); // from l_select back to main menu
		}
		else if (currentGameState == MainMenu)
		{
			levelButtons[i] = ui_button_movement(levelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() * 0.35f + i * (BUTTON_HEIGHT + 25.0f)); // from l_select back to main menu
		}
	}
	if (currentGameState == LevelSelect) // going to main menu
	{
		BackButton = ui_button_movement(BackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
			CP_System_GetWindowHeight() * 1.50f); // from l_select back to main menu
	}
	else if (currentGameState == MainMenu)
	{
		BackButton = ui_button_movement(BackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
			CP_System_GetWindowHeight() * 0.8f); // from l_select back to main menu
	}
}

int level_select_finished_moving(void)
{
	if (currentGameState == LevelSelect)
	{
		if (button_has_finished_moving(*levelButtons, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight()))
		{
			for (Button* b = levelButtons; b < levelButtons + MAX_NUMBER_OF_LEVEL; b++)
			{
				b->interpolationTime = 0.0f;
			}
			BackButton.interpolationTime = 0.0f;
			return 1;
		}
	}
	else if (currentGameState == MainMenu)
	{
		if (button_has_finished_moving(*levelButtons, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.35f))
		{
			for (Button* b = levelButtons; b < levelButtons + MAX_NUMBER_OF_LEVEL; b++)
			{
				b->interpolationTime = 0.0f;
			}
			BackButton.interpolationTime = 0.0f;
			return 1;
		}
	}
	return 0;
}

int button_has_finished_moving(Button button, float destPosX, float destPosY)
{
	return (button.buttonData.xOrigin == destPosX && button.buttonData.yOrigin == destPosY);
}

void init_next_level(int nextGameLevel)
{
	switch (nextGameLevel)
	{
	case 0:
	{
		level1_init();
		break;
	}
	case 1:
	{
		level2_init();
		break;
	}
	case 2:
	{
		level3_init();
		break;
	}
	case 3:
	{
		level4_init();
		break;
	}
	case 4:
	{
		level5_init();
		break;
	}

	game_grid_init();
	isPlacingTurret = T_MAX;
	turretSelectedToUpgrade = NO_TURRET_SELECTED;
	powerUpMenu = FALSE;
	pathfinding_init(&Level[nextGameLevel]);
	environment_init(&Level[nextGameLevel]);

	//turret menu items
	pause_button_init();
	turret_basic_button_init();
	turret_slow_button_init();
	turret_homing_button_init();
	turret_mine_button_init();
	phantomQuartz_init();
	goldQuartz_init();

	turret_init();
	Enemies_init();

	pathfinding_reset(&Level[nextGameLevel]);
	pathfinding_calculate_cost(&Level[nextGameLevel]);
	pathfinding_update(&Level[nextGameLevel]);
	set_building_time(BUILDING_PHASE_TIME);
	currentGameState = Building;
	}
}
