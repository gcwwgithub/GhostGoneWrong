#pragma once
#include "game.h"
#include "Gabriel.h"

#define COLOR_GREY CP_Color_Create(128, 128, 128, 255)
#define COLOR_YELLOW CP_Color_Create(255,255, 0, 255)

// Fixing resolution for easier scaling to bigger resolutions
#define BUTTON_WIDTH CP_System_GetWindowWidth() / 400 * 65.0f
#define BUTTON_HEIGHT CP_System_GetWindowHeight() / 400 * 40.0f
#define FONT_SIZE CP_System_GetWindowWidth() / 400 * 14.0f
#define MOVE_DURATION 5.0f
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
	JasonChu,
	SamirAbouSamra,
	MicheleComair,
	AngelaKugler,
	ErikMohrmann,
	BenjaminEllinger,
	MelvinGonsalvez,

	CreatedAtDigipen,
	DigipenURL,
	CopyrightLine
};

enum OptionText {
	Options,
	BackgroundSFX,
	MuteAll
};

typedef struct Line
{
	const char const * text;
	Coordinates mainMenuPos;
	Coordinates currentPos; // text will be at this position in MainMenu
	Coordinates endingPos;
}Line, CreditLine, OptionLine;

CreditLine CreditTexts[25];
OptionLine OptionTexts[3];

// creditRectCoords for moving bg
Coordinates creditRectCoords;
float creditTextMoveTime; // time for creditRect moving

CP_Image DigipenLogo;
CP_Image DownNOutLogo;
CP_Font pixelFont;
float dpLogoDisplayTime;
float dpLogoFadeTime;
float teamLogoDisplayTime;
float teamLogoFadeTime;

Button PlayButton;
Button QuitButton;
Button CreditsButton;
Button HowToPlayButton;

Button OptionsButton;
Button OptionsBackButton;

Button LevelSelectBackButton;
Button LevelButtons[5];
Button CreditsBackButton;

Button SkipWaveButton;
Button PauseScreenButtons[2];
Button EndScreenButtons[3];
int enemiesInLevel;


// Main Menu Inits
void render_title_screen(void);

void init_main_menu(void);

void init_level_select_buttons(void);
void init_pause_screen(void);
void init_credits_screen(void);
void init_options_screen(void);

// Main Menu Renders
void render_ui_button(Button button);
void render_how_to_play_screen(void);
void render_main_menu(void);
void render_level_select_buttons(void);
void render_pause_screen(void);
void render_credits_screen(void);
void render_options_screen(void);

// Building / Wave Phase
void render_wave_timer(void);
void reduce_building_phase_time(void);
void init_skip_wave_button(void);

// Logo Splash
void init_game_font(void);
void init_splash_logos(void);
void render_logos(void);

// Win / Lose
void game_win_lose_check(void);
void init_end_screen(void);
void render_end_screen(void);

// UI Movement
float linear(float start, float end, float value);
Button ui_button_movement(Button button, float destPosX, float destPosY);
int button_has_finished_moving(Button button, float destPosX, float destPosY);

void move_level_select(void);
int level_select_finished_moving(void);

void move_main_menu(void);
int main_menu_finished_moving(void);

void move_credits_screen(void);

void init_next_level(int nextGameLevel);
void exit_to_desktop(void);

Button init_text_button(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[]);
