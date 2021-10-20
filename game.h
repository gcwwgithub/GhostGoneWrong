#include "cprocessing.h"
#pragma once

//Game Grid
#define GAME_GRID_COLS 3
#define GAME_GRID_ROWS 7
typedef struct GameCoordinates {
	float width;
	float height;
	float xOrigin;
	float yOrigin;
	float gridWidth;
	float gridHeight;
}GameCoordinates;
GameCoordinates Game;;

//Color
#define COLOR_BLACK  CP_Color_Create(0, 0, 0, 255)
#define COLOR_WHITE CP_Color_Create(255, 255, 255, 255)
#define COLOR_RED CP_Color_Create(255, 0, 0, 255)
#define COLOR_BLUE CP_Color_Create(0, 0, 255, 255)
#define COLOR_GREY CP_Color_Create(128, 128, 128, 255)
#define COLOR_GREEN CP_Color_Create(0, 255, 0, 255)

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

void object_init(void);
int Collision_Detection(ObjectData object1, ObjectData object2);

//Turret Menu
typedef struct Coordinates {
	float width;
	float height;
	float xOrigin;
	float yOrigin;
}Coordinates;

Coordinates TurretMenu;
ObjectData ObjectTurretButton0;
Coordinates TurretButton0;
ObjectData ObjectTurretButton1;
Coordinates TurretButton1;
ObjectData ObjectTurretButton2;
Coordinates TurretButton2;
ObjectData ObjectTurretButton3;
Coordinates TurretButton3;