#include "cprocessing.h"
#pragma once

//Game Grid
#define GAME_GRID_COLS 3
#define GAME_GRID_ROWS 7

//Color
#define COLOR_BLACK  CP_Color_Create(0, 0, 0, 255)
#define COLOR_WHITE CP_Color_Create(255, 255, 255, 255)
#define COLOR_RED CP_Color_Create(255, 0, 0, 255)
#define COLOR_BLUE CP_Color_Create(0, 0, 255, 255)
#define COLOR_GREY CP_Color_Create(128, 128, 128, 255)

//Font
#define GAME_FONT CP_Font_Load("Assets/VT323-Regular.ttf")

//Game State
enum GameState
{
	MainMenu,
	Pause,
	Building,
	Wave,
	Win,
	Lose,
	LevelSelect,
	Credits
};

//Level
typedef enum GridState {
	Grid_Color_White = 0,
	Grid_Color_Grey = 1,
	Grid_Color_Red = 2,
	Grid_Color_Blue = 3
}GridState;

typedef struct LevelData {
	int spawnRow;
	int spawnCol;
	int exitRow;
	int exitCol;
	GridState gridColor[GAME_GRID_ROWS][GAME_GRID_COLS];
}LevelData;

//Common Variable
float  Game_Grid_Width, Game_Grid_Height, Game_Width, Game_Height, GAME_X_ORIGIN, GAME_Y_ORIGIN;
enum GameState currentGameState;
LevelData Tutorial;