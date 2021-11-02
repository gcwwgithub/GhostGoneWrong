#pragma once
#include "game.h"
#include "Gabriel.h"

// scaled to factor for different resolutions to an extent
// admittedly hardcoded the anchored resolution
#define BUTTON_WIDTH CP_System_GetWindowWidth() / 400 * 55.0f
#define BUTTON_HEIGHT CP_System_GetWindowHeight() / 400 * 35.0f
#define FONT_SIZE CP_System_GetWindowWidth() / 400 * 15.0f

#define BUILDING_PHASE_TIME 3.0f
#define TIME_LEFT 0.0f

#define PORTAL_HEALTH 10


typedef struct Button {
	Coordinates buttonData;
	float textPositionX;
	float textPositionY;
	char textString[50];

}Button;

Button PlayButton;
Button QuitButton;
Button BackButton;
Button levelButtons[5];

Button PauseQuitButton;
Button PauseBackButton;

int enemiesLeft;
int basicEnemyNum;
int fastEnemyNum;
int fatEnemyNum;

void init_text_button(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[]);

void render_title_screen(void);
void init_play_button(void);
void init_quit_button(void);
void init_back_button(void);

void init_pause_screen(void);

void init_level_select_buttons(void);

void render_ui_button(Button button);
void render_level_select_buttons(void);
void render_pause_screen(void);
void render_wave_timer_text(void);

void detect_grid_square_color(LevelData level);
void reduce_building_phase_time();
void init_game_font(void);

void restart_level(void);
void display_enemies_left(void);
void exit_game(void);