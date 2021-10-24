#pragma once
#include "game.h"

// scaled to factor for different resolutions to an extent
#define BUTTON_WIDTH CP_System_GetWindowWidth() / 400 * 55.0f
#define BUTTON_HEIGHT CP_System_GetWindowHeight() / 400 * 35.0f
#define FONT_SIZE CP_System_GetWindowWidth() / 400 * 18.0f



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



void init_text_button(Button button, float buttonPosX, float buttonPosY, float buttonWidth, float buttonHeight, float textPosX, float textPosY, char string[]);

void render_game_title(void);
void init_play_button(void);
void init_quit_button(void);
void init_back_button(void);

void init_pause_screen(void);

void init_level_select_buttons(void);

void render_ui_button(Button button);
void render_level_select_buttons(void);
void render_pause_screen(void);

int withinBoundaries(float minX, float minY, float maxX, float maxY);
void detect_grid_square_color(LevelData level);

void exit_game(void);