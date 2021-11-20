#pragma once
#include "game.h"
#include "Gabriel.h"

// scaled for different resolutions
// hardcoded resolution
#define BUTTON_WIDTH CP_System_GetWindowWidth() / 400 * 55.0f
#define BUTTON_HEIGHT CP_System_GetWindowHeight() / 400 * 35.0f
#define FONT_SIZE CP_System_GetWindowWidth() / 400 * 15.0f
#define MOVE_DURATION 5.0f
#define BUILDING_PHASE_TIME 3.0f

typedef struct Button {
	Coordinates buttonData;
	float textPositionX;
	float textPositionY;
	float interpolationTime; // time to move from one position to another

	int isMoving;

	char textString[16];
}Button;

float movingDuration; // how fast do the buttons go away after clicking?

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

void init_main_menu(void);

void init_how_to_play_button(void);
void init_how_to_play_screen(void);
void init_level_select_buttons(void);

// Main Menu Renders
void render_ui_button(Button button);
void render_how_to_play_screen(void);
void render_start_menu(void);
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


float linear(float start, float end, float value);

Button ui_button_movement(Button button, float destPosX, float destPosY);
void move_level_select(void);
int level_select_finished_moving(void);
int button_has_finished_moving(Button button, float destPosX, float destPosY);

void init_next_level(int nextGameLevel);
void exit_to_desktop(void);

Button init_text_button(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[]);
//void gold_to_phantom_quartz_conversion(int goldAmtToConvert, int conversionRate);
