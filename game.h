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
#define MAX_NUMBER_OF_LEVEL 5
#define MAX_NUMBER_OF_WAVES 10
#define MAX_ENEMY_TYPE 4
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

typedef enum Environmentaleffects {
	IncreasedPhantomQuartz,
	DecreasedPhantomQuartz,
	FasterEnemies,
	SlowerEnemies,
	IncreasedTurretDamage,
	DecreasedTurretDamage,
	MoreHP,
	LessHP,
	IncreasedTurretAttackSpeed,
	DecreasedTurretAttackSpeed,
	NoPhantomQuartz

}EnvironmentalEffects;

typedef struct LevelData {
	int spawnRow;
	int spawnCol;
	int exitRow;
	int exitCol;
	Grids grid[GAME_GRID_ROWS][GAME_GRID_COLS];
	int phantomQuartz;
	int goldQuartz;
	int health;
	int currentWave;
	int waveEnemies[MAX_NUMBER_OF_WAVES][MAX_ENEMY_TYPE];
	EnvironmentalEffects currentEffect;

}LevelData;

LevelData Level[MAX_NUMBER_OF_LEVEL];

int currentGameLevel;

//Objects
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
	CP_Image image;
}Coordinates;

//TurretMenu
Coordinates TurretMenu;

//Buttons
enum MenuObjectType {
	PauseButton,
	TurretButtonBasic,
	TurretButtonSlow,
	TurretButtonHoming,
	TurretButtonMine,
	SwapButton,
	PhantomQuartzMenu,
	GoldQuartzMenu,
	HealthMenu,
	WaveDisplay,
	BattlefieldEffects,
	MonsterRemainingDisplay,
	UpgradeButton,
	SellButton,
	GameGrid,//needs to be second last variable
	NoButton
};

#define NUMBER_OF_MENU_OBJECTS NoButton

int check_game_button_pressed(void);

Coordinates GameMenuObject[NUMBER_OF_MENU_OBJECTS];
Coordinates MouseInput;

//Environment
#define MAX_ENVIRONMENT_OBJECT 1
Coordinates Environment[MAX_ENVIRONMENT_OBJECT];

//Common Tools
int Collision_Detection(Coordinates object1, Coordinates object2);
int btn_is_pressed(Coordinates object1);
void render_turret_menu_object(Coordinates ButtonX, enum MenuObjectType type);

//graphics

float scalingFactor;
