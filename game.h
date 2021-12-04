/*!
@file       zhengwei.c
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
			Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
			Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Phua Tai Dah Anderson (a.phua@digipen.edu)
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author	
@course     CSD 1400
@section    C
@date       01/12/2021
@brief    	Contains function and variables that are commmonly used by everyone
*//*__________________________________________________________________________*/
#include "cprocessing.h"
#include "vector.h"

// FOR MEM LEAK CHECK
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
// END OF MEM LEAK

#ifndef GOSTGONEWRONG_CURRENTHEADERFILES_GAME_H
#define GOSTGONEWRONG_CURRENTHEADERFILES_GAME_H
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		True False for boolean checks.
*//*_____________________________________________________________*/
typedef enum Boolean {
	kFalse,
	kTrue
}Boolean;
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		enum to check which environmental effect is currently
			active.
*//*_____________________________________________________________*/
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
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		struct used to store price and used in level struct
			to store power up level
*//*_____________________________________________________________*/
struct PowerUps {
	int more_phantom_quartz;
	int reduce_enemy_speed;
	int reduce_enemy_health;
	int increased_mine_damage;
};
/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Defining max enemies, turrets and projectiles
*//*_____________________________________________________________*/
enum {
	kMaxEnemies = 60,
	kMaxTurret = 100,
	kMaxProjectile = 100
};
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		Used for collision detection calculation
*//*_____________________________________________________________*/
enum {
	kObjectCircle,
	kObjectRectangle
};
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		Define the game grid state for use in pathfinding
*//*_____________________________________________________________*/
enum PathType {
	kClear,
	kBlocked,
	kSpawn,
	kExit,
	kPath
};
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		Struct to contain the dgame grid state used for
			pathfinding
*//*_____________________________________________________________*/
struct Grids {
	int cost;
	int parent_row;
	int parent_col;
	Boolean visited;
	enum PathType type;
};
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		Struct to contain information of location and size of
			an object used for both displays and collision
			detection
*//*_____________________________________________________________*/
typedef struct Coordinates {
	float width; //Width and Height are the same for Circles
	float height; //Width and Height are the same for Circles
	float x_origin;
	float y_origin;
	int object_type;
	CP_Image image;
}Coordinates;
/*
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author	Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
			Phua Tai Dah Anderson (a.phua@digipen.edu)
			Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@brief		Initialize variables and call functions to
			initialize level 1.
@param		- void
@return		- void
*//*_____________________________________________________________*/
void InitLevel1(void);
/*
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author	Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
			Phua Tai Dah Anderson (a.phua@digipen.edu)
			Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@brief		Initialize variables and call functions to
			initialize level 2.
@param		- void
@return		- void
*//*_____________________________________________________________*/
void InitLevel2(void);
/*
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author	Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
			Phua Tai Dah Anderson (a.phua@digipen.edu)
			Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@brief		Initialize variables and call functions to
			initialize level 3.
@param		- void
@return		- void
*//*_____________________________________________________________*/
void InitLevel3(void);
/*
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author	Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
			Phua Tai Dah Anderson (a.phua@digipen.edu)
			Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@brief		Initialize variables and call functions to
			initialize level 4.
@param		- void
@return		- void
*//*_____________________________________________________________*/
void InitLevel4(void);
/*
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author	Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
			Phua Tai Dah Anderson (a.phua@digipen.edu)
			Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@brief		Initialize variables and call functions to
			initialize level 5.
@param		- void
@return		- void
*//*_____________________________________________________________*/
void InitLevel5(void);
/*!
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author
@brief		This functions checks for collision between two square,
			two circle, or a square and circle. It returns kTrue,
			if the two given object is colliding, else it returns
			kFalse.
@param		Coordinates object1 - The first object to check.
			Coordinates object2 - The second object to check.
@return		Boolean - kTrue or kFalse.
*//*_____________________________________________________________*/
Boolean CollisionDetection(Coordinates object1, Coordinates object2);
/*!
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author
@brief		This functions return kTrue if given button is the last
			clicked object. Else return kFalse.
@param		Coordinates object - The button to check.
@return		Boolean - kTrue or kFalse.
*//*_____________________________________________________________*/
Boolean BtnIsPressed(Coordinates object1);
/*!
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author
@brief		This function sets the location of the previous click
			to be outside of the screen so that it does not click
			on any object in the game.
@param		- void
@return		- void
*//*_____________________________________________________________*/
void MouseReset(void);
/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@co-author
@brief		Set the current building tume of building phase
@param		float newBuildingTime -  The building time to set to
@return		- void
*//*_____________________________________________________________*/
void SetBuildingTime(float newBuildingTime);
/*!
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author
@brief		This function returns the positive vale of a float.
@param		float x- The float to obtain the positive value .
@return		float - The positive value of the given float.
*//*_____________________________________________________________*/
float FloatAbs(float x);
/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@co-author
@brief		Initialize the enemy data.
@param		- void
@return		- void
*//*_____________________________________________________________*/
void Enemies_init(void);
/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@co-author
@brief		exit the game.
@param		- void
@return		- void
*//*_____________________________________________________________*/
void ExitToDesktop(void);
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		function to upgrade turret
@param		- void
@return		- void
*//*_____________________________________________________________*/
void upgrade_turret(int t_index);
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		init turret and projectiles
@param		- void
@return		int - t_index - The index of the turret to upgrade
*//*_____________________________________________________________*/
void turret_init(void);
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		Remove the sold turret
@param		- void
@return		int - t_index - The index of the turret to sell
*//*_____________________________________________________________*/
void sell_turret(int t_index);
/*!
@author     Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
			Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Phua Tai Dah Anderson (a.phua@digipen.edu)
			Ng Zheng Wei (zhengwei.ng@digipen.edu)
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		enum for gamestate of the game
*//*_____________________________________________________________*/
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
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		The number of cols in the current level
*//*_____________________________________________________________*/
int level_grid_cols;
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		The number of rows in the current level
*//*_____________________________________________________________*/
int level_grid_rows;
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		The current page of how to play. 0 if how to play is
			not open
*//*_____________________________________________________________*/
int current_how_to_play_page;
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		Position and size of the game grid.
*//*_____________________________________________________________*/
struct {
	float width;
	float height;
	float x_origin;
	float y_origin;
	float grid_width;
	float grid_height;
}game;
//Enemies
/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@brief		enum for each type of enemies
*//*_____________________________________________________________*/
typedef enum EnemyTypes {
	kBasic,
	kFastGhost,
	kFatGhost,
	kGrimReaper,
	kMaxEnemyType
}EnemyTypes;
/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@brief		enum for the state of the enemies.
*//*_____________________________________________________________*/
typedef enum EnemyState {
	kEnemyInactive,
	kEnemyMoving,
	kEnemyHurt,
	kEnemyDeath,
	kEnemyReached,
	kEnemyAdjusting
}EnemyState;
/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@brief		enum to track ability of grim reaper.
*//*_____________________________________________________________*/
typedef enum AbilityCharge {
	kUsed,
	kCharges1,
	kCharges2
}AbilityCharge;
/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@brief		struct that contains the data of the enemies
*//*_____________________________________________________________*/
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
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		struct that contains a group of variable  for turrets
*//*_____________________________________________________________*/
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
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		enum for the state of the turret action
*//*_____________________________________________________________*/
typedef enum TriangleAnimState
{
	kTurretInactive,
	kTurretShooting,
	kTurretActive
} TriangleAnimState;
/*!
@author     Lim Jing Rui John (l.jingruijohn@digipen.edu)
@brief		enum for checking if battlefield effect is applied to
			a selected turret
*//*_____________________________________________________________*/
typedef enum TurretCurrentEnvironmentEffect {
	kTEnvironmentNoEffect,
	kTEnvironmentIncreasedDamage,
	kTEnvironmentIncreasedAttackSpeed,
	kTEnvironmentDecreasedDamage,
	kTEnvironmnetDecreasedAttackSpeed
}TurretCurrentEnvironmentEffect;
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		enum for the type of each turret.
*//*_____________________________________________________________*/
typedef enum TurretType
{
	kTBasic,
	kTSlow,
	kTHoming,
	kTMine,
	kTMax
}TurretType;
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		enum for the type of price to use for turret
*//*_____________________________________________________________*/
typedef enum TurretPurchase
{
	kTPPrice,
	kTPUpgradePrice,
	kTPUpgradeMaxLevel,
	kTPMax
}TurretPurchase;
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Struct that contains the data of turret.
*//*_____________________________________________________________*/
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
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Array to contains the data of all turrets
*//*_____________________________________________________________*/
Turret turret[kMaxTurret];
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		The price of upgrading or buying turrets
*//*_____________________________________________________________*/
int turret_purchasing[kTPMax][kTMax];
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		Stores the index of turret array for the given row
			and col.
*//*_____________________________________________________________*/
int** turret_on_grid;
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		The index of the current selected turret to use for
			upgrading.
*//*_____________________________________________________________*/
int turret_selected_to_upgrade;
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		To be used when no turret is selected
*//*_____________________________________________________________*/
enum {
	kNoTurretSelected = -1
};
/*
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		use this to place turret/spawn turret 
			(pass in type and the grid index)
			E.g to place on grid[0][1] pass in type, 0, 1.
@param		TurretType type - The type of turret to place.
			int index_x - x index of the location.
			int index_y - y index of the location.
@return		- void
*//*_____________________________________________________________*/
void place_turret(TurretType type, int index_x, int index_y);
//Projectiles
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		enum for type of projectile
*//*_____________________________________________________________*/
typedef enum ProjectileType
{
	kPBasic,
	kPSlow,
	kPHoming,
	kPMine,
} ProjectileType;
/*!
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@brief		struct to contain the data of the projectile.
*//*_____________________________________________________________*/
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
/*
@author     Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@co-author
@brief		deteremine the collision effect base on type of proj
@param		Projectile* p - The memory address of the projectile
			struct to check.
@return		- void
*//*_____________________________________________________________*/
void col_type_projectile(Projectile* p);
//Level
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
			Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		enum for max number of level wave and building time.
*//*_____________________________________________________________*/
enum {
	kMaxNumberOfLevel = 5,
	kMaxNumberOfWave = 10,
	kFullBuildingPhaseTime = 30
};
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		Struct for the data of the level
*//*_____________________________________________________________*/
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
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		data for level
*//*_____________________________________________________________*/
LevelData level;
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		Price of power ups
*//*_____________________________________________________________*/
struct PowerUps power_up_price;
/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		Enemies left in the wave.
*//*_____________________________________________________________*/
int enemies_left;
/*!
@author     Phua Tai Dah Anderson (a.phua@digipen.edu)
@brief		The remaining building time.
*//*_____________________________________________________________*/
float building_time;
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		Used to store the previous click location for 
			checking if button is pressed.
*//*_____________________________________________________________*/
Coordinates mouse_input;

//Graphics
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		enum for all buttons and dispay found in the level
*//*_____________________________________________________________*/
typedef enum {
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
}MenuObjectType;
/*!
@author     Ng Zheng Wei(zhengwei.ng@digipen.edu)
@co-author
@brief		This function checks which button is pressed
			in the game and returns MenuObjectType which is
			a enum to which menu object is clicked. It
			returns kFalse if no object is clicked.
@param		-void
@return		MenuObjectType - enum of which menu object is clicked.
*//*_____________________________________________________________*/
MenuObjectType CheckGameButtonPressed(void);
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		Array of all menu object used for rendering and
			checking which button is pressed.
*//*_____________________________________________________________*/
Coordinates game_menu_object[kButtonMax];
/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		scaling to scale with different display size.
*//*_____________________________________________________________*/
float scaling_factor;

/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		defination of colors.
*//*_____________________________________________________________*/
#define COLOR_BLACK  CP_Color_Create(0, 0, 0, 255)
#define COLOR_WHITE CP_Color_Create(255, 255, 255, 255)
#define COLOR_RED CP_Color_Create(255, 0, 0, 255)
#define COLOR_GREEN CP_Color_Create(0, 255, 0, 255)

/*!
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)
@brief		defination of game font.
*//*_____________________________________________________________*/
#define GAME_FONT CP_Font_Load("Assets/VT323-Regular.ttf")

/*!
@author    Lim Jing Rui John (l.jingruijohn@digipen.edu)
@brief		Decleration of sounds used in the game.
*//*_____________________________________________________________*/
CP_Sound shoot_sfx;
CP_Sound spawn_exit_sfx;
CP_Sound button_click_sfx;
CP_Sound turret_place_sfx;
CP_Sound mine_explosion_sfx;
CP_Sound win_sfx;
CP_Sound lose_sfx;
CP_Sound hit_sfx;
float sfx_volume;
CP_Sound main_menu_bgm;
CP_Sound building_bgm;
CP_Sound wave_bgm;
CP_Sound main_menu_music;
float bgm_volume;
#endif // !GOSTGONEWRONG_CURRENTHEADERFILES_GAME_H