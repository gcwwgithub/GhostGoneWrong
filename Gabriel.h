/*!
All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Gabriel.h
@author     Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@course     CSD 1400
@section    C
@date       28/11/2021
@brief    	Contains functions that are related to rendering, animation and effects
*//*__________________________________________________________________________*/

#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>


struct SpriteSheetImage //this is a struct to contain pixel values for CP_Draw Subimage
{
	int image_width,image_height;
	float left_pixel_value, right_pixel_value, top_pixel_value, bottom_pixel_value;
};

struct LinkedListNode //For linked list
{
	struct LinkedListNode* next_node;
	int key;
	float node_x_coordinate;
	float node_y_coordinate;
	int node_image_alpha;
	float node_timer;
	int node_counter;
};


struct PortalVariables //Each portal has these 2 variables
{
	float portal_x_coordinate;
	float portal_y_coordinate;
	float portal_image_width;
	float portal_image_height;
};
#pragma region The variables that contain all the image files
//All the images
CP_Image slow_turret_image_array[6];
CP_Image tutorial_image_array[7];

CP_Image pause_button_image;
CP_Image game_title_image;
CP_Image turret_button_background;
CP_Image thin_UI_background;
CP_Image upgrade_menu_background;

CP_Image basic_ghost_spritesheet;
CP_Image fast_ghost_spritesheet;
CP_Image fat_ghost_spritesheet;
CP_Image grim_reaper_spritesheet;
CP_Image blue_portal_spritesheet;
CP_Image red_portal_spritesheet;
CP_Image basic_turret_spritesheet;
CP_Image homing_missle_turret_spritesheet;
CP_Image mine_spritesheet;
CP_Image turret_bullet_spritesheet;
CP_Image turret_bullet_radius_spritesheet;
CP_Image currency_spritesheet;
CP_Image grid_environment_objects_spritesheet;
CP_Image background_spritesheet;
CP_Image portal_enter_effect_spritesheet;
CP_Image portal_spawn_effect_spritesheet;
CP_Image power_up_spritesheet;
CP_Image battlefield_effect_spritesheet;
CP_Image fat_UI_background_spritesheet;
CP_Image interactable_UI_buttons_spritesheet;
CP_Image turret_stats_spritesheet;
CP_Image non_grid_environment_objects_spritesheet;
#pragma endregion

#pragma region The arrays of SpriteSheetImage structs
struct SpriteSheetImage basic_ghost_spritesheet_array[3];
struct SpriteSheetImage fast_ghost_spritesheet_array[3];
struct SpriteSheetImage fat_ghost_spritesheet_array[3];
struct SpriteSheetImage grim_reaper_spritesheet_array[3];
struct SpriteSheetImage blue_portal_spritesheet_array[3];
struct SpriteSheetImage red_portal_spritesheet_array[3];
struct SpriteSheetImage basic_turret_spritesheet_array[6];
struct SpriteSheetImage homing_missle_turret_spritesheet_array[6];
struct SpriteSheetImage mine_spritesheet_array[2];
struct SpriteSheetImage turret_bullet_spritesheet_array[3];
struct SpriteSheetImage turret_bullet_radius_spritesheet_array[4];
struct SpriteSheetImage currency_spritesheet_array[5];
struct SpriteSheetImage grid_environment_objects_spritesheet_array[10];
struct SpriteSheetImage background_spritesheet_array[6];
struct SpriteSheetImage portal_enter_effect_spritesheet_array[4];
struct SpriteSheetImage portal_spawn_effect_spritesheet_array[4];
struct SpriteSheetImage power_up_spritesheet_array[4];
struct SpriteSheetImage battlefield_effect_spritesheet_array[12];
struct SpriteSheetImage fat_UI_background_spritesheet_array[2];
struct SpriteSheetImage interactable_UI_buttons_spritesheet_array[6];
struct SpriteSheetImage turret_stats_spritesheet_array[5];
struct SpriteSheetImage non_grid_environment_objects_spritesheet_array[9];
#pragma endregion

struct PortalVariables portal_variables_array[2];

struct LinkedListNode* bullet_radius_head_node;
int bullet_radius_node_key_number; //a key number to access the key from nodes

struct LinkedListNode* portal_enter_head_node;
struct LinkedListNode* portal_spawn_head_node;
int portal_enter_node_key_number;//a key number to access the key from nodes
int portal_spawn_node_key_number;//a key number to access the key from nodes

int portal_current_sprite;	//the current sprite for the portals
float portal_animation_timer; //the timer to change animation for portal
int is_portal_spawn_effect_sprite; //boolean to check which portal effect it is

float battlefield_effect_text_min_word_size;
float battlefield_effect_text_max_word_size;
float battlefield_text_timer;

void InitAllImages(void); //Loads all images into the game
void InitVariablesForSpriteFunctions(void); //Loads all images into the game

void InitSpritesheetArray(void);
void SpritesheetCalculation(struct SpriteSheetImage* s, CP_Image image, int pixelWidth, int pixelHeight, int stopPoint);
void RenderImageFromSpriteSheet(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY);
void RenderImageFromSpriteSheetWithAlpha(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY, int alphaValue);

void InsertNewNodeBulletRadius(struct LinkedListNode** list, float xPos, float yPos, int typeOfBullet);
void InsertNewNodePortalEffect(struct LinkedListNode** list, float xPosInput, float yPosInput, int portalEffect);
struct LinkedListNode* DeleteNode(struct LinkedListNode* list, int key);
int IsLinkedListEmpty(struct LinkedListNode* currentNode);

void RenderAndUpdateBulletCircles(void);
void RenderAndUpdateSinglePortalEffect(struct LinkedListNode* nodeToChange, int portalEffect);
void RenderAllPortalEffects(void);

void RenderPortalSprite(struct SpriteSheetImage s, struct PortalVariables* pv, CP_Image image);
void UpdatePortalAnimation(void);

void RenderLevelEnvironment(const int currentLevel);
float BattlefieldEffectLinearLerp(float start, float end, float value);
void RenderBattlefieldEffectText(const int effect);
