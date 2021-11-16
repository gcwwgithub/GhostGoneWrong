#pragma once
#include "game.h"
#include "Gabriel.h"

// scaled for different resolutions
// hardcoded resolution
#define BUTTON_WIDTH CP_System_GetWindowWidth() / 400 * 55.0f
#define BUTTON_HEIGHT CP_System_GetWindowHeight() / 400 * 35.0f
#define FONT_SIZE CP_System_GetWindowWidth() / 400 * 15.0f

#define BUILDING_PHASE_TIME 3.0f

typedef struct Button {
	Coordinates buttonData;
	float textPositionX;
	float textPositionY;
	int interpolationTime; // time to move from one position to another
	char textString[16];
}Button;

Button PlayButton;
Button QuitButton;
Button HowToPlayButton;

Button BackButton;
Button levelButtons[5];

Button PauseQuitButton;
Button PauseBackButton;

Button SkipWaveButton;

Button EndScreenButtons[3];


int enemiesLeft;
int enemiesInLevel;
int basicEnemyNum;
int fastEnemyNum;
int fatEnemyNum;

int score;

float buildingTime;

// Main Menu Inits
void render_title_screen(void);
void init_play_button(void);
void init_quit_button(void);
void init_back_button(void);
void init_how_to_play_button(void);
void init_how_to_play_screen(void);
void init_level_select_buttons(void);

// Main Menu Renders
void render_ui_button(Button button);
void render_how_to_play_screen(void);
void render_level_select_buttons(void);
void render_pause_screen(void);

void render_wave_timer(void);
void reduce_building_phase_time(void);
void set_building_time(float newBuildingTime);
void init_skip_wave_button(void);

void init_game_font(void);
void init_pause_screen(void);

void game_win_lose_check(void);
void init_end_screen(void);
void render_end_screen(void);

void ui_button_movement(Coordinates buttonCoord, float destPosX, float destPosY, float time);

void init_level(int gameLevelToRestart);
void display_enemies_left(void);
void exit_to_desktop(void);

//void init_text_button(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[]);
//void gold_to_phantom_quartz_conversion(int goldAmtToConvert, int conversionRate);
