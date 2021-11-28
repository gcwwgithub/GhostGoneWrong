#include "cprocessing.h"
#include "game.h"
#include "Anderson.h"

#pragma region UI

#pragma region Initialisations

void init_game_font(void)
{
	pixelFont = GAME_FONT;
	CP_Font_Set(pixelFont);
	CP_Settings_TextSize(FONT_SIZE);
}

void init_splash_logos(void)
{
	DigipenLogo = CP_Image_Load("./Assets/DigipenLogo.png");
	DownNOutLogo = CP_Image_Load("./Assets/DownNOut.png");
}

// Assuming all buttons are rectangles
Button init_text_button(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[])
{
	button.buttonData.x_origin = buttonPosX;
	button.buttonData.y_origin = buttonPosY;
	button.buttonData.width = buttonWidth;
	button.buttonData.height = buttonHeight;
	button.movementTime = 0.0f;
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	button.textPositionX = button.buttonData.x_origin + textPosX;
	button.textPositionY = button.buttonData.y_origin + textPosY;
	strcpy_s(button.textString, sizeof(button.textString), string);

	return button;
}

void init_how_to_play_button(void)
{
	// implemented later ?
	//HowToPlayButton = init_text_button(HowToPlayButton,CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f,
	//	BUTTON_WIDTH * 2, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "How To Play");
}

void init_main_menu(void)
{
	PlayButton = init_text_button(PlayButton, CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Start Game");
	CreditsButton = init_text_button(CreditsButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Credits");
	QuitButton = init_text_button(QuitButton, CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Quit Game");
	HowToPlayButton = init_text_button(HowToPlayButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.75f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "How To Play");
}

void init_level_select_buttons(void)
{
	int c = 0; char levelNumberText[8];
	for (int i = 0; i < kMaxNumberOfLevel; i++)
	{
		c = snprintf(levelNumberText, 8, "Level %d", i);
		strcpy_s(LevelButtons[i].textString, sizeof(LevelButtons[i].textString), levelNumberText);
		if (i > 0)
		{
			LevelButtons[i] = init_text_button(LevelButtons[i],
				CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f),
				BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, levelNumberText);
		}
		else
		{
			LevelButtons[i] = init_text_button(LevelButtons[i],
				CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f),
				BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Tutorial");
		}
	}


	LevelSelectBackButton = init_text_button(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 1.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
}

void init_pause_screen(void)
{
	// Resume
	PauseScreenButtons[0] = init_text_button(PauseScreenButtons[0],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.2f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Resume Game");
	// Exit Level
	PauseScreenButtons[1] = init_text_button(PauseScreenButtons[1],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.3f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Exit Level");
}

void init_end_screen(void)
{
	// Back to Main Menu
	//EndScreenButtons[0].buttonData.x_origin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f;
	//EndScreenButtons[0].buttonData.y_origin = CP_System_GetWindowHeight() * 0.5f - BUTTON_HEIGHT * 0.5f;
	//EndScreenButtons[0].buttonData.width = BUTTON_WIDTH;
	//EndScreenButtons[0].buttonData.height = BUTTON_HEIGHT;
	//EndScreenButtons[0].buttonData.object_type = kObjectRectangle;
	//EndScreenButtons[0].textPositionX = EndScreenButtons[0].buttonData.x_origin + BUTTON_WIDTH * 0.5f;
	//EndScreenButtons[0].textPositionY = EndScreenButtons[0].buttonData.y_origin + BUTTON_HEIGHT * 0.5f;
	//strcpy_s(EndScreenButtons[0].textString, sizeof(EndScreenButtons[0].textString), "Back");
	//
	// Restart
	//EndScreenButtons[1].buttonData.x_origin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f;
	//EndScreenButtons[1].buttonData.y_origin = CP_System_GetWindowHeight() * 0.6f - BUTTON_HEIGHT * 0.5f;
	//EndScreenButtons[1].buttonData.width = BUTTON_WIDTH;
	//EndScreenButtons[1].buttonData.height = BUTTON_HEIGHT;
	//EndScreenButtons[1].buttonData.object_type = kObjectRectangle;
	//EndScreenButtons[1].textPositionX = EndScreenButtons[1].buttonData.x_origin + BUTTON_WIDTH * 0.5f;
	//EndScreenButtons[1].textPositionY = EndScreenButtons[1].buttonData.y_origin + BUTTON_HEIGHT * 0.5f;
	//strcpy_s(EndScreenButtons[1].textString, sizeof(EndScreenButtons[1].textString), "Restart");
	//
	// Next Level
	//EndScreenButtons[2].buttonData.x_origin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f;
	//EndScreenButtons[2].buttonData.y_origin = CP_System_GetWindowHeight() * 0.7f - BUTTON_HEIGHT * 0.5f;
	//EndScreenButtons[2].buttonData.width = BUTTON_WIDTH;
	//EndScreenButtons[2].buttonData.height = BUTTON_HEIGHT;
	//EndScreenButtons[2].buttonData.object_type = kObjectRectangle;
	//EndScreenButtons[2].textPositionX = EndScreenButtons[2].buttonData.x_origin + BUTTON_WIDTH * 0.5f;
	//EndScreenButtons[2].textPositionY = EndScreenButtons[2].buttonData.y_origin + BUTTON_HEIGHT * 0.5f;
	//strcpy_s(EndScreenButtons[2].textString, sizeof(EndScreenButtons[2].textString), "Next");

	EndScreenButtons[0] = init_text_button(EndScreenButtons[0],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");

	EndScreenButtons[1] = init_text_button(EndScreenButtons[1],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.6f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Restart");

	EndScreenButtons[2] = init_text_button(EndScreenButtons[2],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.7f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Next");
}

// Note: endPos is (initialPosX, initialPosY + CP_Window_Width());
void init_credit_line(int num, char* line, float x, float y)
{
	CreditTexts[num].text = line;
	CreditTexts[num].mainMenuPos.x_origin = x;
	CreditTexts[num].mainMenuPos.y_origin = y + CP_System_GetWindowHeight();

	CreditTexts[num].creditPos.x_origin = x;
	CreditTexts[num].creditPos.y_origin = y;

	CreditTexts[num].currentPos.x_origin = CreditTexts[num].mainMenuPos.x_origin;
	CreditTexts[num].currentPos.y_origin = CreditTexts[num].mainMenuPos.y_origin;
}

void init_credits_screen(void)
{
	creditRectCoords.x_origin = CP_System_GetWindowWidth() * 0.1f; creditRectCoords.y_origin = CP_System_GetWindowHeight() * 1.3f;
	creditRectCoords.width = CP_System_GetWindowWidth() * 0.8f; creditRectCoords.height = CP_System_GetWindowHeight() * 0.6f;

	init_credit_line(CreditsTitle, "Credits", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.35f);
	init_credit_line(TeamCredit, "A DownNOut X Digipen Production", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.4f);

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

	init_credit_line(CreatedAtDigipen, "Created at DigiPen Institute of Technology Singapore", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.82f);
	init_credit_line(DigipenURL, "www.digipen.edu", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.85f);
	// the � copyright symbol is printed as \xc2\xa9, as its UTF-8 (i.e Unicode) string literal counterpart.
	init_credit_line(CopyrightLine, "All content \xc2\xa9 2021 DigiPen Institute of Technology Singapore, all rights reserved.",
		CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.88f);

	CreditsBackButton = init_text_button(CreditsBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 2.0f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
}

void init_options_screen(void)
{
	// init options screen for main menu

}

#pragma endregion

#pragma region Rendering

int cursor_over_button(Coordinates buttonCoord)
{
	return ((CP_Input_GetMouseX() > buttonCoord.x_origin) && (CP_Input_GetMouseX() < buttonCoord.x_origin + buttonCoord.width)
		&& (CP_Input_GetMouseY() > buttonCoord.y_origin) && (CP_Input_GetMouseY() < buttonCoord.y_origin + buttonCoord.height));
}

// Draws a background image, background is drawn with code, rest with pixel art
void render_title_screen(void)
{
	RenderImageFromSpriteSheetWithAlpha(background_spritesheet, background_spritesheet_array[0], CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 150);
	CP_Image_Draw(game_title_image, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.2f, 256 * scaling_factor, 256 * scaling_factor, 255);
}

void render_logos(void)
{
	CP_Graphics_ClearBackground(COLOR_BLACK);
	CP_Settings_TextSize(FONT_SIZE * 0.5f); // for copyright text to fit
	if (dpLogoDisplayTime < 0.0f) // dp logo finish display
	{
		if (dpLogoFadeTime > 0.0f) // fading
		{
			dpLogoFadeTime -= CP_System_GetDt();
			CP_Image_Draw(DigipenLogo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(DigipenLogo) * 0.5f, (float)CP_Image_GetHeight(DigipenLogo) * 0.5f, (int)(255 * (dpLogoFadeTime / FADE_OUT_TIME)));
			CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
		}
		else // dp logo finished fading
		{
			if (teamLogoDisplayTime > 0.0f)
			{
				CP_Image_Draw(DownNOutLogo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(DownNOutLogo), (float)CP_Image_GetHeight(DownNOutLogo), 255);
				teamLogoDisplayTime -= CP_System_GetDt();
				CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);

			}
			else // team logo finished display
			{
				CP_Image_Draw(DownNOutLogo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(DownNOutLogo), (float)CP_Image_GetHeight(DownNOutLogo), (int)(255 * (teamLogoFadeTime / FADE_OUT_TIME)));
				teamLogoFadeTime -= CP_System_GetDt();
				CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);


				if (teamLogoFadeTime < 0.0f) // team logo finished fading
				{
					CP_Settings_TextSize(FONT_SIZE); // set font size back to normal.
					current_game_state = kMainMenu;
				}
			}
		}
	}
	else if (dpLogoDisplayTime >= 0.0f) // dp Logo still displaying
	{
		dpLogoDisplayTime -= CP_System_GetDt();
		CP_Image_Draw(DigipenLogo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(DigipenLogo) / 2, (float)CP_Image_GetHeight(DigipenLogo) / 2, 255);
		CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
	}
}

void render_ui_button(Button button)
{
	CP_Settings_Fill(COLOR_BLACK);

	float mouseOverSizeModifier = cursor_over_button(button.buttonData) ? 1.1f : 1.0f;

	// Adjusting position to account for enlarged button when moused over.
	CP_Graphics_DrawRect(button.buttonData.x_origin - (mouseOverSizeModifier - 1) * button.buttonData.width / 2.0f,
		button.buttonData.y_origin - (mouseOverSizeModifier - 1) * button.buttonData.height / 2.0f,
		button.buttonData.width * mouseOverSizeModifier, button.buttonData.height * mouseOverSizeModifier);

	(mouseOverSizeModifier - 1) ? CP_Settings_Fill(COLOR_GREY) : CP_Settings_Fill(COLOR_WHITE);

	CP_Settings_TextSize(cursor_over_button(button.buttonData) ? FONT_SIZE * mouseOverSizeModifier : FONT_SIZE);
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
	render_ui_button(HowToPlayButton);
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

void render_pause_screen(void)
{
	CP_Settings_Fill(COLOR_GREY);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.15f, CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.2f);
	render_ui_button(PauseScreenButtons[0]);
	render_ui_button(PauseScreenButtons[1]);
}

void render_credit_line(CreditLine cLine)
{
	CP_Font_DrawText(cLine.text, cLine.currentPos.x_origin, cLine.currentPos.y_origin);
}

void render_credits_screen(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	CP_Settings_Fill(COLOR_BLACK);

	CP_Graphics_DrawRect(creditRectCoords.x_origin, creditRectCoords.y_origin, creditRectCoords.width, creditRectCoords.height);

	CP_Settings_Fill(COLOR_WHITE);
	render_credit_line(CreditTexts[CreditsTitle]);

	render_credit_line(CreditTexts[DevelopedBy]);
	render_credit_line(CreditTexts[TeamCredit]);
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

	CP_Settings_Fill(COLOR_WHITE);
	CP_Settings_TextSize(FONT_SIZE * 0.5f);
	render_credit_line(CreditTexts[CreatedAtDigipen]);
	render_credit_line(CreditTexts[DigipenURL]);
	render_credit_line(CreditTexts[CopyrightLine]);
	CP_Settings_TextSize(FONT_SIZE);

	render_ui_button(CreditsBackButton);
}

void render_options_screen(void)
{
	// render options
	// render sound symbols
}


void show_logos(void)
{
	CP_Graphics_ClearBackground(COLOR_BLACK);		
	CP_Settings_TextSize(FONT_SIZE * 0.5f); // for copyright text to fit
	if (dpLogoDisplayTime < 0.0f) // dp logo finish display
	{
		if (dpLogoFadeTime > 0.0f) // fading
		{
			dpLogoFadeTime -= CP_System_GetDt();
			CP_Image_Draw(DigipenLogo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(DigipenLogo) * 0.5f, (float)CP_Image_GetHeight(DigipenLogo) * 0.5f, (int)(255 * (dpLogoFadeTime / FADE_OUT_TIME)));
			CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
		}
		else // dp logo finished fading
		{
			if (teamLogoDisplayTime > 0.0f)
			{
				CP_Image_Draw(DownNOutLogo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(DownNOutLogo), (float)CP_Image_GetHeight(DownNOutLogo), 255);
				teamLogoDisplayTime -= CP_System_GetDt();
				CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
			}
			else // team logo finished display
			{
				CP_Image_Draw(DownNOutLogo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(DownNOutLogo), (float)CP_Image_GetHeight(DownNOutLogo), (int)(255 * (teamLogoFadeTime / FADE_OUT_TIME)));
				teamLogoFadeTime -= CP_System_GetDt();
				CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
				if (teamLogoFadeTime < 0.0f)
				{
					CP_Settings_TextSize(FONT_SIZE); // set font size back to normal.
					current_game_state = kMainMenu;
				}
			}
		}
	}
	else if (dpLogoDisplayTime >= 0.0f) // dp Logo still displaying
	{
		dpLogoDisplayTime -= CP_System_GetDt();
		CP_Image_Draw(DigipenLogo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(DigipenLogo) / 2, (float)CP_Image_GetHeight(DigipenLogo) / 2, 255);
		CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
	}
}

// Terminates game.
void exit_to_desktop(void)
{
	CP_Engine_Terminate();
}

#pragma endregion

#pragma endregion

#pragma region Building / Wave Phase System

void init_skip_wave_button(void)
{
	// Init skip wave button's position.
	SkipWaveButton.buttonData.x_origin = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f;
	SkipWaveButton.buttonData.y_origin = CP_System_GetWindowHeight() * 0.1f;
	SkipWaveButton.buttonData.width = BUTTON_WIDTH;
	SkipWaveButton.buttonData.height = BUTTON_HEIGHT;

	SkipWaveButton.textPositionX = SkipWaveButton.buttonData.x_origin + BUTTON_WIDTH * 0.5f;
	SkipWaveButton.textPositionY = SkipWaveButton.buttonData.y_origin + BUTTON_HEIGHT * 0.5f;
	strcpy_s(SkipWaveButton.textString, sizeof(SkipWaveButton.textString), "Skip");
}

void render_wave_timer_bar(float timeLeft, float maxTime, float barWidth, float barHeight)
{
	float percentage = timeLeft / maxTime;
	// draw red, then green
	CP_Settings_Fill(COLOR_RED);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth, barHeight);
	CP_Settings_Fill(COLOR_GREEN);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth * percentage, barHeight);
}

void render_wave_timer(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	render_wave_timer_bar(building_time, kFullBuildingPhaseTime, CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.1f);
	CP_Settings_Fill(COLOR_BLACK);
	char buffer[16] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "Time Left: %.1f", building_time);
	CP_Font_DrawText(buffer, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.05f);
}

void reduce_building_phase_time()
{
	if (building_time < 0.05f)
	{
		SetBuildingTime(0.0f);
		current_game_state = kWave;
		if (Level.current_wave == 0) {
			Level.current_effect = 0;
		}
		else if (Level.current_wave < 5) {
			Level.current_effect = CP_Random_RangeInt(0, 10);
		}
		else {
			Level.current_effect = CP_Random_RangeInt(0, 11);
		}
		StartBattleFieldEffectTimer(Level.current_effect);
	}
	else
	{
		SetBuildingTime(building_time - CP_System_GetDt());
	}
}

void SetBuildingTime(float newBuildingTime)
{
	// set building phase time to 0.0f
	building_time = newBuildingTime;
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
	if (current_game_state == kLose)
	{
		CP_Font_DrawText("Game Lost!", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.4f);
	}
	else if (current_game_state == kWin)
	{
		CP_Font_DrawText("Game Won!", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.4f);
	}
	CP_Settings_RectMode(CP_POSITION_CORNER);

	render_ui_button(EndScreenButtons[0]);
	render_ui_button(EndScreenButtons[1]);

	if (current_game_level < 4)
	{
		render_ui_button(EndScreenButtons[2]);
	}
}

void game_win_lose_check(void)
{
	// checks portal health && number of enemies left.
	if (Level.health <= 0)
	{
		// free memory
		for (int i = 0; i < level_grid_rows; i++) {
			free(Level.grid[i]);
		}
		free(Level.grid);
		//Free memory for turret_on_grid
		for (int i = 0; i < level_grid_cols; i++) {
			free(turret_on_grid[i]);
		}
		free(turret_on_grid);

		// game lost
		current_game_state = kLose;

	}
	else if (enemies_left == 0)
	{
		if (Level.current_wave == kMaxNumberOfWave - 1)
		{
			// free memory
			for (int i = 0; i < level_grid_rows; i++) {
				free(Level.grid[i]);
			}
			free(Level.grid);
			//Free memory for turret_on_grid
			for (int i = 0; i < level_grid_cols; i++) {
				free(turret_on_grid[i]);
			}
			free(turret_on_grid);
			current_game_state = kWin;
		}
		else // if still in the midst of the current level
		{
			SetBuildingTime(kFullBuildingPhaseTime);
			current_game_state = kBuilding;
			Level.current_wave += 1;
		}
	}
}

void init_next_level(int nextGameLevel)
{
	switch (nextGameLevel)
	{
	case 0:
	{
		Level1Init();
		break;
	}
	case 1:
	{
		Level2Init();
		break;
	}
	case 2:
	{
		Level3Init();
		break;
	}
	case 3:
	{
		Level4Init();
		break;
	}
	case 4:
	{
		Level5Init();
		break;
	}


	}
}

#pragma endregion

#pragma region UI Movement

float linear(float start, float end, float value)
{
	// taken from easing.h, credit goes to prof gerald
	return (1.f - value) * start + value * end;
}

// Updates a moving button's coordinates with regards to time.
Button ui_button_movement(Button button, float destPosX, float destPosY)
{
	if (button.movementTime <= MOVE_DURATION)
	{
		button.movementTime += CP_System_GetDt();
		button.buttonData.x_origin = linear(button.buttonData.x_origin, destPosX, button.movementTime / MOVE_DURATION);
		button.buttonData.y_origin = linear(button.buttonData.y_origin, destPosY, button.movementTime / MOVE_DURATION);
		button.textPositionX = linear(button.textPositionX, destPosX + BUTTON_WIDTH * 0.5f, button.movementTime / MOVE_DURATION);
		button.textPositionY = linear(button.textPositionY, destPosY + BUTTON_HEIGHT * 0.5f, button.movementTime / MOVE_DURATION);
	}
	else
	{
		// Most, if not all buttons take ~3-4s to finish moving.
		button.movementTime = 0.0f;
	}
	return button;
}

// Similar to ui_btn_movement, but meant for grpahic rectangles (e.g. the credit screen background)
Coordinates coord_movement(Coordinates coord, float destPosX, float destPosY)
{
	if (creditTextMoveTime <= MOVE_DURATION)
	{
		creditTextMoveTime += CP_System_GetDt();
		coord.x_origin = linear(coord.x_origin, destPosX, creditTextMoveTime / MOVE_DURATION);
		coord.y_origin = linear(coord.y_origin, destPosY, creditTextMoveTime / MOVE_DURATION);
	}
	else
	{
		// May be good to try basing this on something else instead like distance.
		creditTextMoveTime = 0.0f;
	}
	return coord;
}

// Moving stops when the credit screen's back button has finished moving.
void move_credits_screen(void)
{
	for (int i = 0; i < sizeof(CreditTexts) / sizeof(CreditLine); i++)
	{
		if (current_game_state == kMainMenu)
		{
			CreditTexts[i].currentPos = coord_movement(CreditTexts[i].currentPos, CreditTexts[i].creditPos.x_origin,
				CreditTexts[i].creditPos.y_origin); // from l_select back to main menu
		}
		else if (current_game_state == kCredits) // going to main menu
		{
			CreditTexts[i].currentPos = coord_movement(CreditTexts[i].currentPos, CreditTexts[i].mainMenuPos.x_origin,
				CreditTexts[i].mainMenuPos.y_origin); // from l_select back to main menu
		}
	}

	if (current_game_state == kMainMenu)
	{
		creditRectCoords = coord_movement(creditRectCoords, creditRectCoords.x_origin, CP_System_GetWindowHeight() * 0.3f);
		CreditsBackButton = ui_button_movement(CreditsBackButton, CreditsBackButton.buttonData.x_origin, CP_System_GetWindowHeight() * 0.93f);
	}
	else if (current_game_state == kCredits)
	{
		creditRectCoords = coord_movement(creditRectCoords, creditRectCoords.x_origin, CP_System_GetWindowHeight() * 1.3f);
		CreditsBackButton = ui_button_movement(CreditsBackButton, CreditsBackButton.buttonData.x_origin, CP_System_GetWindowHeight() * 2.0f);
	}
}

void move_level_select(void)
{
	for (int i = 0; i < kMaxNumberOfLevel; i++)
	{
		if (current_game_state == kLevelSelect) // going to main menu
		{
			LevelButtons[i] = ui_button_movement(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f)); // from l_select back to main menu
		}
		else if (current_game_state == kMainMenu)
		{
			LevelButtons[i] = ui_button_movement(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() * 0.35f + i * (BUTTON_HEIGHT + 25.0f)); // from l_select back to main menu
		}
	}
	if (current_game_state == kLevelSelect) // going to main menu
	{
		LevelSelectBackButton = ui_button_movement(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
			CP_System_GetWindowHeight() * 1.50f); // from l_select back to main menu
	}
	else if (current_game_state == kMainMenu)
	{
		LevelSelectBackButton = ui_button_movement(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
			CP_System_GetWindowHeight() * 0.8f); // from l_select back to main menu
	}
}

int level_select_finished_moving(void)
{
	if (current_game_state == kLevelSelect)
	{
		if (button_has_finished_moving(*LevelButtons, LevelButtons->buttonData.x_origin, (float)CP_System_GetWindowHeight()))
		{
			for (Button* b = LevelButtons; b < LevelButtons + kMaxNumberOfLevel; b++)
			{
				b->movementTime = 0.0f;
			}
			LevelSelectBackButton.movementTime = 0.0f;
			return 1;
		}
	}
	else if (current_game_state == kMainMenu)
	{
		if (button_has_finished_moving(*LevelButtons, LevelButtons->buttonData.x_origin, CP_System_GetWindowHeight() * 0.35f))
		{
			for (Button* b = LevelButtons; b < LevelButtons + kMaxNumberOfLevel; b++)
			{
				b->movementTime = 0.0f;
			}
			LevelSelectBackButton.movementTime = 0.0f;
			return 1;
		}
	}
	return 0;
}

void move_main_menu(void)
{
	// The Play, Credits && Quit buttons should move to and from the left,right && bottom of the screen respectively. HowToPlay moves to the bottom too.
	if (current_game_state == kMainMenu)
	{
		PlayButton = ui_button_movement(PlayButton, -BUTTON_WIDTH, PlayButton.buttonData.y_origin);
		CreditsButton = ui_button_movement(CreditsButton, CreditsButton.buttonData.x_origin, (float)CP_System_GetWindowHeight());
		QuitButton = ui_button_movement(QuitButton, (float)CP_System_GetWindowWidth(), QuitButton.buttonData.y_origin);
		HowToPlayButton = ui_button_movement(HowToPlayButton, HowToPlayButton.buttonData.x_origin, CP_System_GetWindowHeight() * 1.25f);
	}
	else if (current_game_state == kLevelSelect)
	{
		PlayButton = ui_button_movement(PlayButton, CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, PlayButton.buttonData.y_origin);
		CreditsButton = ui_button_movement(CreditsButton, CreditsButton.buttonData.x_origin, CP_System_GetWindowHeight() * 0.5f);
		QuitButton = ui_button_movement(QuitButton, CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, QuitButton.buttonData.y_origin);
		HowToPlayButton = ui_button_movement(HowToPlayButton, HowToPlayButton.buttonData.x_origin, CP_System_GetWindowHeight() * 0.75f);
	}
}

int main_menu_finished_moving(void)
{
	if (current_game_state == kMainMenu)
	{
		if (button_has_finished_moving(PlayButton, -BUTTON_WIDTH, PlayButton.buttonData.y_origin) &&
			button_has_finished_moving(CreditsButton, CreditsButton.buttonData.x_origin, (float)CP_System_GetWindowHeight()) &&
			button_has_finished_moving(QuitButton, (float)CP_System_GetWindowWidth(), QuitButton.buttonData.y_origin) &&
			button_has_finished_moving(HowToPlayButton, HowToPlayButton.buttonData.x_origin, (float)CP_System_GetWindowHeight() * 1.25f))
		{
			PlayButton.isMoving = CreditsButton.isMoving = QuitButton.isMoving = HowToPlayButton.isMoving = LevelButtons->isMoving = 0;
			PlayButton.movementTime = CreditsButton.movementTime = QuitButton.movementTime = HowToPlayButton.movementTime = 0.0f;
			return 1;
		}
	}
	else if (current_game_state == kLevelSelect)
	{
		if (button_has_finished_moving(PlayButton, CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, PlayButton.buttonData.y_origin) &&
			button_has_finished_moving(CreditsButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f) &&
			button_has_finished_moving(QuitButton, CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, QuitButton.buttonData.y_origin) &&
			button_has_finished_moving(HowToPlayButton, HowToPlayButton.buttonData.x_origin, CP_System_GetWindowHeight() * 0.75f))
		{
			PlayButton.isMoving = CreditsButton.isMoving = QuitButton.isMoving = HowToPlayButton.isMoving = LevelButtons->isMoving = 0;
			PlayButton.movementTime = CreditsButton.movementTime = QuitButton.movementTime = HowToPlayButton.movementTime = 0.0f;
			return 1;
		}
	}

	return 0;
}

int button_has_finished_moving(Button button, float destPosX, float destPosY)
{
	return (button.buttonData.x_origin == destPosX && button.buttonData.y_origin == destPosY);
}


#pragma endregion

#pragma region Options Settings

// Play a sound track
void play_sound_track(CP_Sound sound)
{
	CP_Sound_Play(sound);
}

// Mute a sound track
void mute_sound_track(void)
{

}

// Mute or resume only the music sound(s)
void toggle_background_sound(int muted)
{
	if (muted) CP_Sound_PauseGroup(CP_SOUND_GROUP_MUSIC); else CP_Sound_ResumeGroup(CP_SOUND_GROUP_MUSIC);
}

// Mute all audio tracks
void mute_all_audio(void)
{
	CP_Sound_StopAll();
}

// Set the volume level of a sound track.
void set_volume_level(float newVolumeLevel)
{
	// a Sound struct may be needed for better control of sound volumes.
	CP_Sound mySound = NULL;
	CP_Sound_PlayAdvanced(mySound, newVolumeLevel, 2.0f, TRUE, CP_SOUND_GROUP_1);
}

void toggle_fullscreen_windowed(int isWindowed)
{
	isWindowed ? CP_System_SetWindowSize(1280.0f, 1280.0f * 1080.0f / 1920.0f) : CP_System_Fullscreen();
}

#pragma endregion
