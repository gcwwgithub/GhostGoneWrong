﻿/*!
All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Anderson.c
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@coauthor   Ng Zheng Wei (zhengwei.ng@digipen.edu)
			Lim Jing Rui John (l.jingruijohn@digipen.edu)
@course     CSD 1400
@section    C
@date       03/12/2021
@brief    	This source file contains the definitions for the main menu UI,
			the transitions between the building & wave phases, as well as the
			checking for win or lose conditions.
*//*__________________________________________________________________________*/
#include "cprocessing.h"
#include "game.h"
#include "Anderson.h"

#pragma region UI

#pragma region Initialisations

// Sets the game font to pixelFont (i.e. VT323-Regular) 
void init_game_font(void)
{
	pixelFont = GAME_FONT;
	CP_Font_Set(pixelFont);
	CP_Settings_TextSize(FONT_SIZE);
}

// Loads in the DP logo and the team logo from the Assets folder.
void init_splash_logos(void)
{
	digipen_logo = CP_Image_Load("./Assets/DigipenLogo.png");
	down_n_out_logo = CP_Image_Load("./Assets/DownNOut.png");
}

// Assuming all buttons are rectangles. Text position is center-aligned and anchored from the button graphic's position.
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

// Initialises title screen buttons.
void init_main_menu(void)
{
	MainMenuButtons[StartButton] = init_text_button(MainMenuButtons[StartButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Start Game");
	MainMenuButtons[CreditsButton] = init_text_button(MainMenuButtons[CreditsButton], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Credits");
	MainMenuButtons[QuitButton] = init_text_button(MainMenuButtons[QuitButton], CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Quit Game");
	MainMenuButtons[HowToPlayButton] = init_text_button(MainMenuButtons[HowToPlayButton], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.75f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "How To Play");
	MainMenuButtons[OptionsButton] = init_text_button(MainMenuButtons[OptionsButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.75f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Options");
}

// Initialises the level buttons in a column.
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
		else // the first level was meant to be kind of a tutorial
		{
			LevelButtons[i] = init_text_button(LevelButtons[i],
				CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f),
				BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Tutorial");
		}
	}

	// Back button initialised seperately.
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

// Initialises the end screen buttons in a column.
void init_end_screen(void)
{
	// Back to Main Menu
	EndScreenButtons[0] = init_text_button(EndScreenButtons[0],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");

	// Restart
	EndScreenButtons[1] = init_text_button(EndScreenButtons[1],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.6f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Restart");

	// Next Level
	EndScreenButtons[2] = init_text_button(EndScreenButtons[2],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.7f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Next");
}

void init_credit_line(int num, char* line, float x, float y)
{
	CreditTexts[num].text = line;
	CreditTexts[num].mainMenuPos.x_origin = x;
	CreditTexts[num].mainMenuPos.y_origin = y + CP_System_GetWindowHeight();

	CreditTexts[num].endingPos.x_origin = x;
	CreditTexts[num].endingPos.y_origin = y;

	// Note: Starting position is (initialPosX, initialPosY + CP_Window_Width());
	CreditTexts[num].currentPos.x_origin = x;
	CreditTexts[num].currentPos.y_origin = y + CP_System_GetWindowHeight();
}

void init_option_line(int num, char* line, float x, float y)
{
	OptionTexts[num].text = line;
	OptionTexts[num].mainMenuPos.x_origin = x;
	OptionTexts[num].mainMenuPos.y_origin = y + CP_System_GetWindowHeight();

	OptionTexts[num].endingPos.x_origin = x;
	OptionTexts[num].endingPos.y_origin = y;

	OptionTexts[num].currentPos.x_origin = x;
	OptionTexts[num].currentPos.y_origin = y;
}

// Initialises the credit screen background, as well as all the individual credits, line-by-line.
void init_credits_screen(void)
{
	creditRectCoords.x_origin = CP_System_GetWindowWidth() * 0.05f; creditRectCoords.y_origin = CP_System_GetWindowHeight() * 1.3f;
	creditRectCoords.width = CP_System_GetWindowWidth() * 0.9f; creditRectCoords.height = CP_System_GetWindowHeight() * 0.6f;

	init_credit_line(CreditsTitle, "Credits", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.35f);
	init_credit_line(TeamCredit, "A DownNOut X Digipen Production", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.4f);

	init_credit_line(DevelopedBy, "Developed by:", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.5f);
	init_credit_line(ZhengWei, "Ng Zheng Wei", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.55f);
	init_credit_line(Samuel, "Samuel Wong Zhi Hao", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.6f);
	init_credit_line(John, "John Lim Jing Rui", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.65f);
	init_credit_line(Gabriel, "Gabriel Chiok Wei Wen", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.7f);
	init_credit_line(Anderson, "Anderson Phua Tai Dah", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.75f);

	init_credit_line(President, "President:", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f);
	init_credit_line(ClaudeComair, "Claude Comair", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.55f);

	init_credit_line(Instructors, "Instructors:", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.6f);
	init_credit_line(DX, "Cheng Ding Xiang", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.65f);
	init_credit_line(Gerald, "Gerald Wong", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.7f);

	init_credit_line(Executives, "Executives:", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.5f);
	init_credit_line(JasonChu_ChrisComair, "Jason Chu, Christopher Comair", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.55f);
	init_credit_line(MichaelGats_MicheleComair, "Michael Gats, Michele Comair", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.6f);
	init_credit_line(Raymond_Samir, "Raymond Yan, Samir Abou Samra", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.65f);
	init_credit_line(Prasanna_John, "Prasanna Ghali, John Bauer", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.7f);
	init_credit_line(Erik_Melvin, "Erik Mohrmann, Melvin Gonsalvez", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.75f);
	init_credit_line(Angela_Charles, "Angela Kugler, Charles Duba", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.80f);
	init_credit_line(BenEllinger_Johnny, "Benjamin Ellinger, Johnny Deek", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.85f);

	init_credit_line(CreatedAtDigipen, "Created at DigiPen Institute of Technology Singapore", CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.82f);
	init_credit_line(DigipenURL, "www.digipen.edu", CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.85f);
	// © copyright symbol is output as \xc2\xa9, its UTF-8 (i.e Unicode) string literal counterpart.
	init_credit_line(CopyrightLine, "All content \xc2\xa9 2021 DigiPen Institute of Technology Singapore, all rights reserved.",
		CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.88f);

	CreditsBackButton = init_text_button(CreditsBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 2.0f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
}

void init_options_screen(void)
{
	// init options screen for main menu - the lines and the two Coordinates-type 'buttons'.
	init_option_line(Options, "Options", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.35f);
	init_option_line(BackgroundSFX, "Background SFX", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.5f);
	init_option_line(MuteAll, "Mute All", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.65f);
	OptionsBackButton = init_text_button(OptionsBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.9f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");

	OptionButtons[0].x_origin = CP_System_GetWindowWidth() * 0.5f;
	OptionButtons[0].y_origin = CP_System_GetWindowHeight() * 0.475f;
	OptionButtons[0].width = 37.5f;
	OptionButtons[0].height = 37.5f;
	OptionButtons[0].object_type = kObjectRectangle;
	// Image left unspecified

	OptionButtons[1].x_origin = CP_System_GetWindowWidth() * 0.5f;
	OptionButtons[1].y_origin = CP_System_GetWindowHeight() * 0.625f;
	OptionButtons[1].width = 37.5f;
	OptionButtons[1].height = 37.5f;
	OptionButtons[1].object_type = kObjectRectangle;
	// Image left unspecified
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
			CP_Image_Draw(digipen_logo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(digipen_logo) * 0.5f, (float)CP_Image_GetHeight(digipen_logo) * 0.5f, (int)(255 * (dpLogoFadeTime / FADE_OUT_TIME)));
			CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
		}
		else // dp logo finished fading
		{
			if (teamLogoDisplayTime > 0.0f)
			{
				CP_Image_Draw(down_n_out_logo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(down_n_out_logo), (float)CP_Image_GetHeight(down_n_out_logo), 255);
				teamLogoDisplayTime -= CP_System_GetDt();
				CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);

			}
			else // team logo finished display
			{
				CP_Image_Draw(down_n_out_logo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(down_n_out_logo), (float)CP_Image_GetHeight(down_n_out_logo), (int)(255 * (teamLogoFadeTime / FADE_OUT_TIME)));
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
		CP_Image_Draw(digipen_logo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(digipen_logo) / 2, (float)CP_Image_GetHeight(digipen_logo) / 2, 255);
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

void render_main_menu(void)
{
	for (int i = 0; i < 5; i++)
	{
		render_ui_button(MainMenuButtons[i]);
	}
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

void render_text_line(Line line)
{
	CP_Font_DrawText(line.text, line.currentPos.x_origin, line.currentPos.y_origin);
}

void render_credits_screen(void)
{
	// Rendered from top to bottom, left to right order.
	CP_Settings_TextSize(FONT_SIZE);
	CP_Settings_Fill(COLOR_BLACK);

	CP_Graphics_DrawRect(creditRectCoords.x_origin, creditRectCoords.y_origin, creditRectCoords.width, creditRectCoords.height);

	CP_Settings_Fill(COLOR_WHITE);
	render_text_line(CreditTexts[CreditsTitle]);
	render_text_line(CreditTexts[TeamCredit]);
	render_text_line(CreditTexts[DevelopedBy]);

	CP_Settings_Fill(COLOR_YELLOW);
	render_text_line(CreditTexts[ZhengWei]);
	render_text_line(CreditTexts[Samuel]);
	render_text_line(CreditTexts[John]);
	render_text_line(CreditTexts[Gabriel]);
	render_text_line(CreditTexts[Anderson]);

	CP_Settings_Fill(COLOR_WHITE);
	render_text_line(CreditTexts[Instructors]);
	render_text_line(CreditTexts[President]);
	render_text_line(CreditTexts[Executives]);

	CP_Settings_Fill(COLOR_YELLOW);
	render_text_line(CreditTexts[DX]);
	render_text_line(CreditTexts[Gerald]);

	render_text_line(CreditTexts[ClaudeComair]);

	CP_Settings_TextSize(FONT_SIZE * 0.67f);
	render_text_line(CreditTexts[JasonChu_ChrisComair]);
	render_text_line(CreditTexts[MichaelGats_MicheleComair]);
	render_text_line(CreditTexts[Raymond_Samir]);
	render_text_line(CreditTexts[Prasanna_John]);
	render_text_line(CreditTexts[Erik_Melvin]);
	render_text_line(CreditTexts[Angela_Charles]);
	render_text_line(CreditTexts[BenEllinger_Johnny]);

	CP_Settings_Fill(COLOR_WHITE);
	CP_Settings_TextSize(FONT_SIZE * 0.5f);
	render_text_line(CreditTexts[CreatedAtDigipen]);
	render_text_line(CreditTexts[DigipenURL]);
	render_text_line(CreditTexts[CopyrightLine]);
	CP_Settings_TextSize(FONT_SIZE);

	render_ui_button(CreditsBackButton);
}

void render_options_screen(void)
{
	// render options
	CP_Settings_Fill(COLOR_BLACK);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.1f, CP_System_GetWindowHeight() * 0.3f, CP_System_GetWindowWidth() * 0.8f, CP_System_GetWindowHeight() * 0.55f);
	CP_Settings_Fill(COLOR_WHITE);

	CP_Settings_TextSize(FONT_SIZE);
	render_text_line(OptionTexts[Options]);
	render_text_line(OptionTexts[BackgroundSFX]);
	render_text_line(OptionTexts[MuteAll]);

	CP_Graphics_DrawRect(OptionButtons[0].x_origin, OptionButtons[0].y_origin, OptionButtons[0].width, OptionButtons[0].height);
	CP_Graphics_DrawRect(OptionButtons[1].x_origin, OptionButtons[1].y_origin, OptionButtons[1].width, OptionButtons[1].height);

	CP_Settings_Fill(COLOR_BLACK);
	if (!bgmAudioPaused)
	{
		CP_Graphics_DrawRect(OptionButtons[0].x_origin + 12.5f, OptionButtons[0].y_origin + 12.5f, 15.0f, 15.0f);
	}
	if (allAudioPaused)
	{
		CP_Graphics_DrawRect(OptionButtons[1].x_origin + 12.5f, OptionButtons[1].y_origin + 12.5f, 15.0f, 15.0f);
	}
	render_ui_button(OptionsBackButton);
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
			CP_Image_Draw(digipen_logo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(digipen_logo) * 0.5f, (float)CP_Image_GetHeight(digipen_logo) * 0.5f, (int)(255 * (dpLogoFadeTime / FADE_OUT_TIME)));
			CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
		}
		else // dp logo finished fading
		{
			if (teamLogoDisplayTime > 0.0f)
			{
				CP_Image_Draw(down_n_out_logo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(down_n_out_logo), (float)CP_Image_GetHeight(down_n_out_logo), 255);
				teamLogoDisplayTime -= CP_System_GetDt();
				CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
			}
			else // team logo finished display
			{
				CP_Image_Draw(down_n_out_logo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(down_n_out_logo), (float)CP_Image_GetHeight(down_n_out_logo), (int)(255 * (teamLogoFadeTime / FADE_OUT_TIME)));
				teamLogoFadeTime -= CP_System_GetDt();
				CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
				if (teamLogoFadeTime < 0.0f)
				{
					CP_Settings_TextSize(FONT_SIZE); // set font size back to normal.
					current_game_state = kMainMenu;
					CP_Sound_StopGroup(CP_SOUND_GROUP_1);
					CP_Sound_PlayAdvanced(MainMenuMusic, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);

					// if there was clicking during the logo displays, this is meant to prevent accidental triggering of buttons.
					MouseReset();
				}
			}
		}
	}
	else if (dpLogoDisplayTime >= 0.0f) // dp Logo still displaying
	{
		dpLogoDisplayTime -= CP_System_GetDt();
		CP_Image_Draw(digipen_logo, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_Image_GetWidth(digipen_logo) / 2, (float)CP_Image_GetHeight(digipen_logo) / 2, 255);
		CP_Font_DrawText(CreditTexts[CopyrightLine].text, (float)CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.95f);
	}
}

// Terminates game.
void exit_to_desktop(void)
{
	CP_Engine_Terminate();
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
			CreditTexts[i].currentPos = coord_movement(CreditTexts[i].currentPos, CreditTexts[i].endingPos.x_origin,
				CreditTexts[i].endingPos.y_origin); // from main menu to credits
		}
		else if (current_game_state == kCredits) // going to main menu
		{
			CreditTexts[i].currentPos = coord_movement(CreditTexts[i].currentPos, CreditTexts[i].mainMenuPos.x_origin,
				CreditTexts[i].mainMenuPos.y_origin); // from credits back to main menu
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
				CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f)); // from level select back to main menu
		}
		else if (current_game_state == kMainMenu)
		{
			LevelButtons[i] = ui_button_movement(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() * 0.35f + i * (BUTTON_HEIGHT + 25.0f)); // from main menu to level select
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
			CP_System_GetWindowHeight() * 0.8f); // from main menu to level select
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
				b->isMoving = 0;
			}
			LevelSelectBackButton.movementTime = 0.0f;
			return 1;
		}
	}
	else if (current_game_state == kMainMenu)
	{
		// Checks the first level button if it has stopped moving, as a check if the entire level select UI has stopped.
		if (button_has_finished_moving(*LevelButtons, LevelButtons->buttonData.x_origin, CP_System_GetWindowHeight() * 0.35f))
		{
			for (Button* b = LevelButtons; b < LevelButtons + kMaxNumberOfLevel; b++)
			{
				b->movementTime = 0.0f;
				b->isMoving = 0;
			}
			LevelSelectBackButton.movementTime = 0.0f;
			return 1;
		}
	}
	return 0;
}

void move_main_menu(void)
{
	// The Play, Credits && Quit buttons should move to and from the left,bottom && right of the screen respectively. 
	// HowToPlay moves to the bottom. Options moves to the left.
	if (current_game_state == kMainMenu) // Going to Level Select
	{
		MainMenuButtons[StartButton] = ui_button_movement(MainMenuButtons[StartButton], -BUTTON_WIDTH, MainMenuButtons[StartButton].buttonData.y_origin);
		MainMenuButtons[QuitButton] = ui_button_movement(MainMenuButtons[QuitButton], (float)CP_System_GetWindowWidth(), MainMenuButtons[QuitButton].buttonData.y_origin);
		MainMenuButtons[CreditsButton] = ui_button_movement(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, (float)CP_System_GetWindowHeight());
		MainMenuButtons[HowToPlayButton] = ui_button_movement(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, CP_System_GetWindowHeight() * 1.25f);
		MainMenuButtons[OptionsButton] = ui_button_movement(MainMenuButtons[OptionsButton], -BUTTON_WIDTH, MainMenuButtons[OptionsButton].buttonData.y_origin);
	}
	else if (current_game_state == kLevelSelect) // Going to Main Menu
	{
		MainMenuButtons[StartButton] = ui_button_movement(MainMenuButtons[StartButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, MainMenuButtons[StartButton].buttonData.y_origin);
		MainMenuButtons[QuitButton] = ui_button_movement(MainMenuButtons[QuitButton], CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, MainMenuButtons[QuitButton].buttonData.y_origin);
		MainMenuButtons[CreditsButton] = ui_button_movement(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, CP_System_GetWindowHeight() * 0.5f);
		MainMenuButtons[HowToPlayButton] = ui_button_movement(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, CP_System_GetWindowHeight() * 0.75f);
		MainMenuButtons[OptionsButton] = ui_button_movement(MainMenuButtons[OptionsButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, MainMenuButtons[OptionsButton].buttonData.y_origin);
	}
}

int main_menu_finished_moving(void)
{
	if (current_game_state == kMainMenu)
	{
		if (button_has_finished_moving(MainMenuButtons[StartButton], -BUTTON_WIDTH, MainMenuButtons[StartButton].buttonData.y_origin) &&
			button_has_finished_moving(MainMenuButtons[QuitButton], (float)CP_System_GetWindowWidth(), MainMenuButtons[QuitButton].buttonData.y_origin) &&
			button_has_finished_moving(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, (float)CP_System_GetWindowHeight()) &&
			button_has_finished_moving(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, (float)CP_System_GetWindowHeight() * 1.25f) &&
			button_has_finished_moving(MainMenuButtons[OptionsButton], -BUTTON_WIDTH, MainMenuButtons[OptionsButton].buttonData.y_origin))
		{
			for (int i = 0; i < 5; i++)
			{
				MainMenuButtons[i].isMoving = 0;
				MainMenuButtons[i].movementTime = 0.0f;
			}
			return 1;
		}
	}
	else if (current_game_state == kLevelSelect)
	{
		if (button_has_finished_moving(MainMenuButtons[StartButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, MainMenuButtons[StartButton].buttonData.y_origin) &&
			button_has_finished_moving(MainMenuButtons[QuitButton], CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, MainMenuButtons[QuitButton].buttonData.y_origin) &&
			button_has_finished_moving(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, CP_System_GetWindowHeight() * 0.5f) &&
			button_has_finished_moving(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, CP_System_GetWindowHeight() * 0.75f) &&
			button_has_finished_moving(MainMenuButtons[OptionsButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, MainMenuButtons[OptionsButton].buttonData.y_origin))
		{
			for (int i = 0; i < 5; i++)
			{
				MainMenuButtons[i].isMoving = 0;
				MainMenuButtons[i].movementTime = 0.0f;
			}
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

#pragma endregion

#pragma region Building / Wave Phase

// Skip Wave button, located directly below the green-red bar.
void init_skip_wave_button(void)
{
	// Init skip wave button's position.
	SkipWaveButton = init_text_button(SkipWaveButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.1f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Skip");
}

// Draws a green to red bar representing the amount of time left in buildingPhase.
void render_wave_timer_bar(float timeLeft, float maxTime, float barWidth, float barHeight)
{
	float percentage = timeLeft / maxTime;
	// draw red, then green
	CP_Settings_Fill(COLOR_RED);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth, barHeight);
	CP_Settings_Fill(COLOR_GREEN);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth * percentage, barHeight);
}

// Display the bar and time text, but not the skip button.
void render_wave_timer(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	render_wave_timer_bar(building_time, kFullBuildingPhaseTime, CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.1f);
	CP_Settings_Fill(COLOR_BLACK);
	char buffer[16] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "Time Left: %.1f", building_time);
	CP_Font_DrawText(buffer, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.05f);
}

// Controlling the building time left till next wave.
void reduce_building_phase_time()
{
	if (building_time < 0.05f)
	{
		SetBuildingTime(0.0f);
		current_game_state = kWave;
		if (level.current_wave == 0) {
			level.current_effect = 0;
		}
		else if (level.current_wave < 5) {
			level.current_effect = CP_Random_RangeInt(0, 10);
		}
		else {
			level.current_effect = CP_Random_RangeInt(0, 11);
		}
		StartBattleFieldEffectTimer(level.current_effect);
		CP_Sound_StopGroup(CP_SOUND_GROUP_1);
		CP_Sound_PlayAdvanced(WaveBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
	}
	else
	{
		SetBuildingTime(building_time - CP_System_GetDt());
	}
}

// Set building time left.
void SetBuildingTime(float newBuildingTime)
{
	building_time = newBuildingTime;
}

#pragma endregion

#pragma region Win / Lose Conditions

void render_end_screen(void)
{
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

	if (current_game_state == kWin)
	{
		if (level.current_game_level < 4)
		{
			render_ui_button(EndScreenButtons[2]); // Next
		}
	}
}


/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@coauthor   Ng Zheng Wei (zhengwei.ng@digipen.edu)
			Lim Jing Rui John (l.jingruijohn@digipen.edu)
@brief		Checks if the game conditions for winning or losing are
			met. If so, memory is freed, relevant sound is played,
			and the gameState is changed accordingly to win/lose.
@param		void
@return		void
*//*_____________________________________________________________*/
void game_win_lose_check(void)
{
	// checks portal health && number of enemies left.
	if (level.health <= 0)
	{
		// free memory
		for (int i = 0; i < level_grid_rows; i++) {
			free(level.grid[i]);
		}
		free(level.grid);
		//Free memory for turret_on_grid
		for (int i = 0; i < level_grid_cols; i++) {
			free(turret_on_grid[i]);
		}
		free(turret_on_grid);

		// game lost
		current_game_state = kLose;
		CP_Sound_PlayAdvanced(LoseSFX, SFX_Volume, 1.0, FALSE, CP_SOUND_GROUP_0);
	}
	else if (enemies_left == 0)
	{
		if (level.current_wave == kMaxNumberOfWave - 1)
		{
			// free memory
			for (int i = 0; i < level_grid_rows; i++) {
				free(level.grid[i]);
			}
			free(level.grid);
			//Free memory for turret_on_grid
			for (int i = 0; i < level_grid_cols; i++) {
				free(turret_on_grid[i]);
			}
			free(turret_on_grid);
			current_game_state = kWin;
			//Win SFX
			CP_Sound_PlayAdvanced(WinSFX, SFX_Volume, 1.0, FALSE, CP_SOUND_GROUP_0);
		}
		else // if still in the midst of the current level
		{
			SetBuildingTime(kFullBuildingPhaseTime);
			current_game_state = kBuilding;
			CP_Sound_StopGroup(CP_SOUND_GROUP_1);
			CP_Sound_PlayAdvanced(BuildingBGM, BGM_Volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
			level.current_wave += 1;
		}
	}
}

void init_next_level(int nextGameLevel)
{
	switch (nextGameLevel)
	{
	case 0:
	{
		InitLevel1();
		break;
	}
	case 1:
	{
		InitLevel2();
		break;
	}
	case 2:
	{
		InitLevel3();
		break;
	}
	case 3:
	{
		InitLevel4();
		break;
	}
	case 4:
	{
		InitLevel5();
		break;
	}
	}
}

#pragma endregion
