#include "cprocessing.h"
#include <stdio.h>
#include "vector.h"

#ifndef GAME_HEADER
#define GAME_HEADER

//Variables used to initialize others game.h variables. These are usable only in game.h
static enum PathType {
	Clear,
	Blocked,
	Spawn,
	Exit,
	Path
};
static struct Grids {
	int cost;
	int parentRow;
	int parentCol;
	int visited;
	enum PathType type;
};
static enum Environmentaleffects {
	NoEnvironmentalEffects,
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
};
static struct PowerUps {
	int morePhantomQuartz;
	int reduceEnemySpeed;
	int reduceEnemyHealth;
	int increasedMineDamage;
};

//Common Tools

//enum object type
enum {
	objectCircle,
	objectRectangle
};
typedef struct Coordinates {
	float width; //Width and Height are the same for Circles
	float height; //Width and Height are the same for Circles
	float xOrigin;
	float yOrigin;
	int objectType;
	CP_Image image;
}Coordinates;

//Common Functions
int Collision_Detection(Coordinates object1, Coordinates object2);
int btn_is_pressed(Coordinates object1);
void mouse_reset(void);

//Boolean
#define TRUE 1
#define FALSE 0

//Common Variables

//GameStates
enum
{
	LogoSplash,
	MainMenu,
	Pause,
	Building,
	Wave,
	Win,
	Lose,
	LevelSelect,
	Credits
}currentGameState;

//Game Grid
int gameGridCols;
int gameGridRows;
struct {
	float width;
	float height;
	float xOrigin;
	float yOrigin;
	float gridWidth;
	float gridHeight;
}Game;

//Enemies
#define MAX_ENEMIES 60
typedef enum EnemyTypes {
	Basic,
	Fast_Ghost,
	Fat_Ghost,
	grimReaper,
	Max_Enemy_Type
} EnemyTypes;

typedef enum EnemyState {
	Inactive,
	Moving,
	Hurt,
	Death,
	Reached,
	Adjusting

}EnemyState;

typedef enum AbilityUsed {
	Used,
	charges_1,
	charges_2
}Ability_charge;

typedef struct Enemy {
	int CurrentWaypoint, alpha, points;
	float xOrigin, yOrigin, enemy_width, enemy_height,
		health, max_health, angle, speed, slow_amt, slow_timer;
	Coordinates data;
	EnemyState state;
	EnemyTypes type;
	CP_Image Render_Enemy;
	Ability_charge charges;
	enum EnvironmentEffectEnemy env_eff;
	int Enemy_pow_up[3];//Update to number of power ups

	int isToken;

	int WavePowUp_isActive;
	int currentAnimState;

	float EnemyPathX[50];
	float EnemyPathY[50];
	int pathPoints;
	float slowed_distance;

	int adjustingWaypoint;

	float movement_timer;
	float timer;
}enemy;
enemy Enemy[MAX_ENEMIES];

//Turret
typedef struct Modifiers
{
	int tracked_index;
	float damage, speed, range, shoot_rate,
		slow_amt, slow_timer;
} Modifiers;

typedef enum TriangleAnimState
{
	INACTIVE,
	SHOOTING,
	ACTIVE
} TriangleAnimState;

//For battlefield effects
typedef enum Turret_Env_effects {
	No_Effect,
	Increased_damage,
	Increased_attack_speed,
	Decreased_damage,
	Decreased_attack_speed
}Turret_Env_effects;
//enum of turret types
typedef enum TurretType
{
	T_BASIC,
	T_SLOW,
	T_HOMING,
	T_MINE,
	T_WALL,
	T_MAX
} TurretType;
typedef enum TurretPurchase
{
	TP_PRICE,
	TP_UPGRADE_PRICE,
	TP_UPGRADE_MAX_LEVEL,
	TP_MAX
}TurretPurchase;
typedef struct Turret
{
	int isActive;
	float size, angle;
	//flaot range, cooldown, damage;
	//float t_slow_amt, t_slow_timer; //for slow turret
	Modifiers mod;
	Vector2 dir;
	Coordinates data;
	TurretType type;
	TriangleAnimState currentAnimState;
	float turretAnimTimer;
	CP_Image turret_img;
	int animCounter;
	// sell price, upgrade price , 
	// total accumulated price (each upgrades + base price) 
	int sell_price, upgrade_price, total_price;
	int level;

	//For Battlefield effects
	Turret_Env_effects Env_effect;
} Turret;
#define MAX_TURRET 100
Turret turret[MAX_TURRET];
int turret_purchasing[TP_MAX][T_MAX];
int** turret_on_grid;

#define NO_TURRET_SELECTED -1
int turretSelectedToUpgrade; //Use the turret index of the turret selected

//Projectiles
#define MAX_PROJECTILE 100
//enum of projectile types
typedef enum ProjectileType
{
	P_BASIC,
	P_SLOW,
	P_HOMING,
	P_MINE,
} ProjectileType;

typedef struct Projectile
{
	int isActive;
	float size, lifetime;
	//float damage, speed, ;
	//float p_slow_amt, p_slow_timer; //for slow projectile
	Modifiers mod;
	Coordinates data;
	Vector2 dir;
	ProjectileType type;
} Projectile;

Projectile proj[MAX_PROJECTILE];

//Level
#define MAX_NUMBER_OF_LEVEL 5
#define MAX_NUMBER_OF_WAVES 10
#define BUILDING_PHASE_TIME 30.0f
typedef struct LevelData {
	int spawnRow;
	int spawnCol;
	int exitRow;
	int exitCol;
	struct Grids** grid;
	int phantomQuartz;
	int goldQuartz;
	int health;
	int currentWave;
	int waveEnemies[MAX_NUMBER_OF_WAVES][Max_Enemy_Type];
	enum EnvironmentalEffects currentEffect;
	struct PowerUps currentPowerUpLevel;
}LevelData;

LevelData Level[MAX_NUMBER_OF_LEVEL];
int currentGameLevel;
struct PowerUps powerUpPrice;
int enemiesLeft;
float buildingTime;

//Environment
#define MAX_ENVIRONMENT_OBJECT 10
Coordinates Environment[MAX_ENVIRONMENT_OBJECT];

//Input
Coordinates MouseInput;

//Graphics

//Game Menus
#define LEFT_MENU_X_END 140.714294f
#define RIGHT_MENU_X_START 998.571411f
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
	UpgradeMenu,// Rendered seperately from the render loop
	GameGrid,//Not included in the render loop
	ButtonMax
};
Coordinates GameMenuObject[ButtonMax];
float scalingFactor;

//Color
#define COLOR_BLACK  CP_Color_Create(0, 0, 0, 255)
#define COLOR_WHITE CP_Color_Create(255, 255, 255, 255)
#define COLOR_RED CP_Color_Create(255, 0, 0, 255)
#define COLOR_GREEN CP_Color_Create(0, 255, 0, 255)

//Font
#define GAME_FONT CP_Font_Load("Assets/VT323-Regular.ttf")

#endif // !GAMEHEADER