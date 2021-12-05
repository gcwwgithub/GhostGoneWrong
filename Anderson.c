/*!
All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Anderson.c
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@coauthor   Ng Zheng Wei (zhengwei.ng@digipen.edu)
			Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@course     CSD 1400
@section    C
@date       03/12/2021
@brief    	This source file contains the definitions for the main menu UI,
			the transitions between the building & wave phases, as well as the
			checking for win or lose conditions.
			Functions:InitGameFont, InitSplashLogos, InitTextButton, InitMainMenu, InitLevelSelectButtons,
			InitPauseScreen, InitEndScreen, InitCreditLine, InitOptionLine, InitCreditsScreen, InitOptionsScreen.
			CursorOverButton,RenderTitleScreen, RenderMainMenu, RenderLevelSelectButtons, RenderPauseScreen,
			RenderTextLine, RenderCreditsScreen, RenderOptionsScreen, RenderLogos,
			UIButtonMovement,CoordinateMovement,CoordinateMovement,MoveCreditsScreen,
			MoveLevelSelect,LevelSelectFinishedMoving,MoveMainMenu,MainMenuFinishedMoving,ButtonHasFinishedMoving, ExitToDesktop.
			InitSkipWaveButton,RenderWaveTimerBar,RenderWaveTimer,ReduceBuildingPhaseTime,SetBuildingTime,
			RenderEndScreen,GameWinLoseCheck,InitNextLevel.
*//*__________________________________________________________________________*/
#include "cprocessing.h"
#include "game.h"
#include "Anderson.h"

#include <stdio.h>

#pragma region UI

#pragma region Initialisations

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Sets the game font to pixelFont (i.e. VT323-Regular)
@param		void
@return		void
*//*_____________________________________________________________*/
void InitGameFont(void)
{
	pixelFont = GAME_FONT;
	CP_Font_Set(pixelFont);
	CP_Settings_TextSize(FONT_SIZE);
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Loads in the DP logo and the team logo from the Assets folder.
@param		void
@return		void
*//*_____________________________________________________________*/
void InitSplashLogos(void)
{
	digipen_logo = CP_Image_Load("./Assets/DigipenLogo.png");
	down_n_out_logo = CP_Image_Load("./Assets/DownNOut.png");
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialises a text button, its dimensions, position, its
			text to be written, and text position. All buttons are
			set to be rectangles. Text position is center-aligned
			and anchored from the button rectangle's top left corner.
@param		Button button - the button to be initialised.
			float buttonPosX,buttonPosY - Position of the button.
			float buttonWidth,buttonHeight - Dimensions of the button.
			float textPosX, textPosY - Position of the button's text.
			char string[] - the text to be entered.
@return		Button button - the initialised button.
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialises the title screen buttons.
@param		void
@return		void
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialises the level buttons in a column.
@param		void
@return		void
*//*_____________________________________________________________*/
void InitLevelSelectButtons(void)
{
	int c = 0; char levelNumberText[8];
	for (int i = 0; i < kMaxNumberOfLevel; i++)
	{
		c = snprintf(levelNumberText, 8, "Level %d", i + 1);
		strcpy_s(LevelButtons[i].textString, sizeof(LevelButtons[i].textString), levelNumberText);
		LevelButtons[i] = InitTextButton(LevelButtons[i],
			CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f) + 5.0f,
			BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, levelNumberText);
	}

	// Back button initialised seperately.
	LevelSelectBackButton = InitTextButton(LevelSelectBackButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 1.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Back");
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialises the pause screen.
@param		void
@return		void
*//*_____________________________________________________________*/
void InitPauseScreen(void)
{
	// Resume
	PauseScreenButtons[0] = InitTextButton(PauseScreenButtons[0],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.2f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Resume Game");
	// Exit Level
	PauseScreenButtons[1] = InitTextButton(PauseScreenButtons[1],
		CP_System_GetWindowWidth() * 0.45f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.3f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH * 1.7f, BUTTON_HEIGHT, BUTTON_WIDTH * 0.85f, BUTTON_HEIGHT * 0.5f, "Return to Main Menu");
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialises the end screen.
@param		void
@return		void
*//*_____________________________________________________________*/
void InitEndScreen(void)
{
	// Back to Main Menu
	EndScreenButtons[0] = InitTextButton(EndScreenButtons[0],
		CP_System_GetWindowWidth() * 0.45f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.5f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH * 1.7f, BUTTON_HEIGHT, BUTTON_WIDTH * 0.85f, BUTTON_HEIGHT * 0.5f, "Return to Main Menu");

	// Restart
	EndScreenButtons[1] = InitTextButton(EndScreenButtons[1],
		CP_System_GetWindowWidth() * 0.49f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.6f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH * 1.1f, BUTTON_HEIGHT, BUTTON_WIDTH * 0.55f, BUTTON_HEIGHT * 0.5f, "Restart Game");

	// Next Level
	EndScreenButtons[2] = InitTextButton(EndScreenButtons[2],
		CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.7f - BUTTON_HEIGHT * 0.5f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Next");
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialise a line for the credits screen.
@param		int num - position on CreditTexts array.
			char* line - text to be filled.
			float x,y - position values for the line.
@return		void
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialise a line for the options screen.
@param		int num - position on OptionTexts array.
			char* line - text to be filled.
			float x,y - position values for the line.
@return		void
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialises the credit screen background, as well as
			all the individual credits, line-by-line.
@param		void
@return		void
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialises the options screen.
@param		void
@return		void
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Checks if cursor is over a button.
@param		Coordinates buttonCoord - the coordinates of the button.
@return		1 if cursor is over the button, 0 otherwise.
*//*_____________________________________________________________*/
int CursorOverButton(Coordinates buttonCoord)
{
	return ((CP_Input_GetMouseX() > buttonCoord.x_origin) && (CP_Input_GetMouseX() < buttonCoord.x_origin + buttonCoord.width)
		&& (CP_Input_GetMouseY() > buttonCoord.y_origin) && (CP_Input_GetMouseY() < buttonCoord.y_origin + buttonCoord.height));
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@coauthor	Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@brief		Draws the background image of the title screen.
@param		Coordinates buttonCoord - the coordinates of the button.
@return		void
*//*_____________________________________________________________*/
void RenderTitleScreen(void)
{
	RenderImageFromSpriteSheetWithAlpha(background_spritesheet, background_spritesheet_array[0], CP_System_GetWindowWidth() * 0.5f, (float)CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 150);
	CP_Image_Draw(game_title_image, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.2f, 256 * scaling_factor, 256 * scaling_factor, 255);
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Renders a button. If moused over, the button is enlarged
			before being rendered.
@param		Button button - the button to render.
@return		void
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Renders all main menu UI buttons.
@param		void
@return		void
*//*_____________________________________________________________*/
void RenderMainMenu(void)
{
	for (int i = 0; i < 5; i++)
	{
		RenderUIButton(MainMenuButtons[i]);
	}
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Renders all level select UI buttons.
@param		void
@return		void
*//*_____________________________________________________________*/
void RenderLevelSelectButtons(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	for (int i = 0; i < 5; i++)
	{
		RenderUIButton(LevelButtons[i]);
	}
	RenderUIButton(LevelSelectBackButton);
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Renders pause screen.
@param		void
@return		void
*//*_____________________________________________________________*/
void RenderPauseScreen(void)
{
	CP_Settings_Fill(COLOR_GREY);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.15f, CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.2f);
	RenderUIButton(PauseScreenButtons[0]);
	RenderUIButton(PauseScreenButtons[1]);
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Renders a text line at its set position.
@param		Line line - the text line to render.
@return		void
*//*_____________________________________________________________*/
void RenderTextLine(Line line)
{
	CP_Font_DrawText(line.text, line.currentPos.x_origin, line.currentPos.y_origin);
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Renders credits screen.
@param		void
@return		void
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Renders options screen.
@param		void
@return		void
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Renders the Digipen and team logos in the logo splash screen.
@param		void
@return		void
*//*_____________________________________________________________*/
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

#pragma endregion

#pragma region UI Movement

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Formula used for interpolating UI button movement.
			Adapted from code by Prof. Gerald.
@param		float start - the starting value.
			float end - the end value.
			float value - percentage between start and end.
@return		float - the resulting value for next frame.
*//*_____________________________________________________________*/
float Linear(float start, float end, float value)
{
	return (1.f - value) * start + value * end;
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Updates a moving button's coordinates with regards to time.
@param		Button button - the button being moved.
			float destPosX,destPosY - the new position values.
@return		Button button - with the next frame's position values.
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Moves non-button object positions to new ones.
@param		Coordinates coord - coordinates of the object.
			float destPosX,destPosY - the new position values.
@return		Coordinates coord - the position values for next frame.
*//*_____________________________________________________________*/
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
		creditTextMoveTime = 0.0f;
	}
	return coord;
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Moves the credits buttons to new positions. Moving stops
			when the credit screen's back button has finished moving.
@param		void
@return		void
*//*_____________________________________________________________*/
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Moves the level select buttons to new positions.
@param		void
@return		void
*//*_____________________________________________________________*/
void MoveLevelSelect(void)
{
	for (int i = 0; i < kMaxNumberOfLevel; i++)
	{
		if (current_game_state == kLevelSelect) // going to main menu
		{
			LevelButtons[i] = UIButtonMovement(LevelButtons[i], CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f,
				CP_System_GetWindowHeight() + i * (BUTTON_HEIGHT + 25.0f) + 5.0f); // from level select back to main menu
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Checks if all level select UI has moved to its new positions.
@param		void
@return		1 if position reached, 0 otherwise.
*//*_____________________________________________________________*/
int LevelSelectFinishedMoving(void)
{
	if (current_game_state == kLevelSelect)
	{
		if (ButtonHasFinishedMoving(*LevelButtons, LevelButtons->buttonData.x_origin, (float)CP_System_GetWindowHeight() + 5.0f))
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Moves the main menu buttons to new positions.
@param		void
@return		void
*//*_____________________________________________________________*/
void MoveMainMenu(void)
{
	// The Play, Credits && Quit buttons should move to and from the left,bottom && right of the screen respectively. 
	// HowToPlay moves to the bottom. Options moves to the left.
	if (current_game_state == kMainMenu) // Going to Level Select
	{
		MainMenuButtons[StartButton] = UIButtonMovement(MainMenuButtons[StartButton], -2*BUTTON_WIDTH, MainMenuButtons[StartButton].buttonData.y_origin);
		MainMenuButtons[QuitButton] = UIButtonMovement(MainMenuButtons[QuitButton], (float)CP_System_GetWindowWidth() * 1.25f, MainMenuButtons[QuitButton].buttonData.y_origin);
		MainMenuButtons[CreditsButton] = UIButtonMovement(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, (float)CP_System_GetWindowHeight() * 1.25f);
		MainMenuButtons[HowToPlayButton] = UIButtonMovement(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, CP_System_GetWindowHeight() * 1.5f);
		MainMenuButtons[OptionsButton] = UIButtonMovement(MainMenuButtons[OptionsButton], -2*BUTTON_WIDTH, MainMenuButtons[OptionsButton].buttonData.y_origin);
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Checks if all main menu UI has moved to its new positions.
@param		void
@return		1 if position reached, 0 otherwise.
*//*_____________________________________________________________*/
int MainMenuFinishedMoving(void)
{
	if (current_game_state == kMainMenu)
	{
		if (ButtonHasFinishedMoving(MainMenuButtons[StartButton], -2*BUTTON_WIDTH, MainMenuButtons[StartButton].buttonData.y_origin) &&
			ButtonHasFinishedMoving(MainMenuButtons[QuitButton], (float)CP_System_GetWindowWidth() * 1.25f, MainMenuButtons[QuitButton].buttonData.y_origin) &&
			ButtonHasFinishedMoving(MainMenuButtons[CreditsButton], MainMenuButtons[CreditsButton].buttonData.x_origin, (float)CP_System_GetWindowHeight() * 1.25f) &&
			ButtonHasFinishedMoving(MainMenuButtons[HowToPlayButton], MainMenuButtons[HowToPlayButton].buttonData.x_origin, (float)CP_System_GetWindowHeight() * 1.5f) &&
			ButtonHasFinishedMoving(MainMenuButtons[OptionsButton], -2*BUTTON_WIDTH, MainMenuButtons[OptionsButton].buttonData.y_origin))
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Checks if a button has moved to a new position.
@param		Button button - the button to be checked.
			float destPosX,destPosY - the position that the button
									  is moving to.
@return		1 if position reached, 0 otherwise.
*//*_____________________________________________________________*/
int ButtonHasFinishedMoving(Button button, float destPosX, float destPosY)
{
	return (button.buttonData.x_origin == destPosX && button.buttonData.y_origin == destPosY);
}

#pragma endregion

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Ends the program.
@param		void
@return		void
*//*_____________________________________________________________*/
void ExitToDesktop(void)
{
	CP_Engine_Terminate();
}

#pragma endregion

#pragma region Building / Wave Phase

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialises the skip Wave button,to be below the timer bar.
@param		void
@return		void
*//*_____________________________________________________________*/
void InitSkipWaveButton(void)
{
	// Init skip wave button's position.
	SkipWaveButton = InitTextButton(SkipWaveButton, CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * 0.5f, CP_System_GetWindowHeight() * 0.1f,
		BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH * 0.5f, BUTTON_HEIGHT * 0.5f, "Skip");
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Draws a green to red bar representing the amount of time
			left in the building phase of a wave, the length of the
			green portion is dependent on time left.
@param		float timeLeft - time left in the building phase
			float maxTime - the amount of time given during the
							building phase.
			float barWidth,barHeight - the dimensions of the timer
									   bar
@return		void
*//*_____________________________________________________________*/
void RenderWaveTimerBar(float timeLeft, float maxTime, float barWidth, float barHeight)
{
	float percentage = timeLeft / maxTime;
	CP_Settings_Fill(COLOR_RED);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth, barHeight);
	CP_Settings_Fill(COLOR_GREEN);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f - barWidth * 0.5f, 0.0f, barWidth * percentage, barHeight);
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Displays the timer bar and time text.
@param		void
@return		void
*//*_____________________________________________________________*/
void RenderWaveTimer(void)
{
	CP_Settings_TextSize(FONT_SIZE);
	RenderWaveTimerBar(building_time, kFullBuildingPhaseTime, CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.1f);
	CP_Settings_Fill(COLOR_BLACK);
	char buffer[16] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "Time Left: %.1f", building_time);
	CP_Font_DrawText(buffer, CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.05f);
}

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@coauthor	Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@brief		Reduces the building time left till next wave. If no time
			is left, changes game state to kWave, sets effects and music.
@param		void
@return		void
*//*_____________________________________________________________*/
void ReduceBuildingPhaseTime(void)
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Sets building time left.
@param		void
@return		void
*//*_____________________________________________________________*/
void SetBuildingTime(float newBuildingTime)
{
	building_time = newBuildingTime;
}

#pragma endregion

#pragma region Win / Lose Conditions

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Renders the end screen when the player wins/loses,
@param		void
@return		void
*//*_____________________________________________________________*/
void RenderEndScreen(void)
{
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(COLOR_GREY);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f, 
		CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.5f);

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
	CP_Settings_TextSize(FONT_SIZE); // stops the win/lose text from being bigger from hovering over last btn
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

/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Initialises the nest level in the game.
@param		int nextGameLevel - the next level of the game.
@return		void
*//*_____________________________________________________________*/
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
