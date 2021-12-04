/*!
All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Gabriel.h
@author     Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
@course     CSD 1400
@section    C
@date       28/11/2021
@brief    	Contains functions protoypes that are related to rendering, animation and effects
			All functions are done by Chiok Wei Wen Gabriel
			The list of functions are: InitAllImages, InitVariablesForSpriteFunctions, InitSpritesheetArray
			SpritesheetCalculation,RenderImageFromSpriteSheetWithAlpha, InsertNewNodeBulletRadius
			InsertNewNodePortalEffect, DeleteNode, IsLinkedListEmpty, RenderAndUpdateBulletCircles
			RenderAndUpdateSinglePortalEffect, RenderAllPortalEffects,RenderPortalSprite
			UpdatePortalAnimation, RenderLevelEnvironment, BattlefieldEffectLinearLerp
			RenderBattlefieldEffectText
			
*//*__________________________________________________________________________*/

#pragma once
#include "cprocessing.h"
#include <stdlib.h>


#pragma region Struct Declarations
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

#pragma endregion

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

//the array of Spritesheet structs
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

#pragma region Variables
struct PortalVariables portal_variables_array[2];

struct LinkedListNode* bullet_radius_head_node; //the linked list for bullet radius
int bullet_radius_node_key_number; //a key number to access the key from nodes

struct LinkedListNode* portal_enter_head_node; //the linked list for portal enter effects
struct LinkedListNode* portal_spawn_head_node; //the linked list for portal spawn effects
int portal_enter_node_key_number;//a key number to access the key from nodes
int portal_spawn_node_key_number;//a key number to access the key from nodes

int portal_current_sprite;	//the current sprite for the portals
float portal_animation_timer; //the timer to change animation for portal
int is_portal_spawn_effect_sprite; //boolean to check which portal effect it is

float battlefield_effect_text_min_word_size;
float battlefield_effect_text_max_word_size;
float battlefield_text_timer;
#pragma endregion

#pragma region Functions
void InitAllImages(void); //Loads all images into the game
void InitVariablesForSpriteFunctions(void); //initialises some variables for the functions

//Creates the array of all the Spritesheet structs
void InitSpritesheetArray(void);

//Calcuates the  4 pixel positions of an image to use CP_DrawSubImage from
void SpritesheetCalculation(struct SpriteSheetImage* s, CP_Image image, int pixelWidth, 
	int pixelHeight, int stopPoint);

//Render an image based from CP_DrawSubImage
void RenderImageFromSpriteSheet(CP_Image image, struct SpriteSheetImage s, 
	float xPos, float yPos, float sizeOfImageX, float sizeOfImageY);

//Render an image based from CP_DrawSubImage with alpha values
void RenderImageFromSpriteSheetWithAlpha(CP_Image image, struct SpriteSheetImage s, 
	float xPos, float yPos, float sizeOfImageX, float sizeOfImageY, int alphaValue);

//Adds a bullet radius node to linkedlist
void InsertNewNodeBulletRadius(struct LinkedListNode** list, float xPos, float yPos, int typeOfBullet);

//Adds a portal node to linkedlist
void InsertNewNodePortalEffect(struct LinkedListNode** list, float xPosInput, float yPosInput, int portalEffect);

//Deletes a node from any linkedlist
struct LinkedListNode* DeleteNode(struct LinkedListNode* list, int key);

//checks if the specified linkedlist is empty
int IsLinkedListEmpty(struct LinkedListNode* currentNode);

//Renders the bullets circles and also updates to appear and disappear once the alpha reaches 0
void RenderAndUpdateBulletCircles(void);

//Renders a portal effect and also updates to appear and disappear once the alpha reaches 0
void RenderAndUpdateSinglePortalEffect(struct LinkedListNode* nodeToChange, int portalEffect);

//The function to call RenderAndUpdateSinglePortalEffect for all potals
void RenderAllPortalEffects(void);

//The function to render the portal
void RenderPortalSprite(struct SpriteSheetImage s, struct PortalVariables* pv, CP_Image image);

//This function updates the sprite for the portal for animation
void UpdatePortalAnimation(void);

//Renders the environments objects and background in the level
void RenderLevelEnvironment(const int currentLevel);

//used for UI tweening for battlefield effect 
float BattlefieldEffectLinearLerp(float start, float end, float value);

//Renders battlefield effect text
void RenderBattlefieldEffectText(const int effect);

#pragma endregion