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

typedef struct Button {
	Coordinates buttonData;
	float textPositionX;
	float textPositionY;
	float movementTime; // time to move from one position to another

	int isMoving;

	char const * textString[24];
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
	const char const * text;
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
void init_game_font(void);
void init_splash_logos(void);
void render_logos(void);

// UI initialisations
Button init_text_button(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[]);
void init_main_menu(void);
void init_level_select_buttons(void);
void init_pause_screen(void);
void init_credits_screen(void);
void init_options_screen(void);

// UI Rendering
void render_ui_button(Button button);
void render_title_screen(void);
void render_main_menu(void);
void render_level_select_buttons(void);
void render_pause_screen(void);
void render_credits_screen(void);
void render_options_screen(void);

// UI Movement
float linear(float start, float end, float value);
Button ui_button_movement(Button button, float destPosX, float destPosY);
int button_has_finished_moving(Button button, float destPosX, float destPosY);

// Movement of Level Select, Main Menu , Credits UI
void move_level_select(void);
int level_select_finished_moving(void);

void move_main_menu(void);
int main_menu_finished_moving(void);

void move_credits_screen(void);

// Building / Wave Phase
void render_wave_timer(void);
void reduce_building_phase_time(void);
void init_skip_wave_button(void);

// Win / Lose
void game_win_lose_check(void);
void init_end_screen(void);
void render_end_screen(void);
void init_next_level(int nextGameLevel);

void exit_to_desktop(void);
