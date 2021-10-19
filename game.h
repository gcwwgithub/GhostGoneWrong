#include "cprocessing.h"
#pragma once

//Game Grid
#define GAME_GRID_COLS 3
#define GAME_GRID_ROWS 7
float  Game_Grid_Width, Game_Grid_Height, Game_Width, Game_Height, GAME_X_ORIGIN, GAME_Y_ORIGIN;

//Color
#define COLOR_BLACK  CP_Color_Create(0, 0, 0, 255)
#define COLOR_WHITE CP_Color_Create(255, 255, 255, 255)
#define COLOR_RED CP_Color_Create(255, 0, 0, 255)
#define COLOR_BLUE CP_Color_Create(0, 0, 255, 255)
#define COLOR_GREY CP_Color_Create(128, 128, 128, 255)

//Font
#define GAME_FONT CP_Font_Load("Assets/VT323-Regular.ttf")

//Game State
typedef enum GameState
{
	MainMenu,
	Pause,
	Building,
	Wave,
	Win,
	Lose,
	LevelSelect,
	Credits
}GameState;

GameState currentGameState;

//Level
typedef enum GridState {
	Grid_Color_White,
	Grid_Color_Grey,
	Grid_Color_Red,
	Grid_Color_Blue
}GridState;

typedef struct LevelData {
	int spawnRow;
	int spawnCol;
	int exitRow;
	int exitCol;
	GridState gridColor[GAME_GRID_ROWS][GAME_GRID_COLS];
}LevelData;

LevelData Tutorial;

//Common Tools
typedef enum ObjectShape {
	objectCircle,
	objectRectangle
}ObjectShape;

typedef struct ObjectData {
	float objectPositionX;
	float objectPositionY;
	float rectLengthX;
	float rectLengthY;
	float circleRadius;
	ObjectShape objectType;
}ObjectData;

ObjectData MouseInput;
