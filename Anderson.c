#include "cprocessing.h"
#include "game.h"
#include "Anderson.h"
#include "ZhengWei.h"
#include "John.h"
#include "Samuel.h"

CP_Font pixelFont;

buildingTime = BUILDING_PHASE_TIME;

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
	button.movementTime = 0.0f;
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
	PlayButton = init_text_button(PlayButton, CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Play");
	CreditsButton = init_text_button(CreditsButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Credits");
	QuitButton = init_text_button(QuitButton, CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Quit");
}

void init_level_select_buttons(void)
{
	int c = 0; char levelNumberText[8];
	for (int i = 0; i < 5; i++)
	{
		c = snprintf(levelNumberText, 8, "Level %d", i);
		strcpy_s(LevelButtons[i].textString, sizeof(LevelButtons[i].textString), levelNumberText);
		if (i > 0)
		{
			LevelButtons[i] = init_text_button(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f), BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, levelNumberText);
		}
		else
		{
			LevelButtons[i] = init_text_button(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f), BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Test");
		}
	}
	LevelSelectBackButton = init_text_button(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 1.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
}

void init_pause_screen(void)
{
	PauseScreenButtons[0] = init_text_button(PauseScreenButtons[0], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.3f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
	PauseScreenButtons[1] = init_text_button(PauseScreenButtons[1], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.4f - BUTTON_HEIGHT * 0.5f,
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

// Note: endPos is (initialPosX, initialPosY + CP_Window_Width());
void init_credit_line(int num, char* line, float x, float y)
{
	CreditTexts[num].text = line;
	CreditTexts[num].mainMenuPos.xOrigin = x;
	CreditTexts[num].mainMenuPos.yOrigin = y + CP_System_GetWindowHeight();

	CreditTexts[num].creditPos.xOrigin = x;
	CreditTexts[num].creditPos.yOrigin = y;

	CreditTexts[num].currentPos.xOrigin = CreditTexts[num].mainMenuPos.xOrigin;
	CreditTexts[num].currentPos.yOrigin = CreditTexts[num].mainMenuPos.yOrigin;
}

void init_credits_screen(void)
{
	creditRectCoords.xOrigin = CP_System_GetWindowWidth() * 0.1f; creditRectCoords.yOrigin = CP_System_GetWindowHeight() * 1.35f;
	creditRectCoords.width = CP_System_GetWindowWidth() * 0.8f; creditRectCoords.height = CP_System_GetWindowHeight() * 0.5f;

	// the © copyright symbol is printed as \xc2\xa9, as its UTF-8 (i.e Unicode) string literal counterpart.
	init_credit_line(CopyrightLine, "All content \xc2\xa9 2021 DigiPen Institute of Technology Singapore, all rights reserved.",
		CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.45f);

	init_credit_line(CreditsTitle, "Credits", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.4f);

	init_credit_line(DevelopedBy, "Developed by:", CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.5f);
	init_credit_line(ZhengWei, "Ng Zheng Wei", CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.55f);
	init_credit_line(Samuel, "Wong Zhi Hao Samuel", CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.6f);
	init_credit_line(John, "Lim Jing Rui John", CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.65f);
	init_credit_line(Gabriel, "Chiok Wei Wen Gabriel", CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.7f);
	init_credit_line(Anderson, "Phua Tai Dah Anderson", CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.75f);

	init_credit_line(President, "President: ", CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.5f);
	init_credit_line(ClaudeComair, "Claude Comair", CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.55f);

	init_credit_line(Instructors, "Instructors: ", CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.6f);
	init_credit_line(DX, "Cheng Ding Xiang", CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.65f);
	init_credit_line(Gerald, "Gerald Wong", CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.7f);

	CreditsBackButton = init_text_button(CreditsBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 1.9f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
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
	render_ui_button(CreditsButton);
	render_ui_button(QuitButton);
}

void render_level_select_buttons(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	// render the 5 level buttons
	for (int i = 0; i < 5; i++)
	{
		render_ui_button(LevelButtons[i]);
	}
	render_ui_button(LevelSelectBackButton);
}

void render_credit_line(CreditLine cLine)
{
	CP_Font_DrawText(cLine.text, cLine.currentPos.xOrigin, cLine.currentPos.yOrigin);
}

void render_credits_screen(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	CP_Settings_Fill(COLOR_BLACK);

	CP_Graphics_DrawRect(creditRectCoords.xOrigin, creditRectCoords.yOrigin, creditRectCoords.width, creditRectCoords.height);

	CP_Settings_Fill(COLOR_WHITE);
	render_credit_line(CreditTexts[CreditsTitle]);
	CP_Settings_TextSize(FONT_SIZE * 0.5);
	render_credit_line(CreditTexts[CopyrightLine]);

	CP_Settings_TextSize(FONT_SIZE);

	render_credit_line(CreditTexts[DevelopedBy]);
	CP_Settings_Fill(COLOR_YELLOW);
	render_credit_line(CreditTexts[ZhengWei]);
	render_credit_line(CreditTexts[Samuel]);
	render_credit_line(CreditTexts[John]);
	render_credit_line(CreditTexts[Gabriel]);
	render_credit_line(CreditTexts[Anderson]);

	CP_Settings_Fill(COLOR_WHITE);
	render_credit_line(CreditTexts[Instructors]);
	render_credit_line(CreditTexts[President]);
	CP_Settings_Fill(COLOR_YELLOW);
	render_credit_line(CreditTexts[DX]);
	render_credit_line(CreditTexts[Gerald]);
	render_credit_line(CreditTexts[ClaudeComair]);

	render_ui_button(CreditsBackButton);
}

void render_pause_screen(void)
{
	CP_Settings_Fill(COLOR_GREY);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.25f, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.2f);
	render_ui_button(PauseScreenButtons[0]);
	render_ui_button(PauseScreenButtons[1]);
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
	char buffer[16] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "Time Left: %.1f", buildingTime);
	CP_Font_DrawText(buffer, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.05f);
}

void reduce_building_phase_time()
{
	if (buildingTime < 0.05f)
	{
		set_building_time(0.0f);
		currentGameState = Wave;
		if (Level[currentGameLevel].currentWave == 0) {
			Level[currentGameLevel].currentEffect = 0;
		}
		else if (Level[currentGameLevel].currentWave < 5) {
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
		// free memory
		for (int i = 0; i < gameGridRows; i++) {
			free(Level[currentGameLevel].grid[i]);
		}
		free(Level[currentGameLevel].grid);
		//Free memory for turret_on_grid
		for (int i = 0; i < gameGridCols; i++) {
			free(turret_on_grid[i]);
		}
		free(turret_on_grid);

		// game lost
		currentGameState = Lose;

	}
	else if (enemiesLeft == 0)
	{
		if (Level[currentGameLevel].currentWave == MAX_NUMBER_OF_WAVES-1)
		{
			// free memory
			for (int i = 0; i < gameGridRows; i++) {
				free(Level[currentGameLevel].grid[i]);
			}
			free(Level[currentGameLevel].grid);
			//Free memory for turret_on_grid
			for (int i = 0; i < gameGridCols; i++) {
				free(turret_on_grid[i]);
			}
			free(turret_on_grid);
			currentGameState = Win;
		}
		else // if still in the midst of the current level
		{
			set_building_time(BUILDING_PHASE_TIME);
			currentGameState = Building;
			Level[currentGameLevel].currentWave += 1;
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

#pragma region UI Movement

float linear(float start, float end, float value)
{
	// taken from easing.h, credit goes to prof gerald
	return (1.f - value) * start + value * end;
}

Button ui_button_movement(Button button, float destPosX, float destPosY)
{
	if (button.movementTime <= MOVE_DURATION)
	{
		button.movementTime += CP_System_GetDt();
		button.buttonData.xOrigin = linear(button.buttonData.xOrigin, destPosX, button.movementTime / MOVE_DURATION);
		button.buttonData.yOrigin = linear(button.buttonData.yOrigin, destPosY, button.movementTime / MOVE_DURATION);
		button.textPositionX = linear(button.textPositionX, destPosX + BUTTON_WIDTH * 0.5f, button.movementTime / MOVE_DURATION);
		button.textPositionY = linear(button.textPositionY, destPosY + BUTTON_HEIGHT * 0.5f, button.movementTime / MOVE_DURATION);
	}
	else
	{
		// Most buttons take ~3-4s to finish moving.
		// May be good to try basing this on something else instead like distance.
		button.movementTime = 0.0f;
	}
	return button;
}

Coordinates coord_movement(Coordinates coord, float destPosX, float destPosY)
{
	if (creditTextMoveTime <= MOVE_DURATION)
	{
		creditTextMoveTime += CP_System_GetDt();
		coord.xOrigin = linear(coord.xOrigin, destPosX, creditTextMoveTime / MOVE_DURATION);
		coord.yOrigin = linear(coord.yOrigin, destPosY, creditTextMoveTime / MOVE_DURATION);
	}
	else
	{
		// May be good to try basing this on something else instead like distance.
		creditTextMoveTime = 0.0f;
	}
	return coord;
}

// A note: moving stops when the credit screen's back button has finished moving.
void move_credits_screen(void)
{
	for (int i = 0; i < sizeof(CreditTexts) / sizeof(CreditLine); i++)
	{
		if (currentGameState == MainMenu)
		{
			CreditTexts[i].currentPos = coord_movement(CreditTexts[i].currentPos, CreditTexts[i].creditPos.xOrigin,
				CreditTexts[i].creditPos.yOrigin); // from l_select back to main menu
		}
		else if (currentGameState == Credits) // going to main menu
		{
			CreditTexts[i].currentPos = coord_movement(CreditTexts[i].currentPos, CreditTexts[i].mainMenuPos.xOrigin,
				CreditTexts[i].mainMenuPos.yOrigin); // from l_select back to main menu
		}
	}

	if (currentGameState == MainMenu)
	{
		creditRectCoords = coord_movement(creditRectCoords, creditRectCoords.xOrigin, CP_System_GetWindowHeight() * 0.35f);
		CreditsBackButton = ui_button_movement(CreditsBackButton, CreditsBackButton.buttonData.xOrigin, CP_System_GetWindowHeight() * 0.9f);
	}
	else if (currentGameState == Credits)
	{
		creditRectCoords = coord_movement(creditRectCoords, creditRectCoords.xOrigin, CP_System_GetWindowHeight() * 1.35f);
		CreditsBackButton = ui_button_movement(CreditsBackButton, CreditsBackButton.buttonData.xOrigin, CP_System_GetWindowHeight() * 2.0f);
	}
}

void move_level_select(void)
{
	for (int i = 0; i < MAX_NUMBER_OF_LEVEL; i++)
	{
		if (currentGameState == LevelSelect) // going to main menu
		{
			LevelButtons[i] = ui_button_movement(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f)); // from l_select back to main menu
		}
		else if (currentGameState == MainMenu)
		{
			LevelButtons[i] = ui_button_movement(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() * 0.35f + i * (BUTTON_HEIGHT + 25.0f)); // from l_select back to main menu
		}
	}
	if (currentGameState == LevelSelect) // going to main menu
	{
		LevelSelectBackButton = ui_button_movement(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
			CP_System_GetWindowHeight() * 1.50f); // from l_select back to main menu
	}
	else if (currentGameState == MainMenu)
	{
		LevelSelectBackButton = ui_button_movement(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
			CP_System_GetWindowHeight() * 0.8f); // from l_select back to main menu
	}
}

int level_select_finished_moving(void)
{
	if (currentGameState == LevelSelect)
	{
		if (button_has_finished_moving(*LevelButtons, LevelButtons->buttonData.xOrigin, CP_System_GetWindowHeight()))
		{
			for (Button* b = LevelButtons; b < LevelButtons + MAX_NUMBER_OF_LEVEL; b++)
			{
				b->movementTime = 0.0f;
			}
			LevelSelectBackButton.movementTime = 0.0f;
			return 1;
		}
	}
	else if (currentGameState == MainMenu)
	{
		if (button_has_finished_moving(*LevelButtons, LevelButtons->buttonData.xOrigin, CP_System_GetWindowHeight() * 0.35f))
		{
			for (Button* b = LevelButtons; b < LevelButtons + MAX_NUMBER_OF_LEVEL; b++)
			{
				b->movementTime = 0.0f;
			}
			LevelSelectBackButton.movementTime = 0.0f;
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

#pragma endregion
