#include "cprocessing.h"
#include <stdio.h>
#include "vector.h"

#ifndef GOSTGONEWRONG_CURRENTHEADERFILES_GAME_H
#define GOSTGONEWRONG_CURRENTHEADERFILES_GAME_H

typedef enum Boolean {
	kFalse,
	kTrue
}Boolean;
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
//defining max enemies, turret and projectile
enum {
	kMaxEnemies = 60,
	kMaxTurret = 100,
	kMaxProjectile = 100
};

//Common Tools
//Used for collision Detection
enum {
	kObjectCircle,
	kObjectRectangle
};
//Used for pathfinding
enum PathType {
	kClear,
	kBlocked,
	kSpawn,
	kExit,
	kPath
};
struct Grids {
	int cost;
	int parent_row;
	int parent_col;
	Boolean visited;
	enum PathType type;
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
Boolean CollisionDetection(Coordinates object1, Coordinates object2);
Boolean BtnIsPressed(Coordinates object1);
void MouseReset(void);//Set last click position to out of screen
//Set the current building time of building phase
void SetBuildingTime(float newBuildingTime);
int CheckGameButtonPressed(void);//Check which button is pressed in menu
float FloatAbs(float x);
void Enemies_init(void);
void exit_to_desktop(void);
//upgrade system (for now using index will change depending how to access this easier)
void upgrade_turret(int t_index);
//init turret and projectiles
void turret_init(void);
//sell them turrets
void sell_turret(int t_index);
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
	kCredits,
	kHowToPlay,
	kOptions
}current_game_state;
//Game Grid
int level_grid_cols;
int level_grid_rows;
int current_how_to_play_page;
struct {
	float width;
	float height;
	float x_origin;
	float y_origin;
	float grid_width;
	float grid_height;
}game;
//Enemies
typedef enum EnemyTypes {
	kBasic,
	kFastGhost,
	kFatGhost,
	kGrimReaper,
	kMaxEnemyType
}EnemyTypes;
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
typedef enum TurretCurrentEnvironmentEffect {
	kTEnvironmentNoEffect,
	kTEnvironmentIncreasedDamage,
	kTEnvironmentIncreasedAttackSpeed,
	kTEnvironmentDecreasedDamage,
	kTEnvironmnetDecreasedAttackSpeed
}TurretCurrentEnvironmentEffect;
//enum of turret types
typedef enum TurretType
{
	kTBasic,
	kTSlow,
	kTHoming,
	kTMine,
	kTMax
}TurretType;
typedef enum TurretPurchase
{
	kTPPrice,
	kTPUpgradePrice,
	kTPUpgradeMaxLevel,
	kTPMax
}TurretPurchase;
typedef struct Turret
{
	int is_active;
	float size;
	float angle;
	//flaot range, cooldown, damage;
	//float t_slow_amt, t_slow_timer; //for slow turret
	Modifiers mod;
	Vector2 dir;
	Coordinates data;
	TurretType type;
	TriangleAnimState current_aim_state;
	float turret_anim_timer;
	CP_Image turret_img;
	int anim_counter;
	// sell price, upgrade price , 
	// total accumulated price (each upgrades + base price) 
	int sell_price;
	int upgrade_price;
	int total_price;
	int level;
	//For Battlefield effects
	TurretCurrentEnvironmentEffect env_effects;
} Turret;
Turret turret[kMaxTurret];
int turret_purchasing[kTPMax][kTMax];
int** turret_on_grid;
int turret_selected_to_upgrade; //Use the turret index of the turret selected
//Used when no turret is selected
enum {
	kNoTurretSelected = -1
};
/*
	use this to place turret/spawn turret (pass in type and the grid index)
	E.g to place on grid[0][1] pass in type, 0, 1
*/
void place_turret(TurretType type, int index_x, int index_y);
//Projectiles
//enum of projectile types
typedef enum ProjectileType
{
	kPBasic,
	kPSlow,
	kPHoming,
	kPMine,
} ProjectileType;
typedef struct Projectile
{
	int is_active;
	float size;
	float lifetime;
	//float damage, speed, ;
	//float p_slow_amt, p_slow_timer; //for slow projectile
	Modifiers mod;
	Coordinates data;
	Vector2 dir;
	ProjectileType type;
} Projectile;
Projectile proj[kMaxProjectile];
//deteremine the collisoin effect base on type of proj
void col_type_projectile(Projectile* p);
//Level
//defination for levels
enum {
	kMaxNumberOfLevel = 5,
	kMaxNumberOfWave = 10,
	kFullBuildingPhaseTime = 30
};
typedef struct LevelData {
	int spawn_row;
	int spawn_col;
	int exit_row;
	int exit_col;
	struct Grids** grid;
	int phantom_quartz;
	int gold_quartz;
	int health;
	int current_game_level;
	int current_wave;
	int wave_enemies[kMaxNumberOfWave][kMaxEnemyType];
	enum EnvironmentalEffects current_effect;
	struct PowerUps current_power_up_level;
}LevelData;
LevelData Level;
struct PowerUps power_up_price; //Price of power ups
int enemies_left;// enemies remaining in the current round
float building_time;//Remaining building time
//Input
Coordinates mouse_input;

//Graphics
//Game Menus
enum MenuObjectType {
	kPauseButton,
	kTurretButtonBasic,
	kTurretButtonSlow,
	kTurretButtonHoming,
	kTurretButtonMine,
	kSwapButton,
	kPhantomQuartzMenu,
	kGoldQuartzMenu,
	kHealthMenu,
	kWaveDisplay,
	kEnvironmentalEffects,
	kMonsterRemainingDisplay,
	kUpgradeButton,
	kSellButton,
	kTurretDetailsDisplay, //Rendered when mouse is hovered on button
	kUpgradeMenu,// Rendered seperately from the render loop
	kGameGrid,//Not included in the render loop
	kButtonMax
};
//Array of all menu object used for rendering
Coordinates game_menu_object[kButtonMax];
float scaling_factor;//scaling to scale with different display size

//Color
#define COLOR_BLACK  CP_Color_Create(0, 0, 0, 255)
#define COLOR_WHITE CP_Color_Create(255, 255, 255, 255)
#define COLOR_RED CP_Color_Create(255, 0, 0, 255)
#define COLOR_GREEN CP_Color_Create(0, 255, 0, 255)

//Font
#define GAME_FONT CP_Font_Load("Assets/VT323-Regular.ttf")

//Sound Effects
CP_Sound ShootSFX;
CP_Sound SpawnxExitSFX;
CP_Sound ButtonClickSFX;
CP_Sound TurretPlaceSFX;
CP_Sound MineExplosionSFX;
CP_Sound WinSFX;
CP_Sound LoseSFX;
CP_Sound HitSFX;
float SFX_Volume;

CP_Sound MainMenuBGM;
CP_Sound BuildingBGM;
CP_Sound WaveBGM;
float BGM_Volume;
CP_Sound MainMenuMusic;

#endif // !GOSTGONEWRONG_CURRENTHEADERFILES_GAME_H