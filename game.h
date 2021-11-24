#include "cprocessing.h"
#include <stdio.h>
#include "vector.h"

#ifndef GOSTGONEWRONG_CURRENTHEADERFILES_GAME_H
#define GOSTGONEWRONG_CURRENTHEADERFILES_GAME_H

//Variables used to initialize others game.h variables.
enum PathType {
	kClear,
	kBlocked,
	kSpawn,
	kExit,
	kPath
};
//Used for pathfinding
struct Grids {
	int cost;
	int parent_row;
	int parent_col;
	int visited;
	enum PathType type;
};
enum Environmentaleffects {
	kNoEnvironmentalEffects,
	kIncreasedPhantomQuartz,
	kDecreasedPhantomQuartz,
	kFasterEnemies,
	kSlowerEnemies,
	kIncreasedTurretDamage,
	kDecreasedTurretDamage,
	kMoreHP,
	kLessHP,
	kIncreasedTurretAttackSpeed,
	kDecreasedTurretAttackSpeed,
	kNoPhantomQuartz
};
struct PowerUps {
	int more_phantom_quartz;
	int reduce_enemy_speed;
	int reduce_enemy_health;
	int increased_mine_damage;
};

//Common Tools
//Used for collision Detection
enum {
	kObjectCircle,
	kObjectRectangle
};
typedef struct Coordinates {
	float width; //Width and Height are the same for Circles
	float height; //Width and Height are the same for Circles
	float x_origin;
	float y_origin;
	int object_type;
	CP_Image image;
}Coordinates;
//Common Functions
int CollisionDetection(Coordinates object1, Coordinates object2);
int BtnIsPressed(Coordinates object1);
void MouseReset(void);//Set last click position to out of screen
//Set the current building time of building phase
void SetBuildingTime(float newBuildingTime);
int CheckGameButtonPressed(void);//Check which button is pressed in menu
//Boolean
typedef enum Boolean {
	kFalse,
	kTrue
}Boolean;

//Common Variables
//GameStates
enum
{
	kLogoSplash,
	kMainMenu,
	kPause,
	kBuilding,
	kWave,
	kWin,
	kLose,
	kLevelSelect,
	kCredits
}current_game_state;
//Game Grid
int level_grid_cols;
int level_grid_rows;
struct {
	float width;
	float height;
	float x_origin;
	float y_origin;
	float grid_width;
	float grid_height;
}game;
//Enemies
enum {
	kMaxEnemies = 60
};
typedef enum EnemyTypes {
	kBasic,
	kFastGhost,
	kFatGhost,
	kGrimReaper,
	kMaxEnemyType
} EnemyTypes;
typedef enum EnemyState {
	kEnemyInactive,
	kEnemyMoving,
	kEnemyHurt,
	kEnemyDeath,
	kEnemyReached,
	kEnemyAdjusting
}EnemyState;
//Ability tracker of the Grim Reaper enemy
typedef enum AbilityCharge {
	kUsed,
	kCharges1,
	kCharges2
}AbilityCharge;
typedef struct Enemy {
	int current_way_point;
	int alpha;
	int points;
	float x_origin; 
	float y_origin; 
	float enemy_width;
	float enemy_height;
	float health;
	float max_health;
	float angle;
	float speed;
	float slow_amt;
	float slow_timer;
	Coordinates data;
	EnemyState state;
	EnemyTypes type;
	CP_Image render_enemy;
	AbilityCharge charges;
	enum EnvironmentEffectEnemy env_eff;
	int enemy_pow_up[3];//Update to number of power ups
	int is_token;
	int wave_pow_up_is_active;
	int current_aim_state;
	float enemy_path_x[50];
	float enemy_path_y[50];
	int path_points;
	float slowed_distance;
	int adjusting_waypoint;
	float movement_timer;
	float timer;
}Enemy;
Enemy enemy[kMaxEnemies];
//Turret
typedef struct Modifiers
{
	int tracked_index;
	float damage;
	float speed;
	float range;
	float shoot_rate;
	float slow_amt;
	float slow_timer;
} Modifiers;
typedef enum TriangleAnimState
{
	kTurretInactive,
	kTurretShooting,
	kTurretActive
} TriangleAnimState;//Turret current action
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
	TriangleAnimState current_aim_state;
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
	int waveEnemies[MAX_NUMBER_OF_WAVES][kMaxEnemyType];
	enum EnvironmentalEffects currentEffect;
	struct PowerUps currentPowerUpLevel;
}LevelData;

LevelData Level[MAX_NUMBER_OF_LEVEL];
int currentGameLevel;
struct PowerUps powerUpPrice;
int enemiesLeft;
float buildingTime;

//Input
Coordinates MouseInput;

//Graphics

//Game Menus
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
	TurretDetailsDisplay, //Rendered when mouse is hovered on button
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

#endif // !GOSTGONEWRONG_CURRENTHEADERFILES_GAME_H