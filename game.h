#include "cprocessing.h"
#pragma once

//Grid
#define Grid_Cols 3
#define Grid_Rows 7
//#define X_ORIGIN 480.0f
#define Y_ORIGIN 270.0f

//Color
#define color_Black  CP_Color_Create(0, 0, 0, 255)
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

//Common Variable
float  gridWidth, gridHeight, gameWidth, gameHeight, X_ORIGIN;
enum GameState currentGameState;