#include "cprocessing.h"
#pragma once
#include <stdio.h>

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
#define COLOR_PURPLE CP_Color_Create(255, 0, 255, 255)
#define COLOR_YELLOW CP_Color_Create(255,255, 0, 255)

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
typedef enum PathType {
	Clear = 0,
	Blocked = 1,
	Spawn = 2,
	Exit = 3,
	Path = 4
}PathType;

typedef struct Grids {
	int cost;
	int parentRow;
	int parentCol;
	int visited;
	PathType type;
}Grids;


typedef struct LevelData {
	int spawnRow;
	int spawnCol;
	int exitRow;
	int exitCol;
	Grids grid[GAME_GRID_ROWS][GAME_GRID_COLS];
}LevelData;

LevelData Tutorial;

//Objects
#define NUMBER_OF_BUTTONS 5

typedef enum ObjectShape {
	objectCircle,
	objectRectangle
}ObjectShape;


typedef struct Coordinates {
	float width; //Width and Height are the same for Circles
	float height; //Width and Height are the same for Circles
	float xOrigin;
	float yOrigin;
	ObjectShape objectType;
	CP_Image imageOfButton;
}Coordinates;

//TurretMenu
Coordinates TurretMenu;

//Buttons
enum ButtonType {
	TurretButtonTriangle = 0,
	TurretButtonCircle = 1,
	TurretButtonStar = 2,
	TurretButtonPercentage = 3,
	PauseButton = 4,
	NoButton = NUMBER_OF_BUTTONS
};

int check_game_button_pressed(void);

Coordinates GameButton[NUMBER_OF_BUTTONS];
Coordinates MouseInput;

//Common Tools
int Collision_Detection(Coordinates object1, Coordinates object2);
int btn_is_pressed(Coordinates object1);
void render_button(Coordinates ButtonX, CP_Color Color);
