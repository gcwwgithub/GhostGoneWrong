/*!
All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Anderson.h
@author     Anderson Phua (a.phua@digipen.edu)
@course     CSD 1400
@section    C
@date       03/12/2021
@brief    	This header file contains the declarations for the main menu UI,
			the transitions between the building & wave phases, as well as the
			checking for win or lose conditions. Further comments are found below.
			All functions written by Anderson.

			Functions:
			InitGameFont, InitSplashLogos, RenderLogos, ...

*//*__________________________________________________________________________*/
#pragma once
#include "game.h"
#include "Gabriel.h"

#define COLOR_GREY CP_Color_Create(128, 128, 128, 255)
#define COLOR_YELLOW CP_Color_Create(255,255, 0, 255)

// Fixed resolution for scaling to bigger resolutions
#define BUTTON_WIDTH CP_System_GetWindowWidth() / 400 * 65.0f
#define BUTTON_HEIGHT CP_System_GetWindowHeight() / 400 * 40.0f
#define FONT_SIZE CP_System_GetWindowWidth() / 400 * 14.0f
#define MOVE_DURATION 5.0f
// Logos' display times
#define LOGO_DISPLAY_TIME 4.0f
#define FADE_OUT_TIME 1.0f

// Button object
typedef struct Button {
	Coordinates buttonData;
	float textPositionX;
	float textPositionY;
	float movementTime; // time to move from one position to another

	int isMoving;

	char textString[24];
}Button;

enum CreditText {

	CreditsTitle,
	TeamCredit,
	DevelopedBy,
	ZhengWei,
	Samuel,
	John,
	Gabriel,
	Anderson,

	Instructors,
	DX,
	Gerald,

	President,
	ClaudeComair,

	Executives,
	JasonChu_ChrisComair,
	Raymond_Samir,
	MichaelGats_MicheleComair,
	Erik_Melvin,
	BenEllinger_Johnny,
	Prasanna_John,
	Angela_Charles,

	CreatedAtDigipen,
	DigipenURL,
	CopyrightLine
};

enum OptionText {
	Options,
	BackgroundSFX,
	MuteAll
};

enum MainMenuButtonNames {
	StartButton,	CreditsButton,	QuitButton,
	OptionsButton,	HowToPlayButton
};

typedef struct Line
{
	const char* text;
	Coordinates mainMenuPos;
	Coordinates currentPos; // text will be at this position in MainMenu
	Coordinates endingPos;
}Line, CreditLine, OptionLine;

// Text lines in Credits and Options
CreditLine CreditTexts[25];
OptionLine OptionTexts[3];

// Credit UI movement controls
Coordinates creditRectCoords;
float creditTextMoveTime; // time for creditRect moving

// Logo Splash display objects and checks
CP_Image digipen_logo;
CP_Image down_n_out_logo;
CP_Font pixelFont;
float dpLogoDisplayTime;
float dpLogoFadeTime;
float teamLogoDisplayTime;
float teamLogoFadeTime;

// Main Menu UI Buttons
Button MainMenuButtons[5];
Coordinates OptionButtons[2];
Button OptionsBackButton;
Button LevelButtons[5];
Button LevelSelectBackButton;
Button CreditsBackButton;

// Gameplay UI
Button SkipWaveButton;
Button PauseScreenButtons[2];
Button EndScreenButtons[3];
int enemiesInLevel;

// Audio
int bgmAudioPaused;
int allAudioPaused;

// Logo Splash
void InitGameFont(void);
void InitSplashLogos(void);
void RenderLogos(void);

// UI initialisations
Button InitTextButton(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[]);
void InitMainMenu(void);
void InitLevelSelectButtons(void);
void InitPauseScreen(void);
void InitCreditsScreen(void);
void InitOptionsScreen(void);

// UI Rendering
void RenderUIButton(Button button);
void RenderTitleScreen(void);
void RenderMainMenu(void);
void RenderLevelSelectButtons(void);
void RenderPauseScreen(void);
void RenderCreditsScreen(void);
void RenderOptionsScreen(void);

// UI Movement
float Linear(float start, float end, float value);
Button UIButtonMovement(Button button, float destPosX, float destPosY);
int ButtonHasFinishedMoving(Button button, float destPosX, float destPosY);

// Movement of Level Select, Main Menu , Credits UI
void MoveLevelSelect(void);
int LevelSelectFinishedMoving(void);

void MoveMainMenu(void);
int MainMenuFinishedMoving(void);

void MoveCreditsScreen(void);

// Building / Wave Phase
void RenderWaveTimer(void);
void ReduceBuildingPhaseTime(void);
void InitSkipWaveButton(void);

// Win / Lose
void GameWinLoseCheck(void);
void InitEndScreen(void);
void RenderEndScreen(void);
void InitNextLevel(int nextGameLevel);

void ExitToDesktop(void);
