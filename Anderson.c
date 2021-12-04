/*!
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

			Functions:
			InitGameFont, InitSplashLogos, RenderLogos,
			InitTextButton, InitMainMenu, InitLevelSelectButtons, InitPauseScreen,
			InitCreditsScreen, InitOptionsScreen,


*//*__________________________________________________________________________*/
#include "cprocessing.h"
#include "game.h"
#include "Anderson.h"

#include <stdio.h>

#pragma region UI

#pragma region Initialisations

// Sets the game font to pixelFont (i.e. VT323-Regular)
void InitGameFont(void)
{
	pixelFont = GAME_FONT;
	CP_Font_Set(pixelFont);
	CP_Settings_TextSize(FONT_SIZE);
}

// Loads in the DP logo and the team logo from the Assets folder.
void InitSplashLogos(void)
{
	digipen_logo = CP_Image_Load("./Assets/DigipenLogo.png");
	down_n_out_logo = CP_Image_Load("./Assets/DownNOut.png");
}

// Assuming all buttons are rectangles. Text position is center-aligned and anchored from the button graphic's position.
Button InitTextButton(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[])
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
void InitMainMenu(void)
{
	MainMenuButtons[StartButton] = InitTextButton(MainMenuButtons[StartButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Start Game");
	MainMenuButtons[CreditsButton] = InitTextButton(MainMenuButtons[CreditsButton], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Credits");
	MainMenuButtons[QuitButton] = InitTextButton(MainMenuButtons[QuitButton], CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Quit Game");
	MainMenuButtons[HowToPlayButton] = InitTextButton(MainMenuButtons[HowToPlayButton], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.75f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "How To Play");
	MainMenuButtons[OptionsButton] = InitTextButton(MainMenuButtons[OptionsButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.75f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Options");
}

// Initialises the level buttons in a column.
void InitLevelSelectButtons(void)
{
	int c = 0; char levelNumberText[8];
	for (int i = 0; i < kMaxNumberOfLevel; i++)
	{
		c = snprintf(levelNumberText, 8, "Level %d", i);
		strcpy_s(LevelButtons[i].textString, sizeof(LevelButtons[i].textString), levelNumberText);
		if (i > 0)
		{
			LevelButtons[i] = InitTextButton(LevelButtons[i],
				CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f),
				BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, levelNumberText);
		}
		else // the first level was meant to be kind of a tutorial
		{
			LevelButtons[i] = InitTextButton(LevelButtons[i],
				CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f),
				BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Tutorial");
		}
	}

	// Back button initialised seperately.
	LevelSelectBackButton = InitTextButton(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 1.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
}

void InitPauseScreen(void)
{
	// Resume
	PauseScreenButtons[0] = InitTextButton(PauseScreenButtons[0],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.2f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Resume Game");
	// Exit Level
	PauseScreenButtons[1] = InitTextButton(PauseScreenButtons[1],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.3f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Exit Level");
}

// Initialises the end screen buttons in a column.
void InitEndScreen(void)
{
	// Back to Main Menu
	EndScreenButtons[0] = InitTextButton(EndScreenButtons[0],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");

	// Restart
	EndScreenButtons[1] = InitTextButton(EndScreenButtons[1],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.6f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Restart");

	// Next Level
	EndScreenButtons[2] = InitTextButton(EndScreenButtons[2],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.7f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Next");
}

void InitCreditLine(int num, char* line, float x, float y)
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

void InitOptionLine(int num, char* line, float x, float y)
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
void InitCreditsScreen(void)
{
	creditRectCoords.x_origin = CP_System_GetWindowWidth() * 0.05f; creditRectCoords.y_origin = CP_System_GetWindowHeight() * 1.3f;
	creditRectCoords.width = CP_System_GetWindowWidth() * 0.9f; creditRectCoords.height = CP_System_GetWindowHeight() * 0.6f;

	InitCreditLine(CreditsTitle, "Credits", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.35f);
	InitCreditLine(TeamCredit, "A DownNOut X Digipen Production", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.4f);

	InitCreditLine(DevelopedBy, "Developed by:", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.5f);
	InitCreditLine(ZhengWei, "Ng Zheng Wei", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.55f);
	InitCreditLine(Samuel, "Samuel Wong Zhi Hao", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.6f);
	InitCreditLine(John, "John Lim Jing Rui", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.65f);
	InitCreditLine(Gabriel, "Gabriel Chiok Wei Wen", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.7f);
	InitCreditLine(Anderson, "Anderson Phua Tai Dah", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.75f);

	InitCreditLine(President, "President:", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f);
	InitCreditLine(ClaudeComair, "Claude Comair", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.55f);

	InitCreditLine(Instructors, "Instructors:", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.6f);
	InitCreditLine(DX, "Cheng Ding Xiang", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.65f);
	InitCreditLine(Gerald, "Gerald Wong", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.7f);

	InitCreditLine(Executives, "Executives:", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.5f);
	InitCreditLine(JasonChu_ChrisComair, "Jason Chu, Christopher Comair", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.55f);
	InitCreditLine(MichaelGats_MicheleComair, "Michael Gats, Michele Comair", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.6f);
	InitCreditLine(Raymond_Samir, "Raymond Yan, Samir Abou Samra", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.65f);
	InitCreditLine(Prasanna_John, "Prasanna Ghali, John Bauer", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.7f);
	InitCreditLine(Erik_Melvin, "Erik Mohrmann, Melvin Gonsalvez", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.75f);
	InitCreditLine(Angela_Charles, "Angela Kugler, Charles Duba", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.80f);
	InitCreditLine(BenEllinger_Johnny, "Benjamin Ellinger, Johnny Deek", CP_System_GetWindowWidth() * 0.78f, CP_System_GetWindowHeight() * 0.85f);

	InitCreditLine(CreatedAtDigipen, "Created at DigiPen Institute of Technology Singapore", CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.82f);
	InitCreditLine(DigipenURL, "www.digipen.edu", CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.85f);
	// © copyright symbol is output as \xc2\xa9, its UTF-8 (i.e Unicode) string literal counterpart.
	InitCreditLine(CopyrightLine, "All content \xc2\xa9 2021 DigiPen Institute of Technology Singapore, all rights reserved.",
		CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.88f);

	CreditsBackButton = InitTextButton(CreditsBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 2.0f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
}

void InitOptionsScreen(void)
{
	// init options screen for main menu - the lines and the two Coordinates-type 'buttons'.
	InitOptionLine(Options, "Options", CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.35f);
	InitOptionLine(BackgroundSFX, "Background SFX", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.5f);
	InitOptionLine(SFX, "SFX", CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.65f);
	OptionsBackButton = InitTextButton(OptionsBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.9f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");

	OptionButtons[BackgroundSFX].x_origin = CP_System_GetWindowWidth() * 0.5f;
	OptionButtons[BackgroundSFX].y_origin = CP_System_GetWindowHeight() * 0.475f;
	OptionButtons[BackgroundSFX].width = 37.5f;
	OptionButtons[BackgroundSFX].height = 37.5f;
	OptionButtons[BackgroundSFX].object_type = kObjectRectangle;
	// Image left unspecified

	OptionButtons[SFX].x_origin = CP_System_GetWindowWidth() * 0.5f;
	OptionButtons[SFX].y_origin = CP_System_GetWindowHeight() * 0.625f;
	OptionButtons[SFX].width = 37.5f;
	OptionButtons[SFX].height = 37.5f;
	OptionButtons[SFX].object_type = kObjectRectangle;
	// Image left unspecified
}

#pragma endregion

#pragma region Rendering

int CursorOverButton(Coordinates buttonCoord)
{
	return ((CP_Input_GetMouseX() > buttonCoord.x_origin) && (CP_Input_GetMouseX() < buttonCoord.x_origin + buttonCoord.width)
		&& (CP_Input_GetMouseY() > buttonCoord.y_origin) && (CP_Input_GetMouseY() < buttonCoord.y_origin + buttonCoord.height));
}

// Draws a background image, background is drawn with code, rest with pixel art
void RenderTitleScreen(void)
{
	RenderImageFromSpriteSheetWithAlpha(background_spritesheet, background_spritesheet_array[0], CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 150);
	CP_Image_Draw(game_title_image, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.2f, 256 * scaling_factor, 256 * scaling_factor, 255);
}

void RenderUIButton(Button button)
{
	CP_Settings_Fill(COLOR_BLACK);

	float mouseOverSizeModifier = CursorOverButton(button.buttonData) ? 1.1f : 1.0f;

	// Adjusting position to account for enlarged button when moused over.
	CP_Graphics_DrawRect(button.buttonData.x_origin - (mouseOverSizeModifier - 1) * button.buttonData.width / 2.0f,
		button.buttonData.y_origin - (mouseOverSizeModifier - 1) * button.buttonData.height / 2.0f,
		button.buttonData.width * mouseOverSizeModifier, button.buttonData.height * mouseOverSizeModifier);

	(mouseOverSizeModifier - 1) ? CP_Settings_Fill(COLOR_GREY) : CP_Settings_Fill(COLOR_WHITE);

	CP_Settings_TextSize(CursorOverButton(button.buttonData) ? FONT_SIZE * mouseOverSizeModifier : FONT_SIZE);
	CP_Font_DrawText(button.textString, button.textPositionX, button.textPositionY);
}

void RenderMainMenu(void)
{
	for (int i = 0; i < 5; i++)
	{
		RenderUIButton(MainMenuButtons[i]);
	}
}

void RenderLevelSelectButtons(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	// render the 5 level buttons
	for (int i = 0; i < 5; i++)
	{
		RenderUIButton(LevelButtons[i]);
	}
	RenderUIButton(LevelSelectBackButton);
}

void RenderPauseScreen(void)
{
	CP_Settings_Fill(COLOR_GREY);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.15f, CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.2f);
	RenderUIButton(PauseScreenButtons[0]);
	RenderUIButton(PauseScreenButtons[1]);
}

void RenderTextLine(Line line)
{
	CP_Font_DrawText(line.text, line.currentPos.x_origin, line.currentPos.y_origin);
}

void RenderCreditsScreen(void)
{
	// Rendered from top to bottom, left to right order.
	CP_Settings_TextSize(FONT_SIZE);
	CP_Settings_Fill(COLOR_BLACK);

	CP_Graphics_DrawRect(creditRectCoords.x_origin, creditRectCoords.y_origin, creditRectCoords.width, creditRectCoords.height);

	CP_Settings_Fill(COLOR_WHITE);
	RenderTextLine(CreditTexts[CreditsTitle]);
	RenderTextLine(CreditTexts[TeamCredit]);
	RenderTextLine(CreditTexts[DevelopedBy]);

	CP_Settings_Fill(COLOR_YELLOW);
	RenderTextLine(CreditTexts[ZhengWei]);
	RenderTextLine(CreditTexts[Samuel]);
	RenderTextLine(CreditTexts[John]);
	RenderTextLine(CreditTexts[Gabriel]);
	RenderTextLine(CreditTexts[Anderson]);

	CP_Settings_Fill(COLOR_WHITE);
	RenderTextLine(CreditTexts[Instructors]);
	RenderTextLine(CreditTexts[President]);
	RenderTextLine(CreditTexts[Executives]);

	CP_Settings_Fill(COLOR_YELLOW);
	RenderTextLine(CreditTexts[DX]);
	RenderTextLine(CreditTexts[Gerald]);

	RenderTextLine(CreditTexts[ClaudeComair]);

	CP_Settings_TextSize(FONT_SIZE * 0.67f);
	RenderTextLine(CreditTexts[JasonChu_ChrisComair]);
	RenderTextLine(CreditTexts[MichaelGats_MicheleComair]);
	RenderTextLine(CreditTexts[Raymond_Samir]);
	RenderTextLine(CreditTexts[Prasanna_John]);
	RenderTextLine(CreditTexts[Erik_Melvin]);
	RenderTextLine(CreditTexts[Angela_Charles]);
	RenderTextLine(CreditTexts[BenEllinger_Johnny]);

	CP_Settings_Fill(COLOR_WHITE);
	CP_Settings_TextSize(FONT_SIZE * 0.5f);
	RenderTextLine(CreditTexts[CreatedAtDigipen]);
	RenderTextLine(CreditTexts[DigipenURL]);
	RenderTextLine(CreditTexts[CopyrightLine]);
	CP_Settings_TextSize(FONT_SIZE);

	RenderUIButton(CreditsBackButton);
}

void RenderOptionsScreen(void)
{
	CP_Settings_Fill(COLOR_BLACK);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.1f, CP_System_GetWindowHeight() * 0.3f, CP_System_GetWindowWidth() * 0.8f, CP_System_GetWindowHeight() * 0.55f);
	CP_Settings_Fill(COLOR_WHITE);

	CP_Settings_TextSize(FONT_SIZE);
	RenderTextLine(OptionTexts[Options]);
	RenderTextLine(OptionTexts[BackgroundSFX]);
	RenderTextLine(OptionTexts[SFX]);

	CP_Graphics_DrawRect(OptionButtons[BackgroundSFX].x_origin, OptionButtons[BackgroundSFX].y_origin, OptionButtons[BackgroundSFX].width, OptionButtons[0].height);
	CP_Graphics_DrawRect(OptionButtons[SFX].x_origin, OptionButtons[SFX].y_origin, OptionButtons[SFX].width, OptionButtons[SFX].height);

	CP_Settings_Fill(COLOR_BLACK);
	if (!bgmSFXEnabled)
	{
		CP_Graphics_DrawRect(OptionButtons[BackgroundSFX].x_origin + 12.5f, OptionButtons[BackgroundSFX].y_origin + 12.5f, 15.0f, 15.0f);
	}
	if (!sfxEnabled)
	{
		CP_Graphics_DrawRect(OptionButtons[SFX].x_origin + 12.5f, OptionButtons[SFX].y_origin + 12.5f, 15.0f, 15.0f);
	}
	RenderUIButton(OptionsBackButton);
}

void RenderLogos(void)
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
					CP_Sound_PlayAdvanced(main_menu_music, bgm_volume, 1.0f, TRUE, CP_SOUND_GROUP_1);

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
void ExitToDesktop(void)
{
	CP_Engine_Terminate();
}

#pragma endregion

#pragma region UI Movement

float Linear(float start, float end, float value)
{
	// taken from easing.h, credit goes to prof gerald
	return (1.f - value) * start + value * end;
}

// Updates a moving button's coordinates with regards to time.
Button UIButtonMovement(Button button, float destPosX, float destPosY)
{
	if (button.movementTime <= MOVE_DURATION)
	{
		button.movementTime += CP_System_GetDt();
		button.buttonData.x_origin = Linear(button.buttonData.x_origin, destPosX, button.movementTime / MOVE_DURATION);
		button.buttonData.y_origin = Linear(button.buttonData.y_origin, destPosY, button.movementTime / MOVE_DURATION);
		button.textPositionX = Linear(button.textPositionX, destPosX + BUTTON_WIDTH * 0.5f, button.movementTime / MOVE_DURATION);
		button.textPositionY = Linear(button.textPositionY, destPosY + BUTTON_HEIGHT * 0.5f, button.movementTime / MOVE_DURATION);
	}
	else
	{
		// Most, if not all buttons take ~3-4s to finish moving.
		button.movementTime = 0.0f;
	}
	return button;
}

// Similar to ui_btn_movement, but meant for grpahic rectangles (e.g. the credit screen background)
Coordinates CoordinateMovement(Coordinates coord, float destPosX, float destPosY)
{
	if (creditTextMoveTime <= MOVE_DURATION)
	{
		creditTextMoveTime += CP_System_GetDt();
		coord.x_origin = Linear(coord.x_origin, destPosX, creditTextMoveTime / MOVE_DURATION);
		coord.y_origin = Linear(coord.y_origin, destPosY, creditTextMoveTime / MOVE_DURATION);
	}
	else
	{
		// May be good to try basing this on something else instead like distance.
		creditTextMoveTime = 0.0f;
	}
	return coord;
}

// Moving stops when the credit screen's back button has finished moving.
void MoveCreditsScreen(void)
{
	for (int i = 0; i < sizeof(CreditTexts) / sizeof(CreditLine); i++)
	{
		if (current_game_state == kMainMenu)
		{
			CreditTexts[i].currentPos = CoordinateMovement(CreditTexts[i].currentPos, CreditTexts[i].endingPos.x_origin,
				CreditTexts[i].endingPos.y_origin); // from main menu to credits
		}
		else if (current_game_state == kCredits) // going to main menu
		{
			CreditTexts[i].currentPos = CoordinateMovement(CreditTexts[i].currentPos, CreditTexts[i].mainMenuPos.x_origin,
				CreditTexts[i].mainMenuPos.y_origin); // from credits back to main menu
		}
	}

	if (current_game_state == kMainMenu)
	{
		creditRectCoords = CoordinateMovement(creditRectCoords, creditRectCoords.x_origin, CP_System_GetWindowHeight() * 0.3f);
		CreditsBackButton = UIButtonMovement(CreditsBackButton, CreditsBackButton.buttonData.x_origin, CP_System_GetWindowHeight() * 0.93f);
	}
	else if (current_game_state == kCredits)
	{
		creditRectCoords = CoordinateMovement(creditRectCoords, creditRectCoords.x_origin, CP_System_GetWindowHeight() * 1.3f);
		CreditsBackButton = UIButtonMovement(CreditsBackButton, CreditsBackButton.buttonData.x_origin, CP_System_GetWindowHeight() * 2.0f);
	}
}

void MoveLevelSelect(void)
{
	for (int i = 0; i < kMaxNumberOfLevel; i++)
	{
		if (current_game_state == kLevelSelect) // going to main menu
		{
			LevelButtons[i] = UIButtonMovement(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f)); // from level select back to main menu
		}
		else if (current_game_state == kMainMenu)
		{
			LevelButtons[i] = UIButtonMovement(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() * 0.35f + i * (BUTTON_HEIGHT + 25.0f)); // from main menu to level select
		}
	}
	if (current_game_state == kLevelSelect) // going to main menu
	{
		LevelSelectBackButton = UIButtonMovement(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
			CP_System_GetWindowHeight() * 1.50f); // from l_select back to main menu
	}
	else if (current_game_state == kMainMenu)
	{
		LevelSelectBackButton = UIButtonMovement(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
			CP_System_GetWindowHeight() * 0.8f); // from main menu to level select
	}
}

int LevelSelectFinishedMoving(void)
{
	if (current_game_state == kLevelSelect)
	{
		if (ButtonHasFinishedMoving(*LevelButtons, LevelButtons->buttonData.x_origin, (float)CP_System_GetWindowHeight()))
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
		if (ButtonHasFinishedMoving(*LevelButtons, LevelButtons->buttonData.x_origin, CP_System_GetWindowHeight() * 0.35f))
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

void MoveMainMenu(void)
{
	// The Play, Credits && Quit buttons should move to and from the left,bottom && right of the screen respectively. 
	// HowToPlay moves to the bottom. Options moves to the left.
	if (current_game_state == kMainMenu) // Going to Level Select
	{
		MainMenuButtons[StartButton] = UIButtonMovement(MainMenuButtons[StartButton], -BUTTON_WIDTH, MainMenuButtons[StartButton].buttonData.y_origin);
		MainMenuButtons[QuitButton] = UIButtonMovement(MainMenuButtons[QuitButton], (float)CP_System_GetWindowWidth(), MainMenuButtons[QuitButton].buttonData.y_origin);
		MainMenuButtons[CreditsButton] = UIButtonMovement(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, (float)CP_System_GetWindowHeight());
		MainMenuButtons[HowToPlayButton] = UIButtonMovement(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, CP_System_GetWindowHeight() * 1.25f);
		MainMenuButtons[OptionsButton] = UIButtonMovement(MainMenuButtons[OptionsButton], -BUTTON_WIDTH, MainMenuButtons[OptionsButton].buttonData.y_origin);
	}
	else if (current_game_state == kLevelSelect) // Going to Main Menu
	{
		MainMenuButtons[StartButton] = UIButtonMovement(MainMenuButtons[StartButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, MainMenuButtons[StartButton].buttonData.y_origin);
		MainMenuButtons[QuitButton] = UIButtonMovement(MainMenuButtons[QuitButton], CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, MainMenuButtons[QuitButton].buttonData.y_origin);
		MainMenuButtons[CreditsButton] = UIButtonMovement(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, CP_System_GetWindowHeight() * 0.5f);
		MainMenuButtons[HowToPlayButton] = UIButtonMovement(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, CP_System_GetWindowHeight() * 0.75f);
		MainMenuButtons[OptionsButton] = UIButtonMovement(MainMenuButtons[OptionsButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, MainMenuButtons[OptionsButton].buttonData.y_origin);
	}
}

int MainMenuFinishedMoving(void)
{
	if (current_game_state == kMainMenu)
	{
		if (ButtonHasFinishedMoving(MainMenuButtons[StartButton], -BUTTON_WIDTH, MainMenuButtons[StartButton].buttonData.y_origin) &&
			ButtonHasFinishedMoving(MainMenuButtons[QuitButton], (float)CP_System_GetWindowWidth(), MainMenuButtons[QuitButton].buttonData.y_origin) &&
			ButtonHasFinishedMoving(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, (float)CP_System_GetWindowHeight()) &&
			ButtonHasFinishedMoving(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, (float)CP_System_GetWindowHeight() * 1.25f) &&
			ButtonHasFinishedMoving(MainMenuButtons[OptionsButton], -BUTTON_WIDTH, MainMenuButtons[OptionsButton].buttonData.y_origin))
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
		if (ButtonHasFinishedMoving(MainMenuButtons[StartButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, MainMenuButtons[StartButton].buttonData.y_origin) &&
			ButtonHasFinishedMoving(MainMenuButtons[QuitButton], CP_System_GetWindowWidth() * 0.75f - BUTTON_WIDTH * 0.5f, MainMenuButtons[QuitButton].buttonData.y_origin) &&
			ButtonHasFinishedMoving(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, CP_System_GetWindowHeight() * 0.5f) &&
			ButtonHasFinishedMoving(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, CP_System_GetWindowHeight() * 0.75f) &&
			ButtonHasFinishedMoving(MainMenuButtons[OptionsButton], CP_System_GetWindowWidth() * 0.25f - BUTTON_WIDTH * 0.5f, MainMenuButtons[OptionsButton].buttonData.y_origin))
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

int ButtonHasFinishedMoving(Button button, float destPosX, float destPosY)
{
	return (button.buttonData.x_origin == destPosX && button.buttonData.y_origin == destPosY);
}

#pragma endregion

#pragma endregion

#pragma region Building / Wave Phase

// Skip Wave button, located directly below the green-red bar.
void InitSkipWaveButton(void)
{
	// Init skip wave button's position.
	SkipWaveButton = InitTextButton(SkipWaveButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.1f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Skip");
}

// Draws a green to red bar representing the amount of time left in buildingPhase.
void RenderWaveTimerBar(float timeLeft, float maxTime, float barWidth, float barHeight)
{
	float percentage = timeLeft / maxTime;
	// draw red, then green
	CP_Settings_Fill(COLOR_RED);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth, barHeight);
	CP_Settings_Fill(COLOR_GREEN);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth * percentage, barHeight);
}

// Display the bar and time text, but not the skip button.
void RenderWaveTimer(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	RenderWaveTimerBar(building_time, kFullBuildingPhaseTime, CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.1f);
	CP_Settings_Fill(COLOR_BLACK);
	char buffer[16] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "Time Left: %.1f", building_time);
	CP_Font_DrawText(buffer, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.05f);
}

// Controlling the building time left till next wave.
void ReduceBuildingPhaseTime()
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
		CP_Sound_PlayAdvanced(wave_bgm, bgm_volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
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

void RenderEndScreen(void)
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

	RenderUIButton(EndScreenButtons[0]);
	RenderUIButton(EndScreenButtons[1]);

	if (current_game_state == kWin)
	{
		if (level.current_game_level < 4)
		{
			RenderUIButton(EndScreenButtons[2]); // Next
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
void GameWinLoseCheck(void)
{
	// checks portal health && number of enemies left.
	if (level.health <= 0)
	{
		// free memory
		for (int i = 0; i < level_grid_rows; i++) {
			free(level.grid[i]);
			level.grid[i] = NULL;
		}
		free(level.grid);
		level.grid = NULL;
		//Free memory for turret_on_grid
		for (int i = 0; i < level_grid_cols; i++) {
			free(turret_on_grid[i]);
			turret_on_grid[i] = NULL;
		}
		free(turret_on_grid);
		turret_on_grid = NULL;
		// game lost
		current_game_state = kLose;
		CP_Sound_PlayAdvanced(lose_sfx, sfx_volume, 1.0, FALSE, CP_SOUND_GROUP_0);
	}
	else if (enemies_left == 0)
	{
		if (level.current_wave == kMaxNumberOfWave - 1)
		{
			// free memory
			for (int i = 0; i < level_grid_rows; i++) {
				free(level.grid[i]);
				level.grid[i] = NULL;
			}
			free(level.grid);
			level.grid = NULL;
			//Free memory for turret_on_grid
			for (int i = 0; i < level_grid_cols; i++) {
				free(turret_on_grid[i]);
				turret_on_grid[i] = NULL;
			}
			free(turret_on_grid);
			turret_on_grid = NULL;
			current_game_state = kWin;
			//Win SFX
			CP_Sound_PlayAdvanced(win_sfx, sfx_volume, 1.0, FALSE, CP_SOUND_GROUP_0);
		}
		else // if still in the midst of the current level
		{
			SetBuildingTime(kFullBuildingPhaseTime);
			current_game_state = kBuilding;
			CP_Sound_StopGroup(CP_SOUND_GROUP_1);
			CP_Sound_PlayAdvanced(building_bgm, bgm_volume, 1.0f, TRUE, CP_SOUND_GROUP_1);
			level.current_wave += 1;
		}
	}
}

void InitNextLevel(int nextGameLevel)
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
