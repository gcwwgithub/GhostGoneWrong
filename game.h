#include "cprocessing.h"
#pragma once
#include <stdio.h>

//True, False
#define TRUE 1
#define FALSE 0

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
	TurretButton0 = 0,
	TurretButton1 = 1,
	TurretButton2 = 2,
	TurretButton3 = 3,
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
