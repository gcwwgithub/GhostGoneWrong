
#include "Gabriel.h"
#include "game.h"

void InitAllImages(void)
{
	slow_turret_image_array[0] = CP_Image_Load("./Assets/SlowTurret1.png");
	slow_turret_image_array[1] = CP_Image_Load("./Assets/SlowTurret2.png");
	slow_turret_image_array[2] = CP_Image_Load("./Assets/SlowTurret3.png");
	slow_turret_image_array[3] = CP_Image_Load("./Assets/SlowTurret1.png");
	slow_turret_image_array[4] = CP_Image_Load("./Assets/SlowTurret4.png");
	slow_turret_image_array[5] = CP_Image_Load("./Assets/SlowTurret5.png");


	pause_button_image = CP_Image_Load("./Assets/Pause.png");
	game_title_image = CP_Image_Load("./Assets/Title.png");
	turret_button_background = CP_Image_Load("./Assets/TurretIcon.png");
	thin_UI_background = CP_Image_Load("./Assets/CurrencyIcon.png");
	upgrade_menu_background = CP_Image_Load("./Assets/UpgradeMenu.png");

	basic_ghost_spritesheet = CP_Image_Load("./Assets/BasicGhost.png");
	fast_ghost_spritesheet = CP_Image_Load("./Assets/FastGhost.png");
	fat_ghost_spritesheet = CP_Image_Load("./Assets/FatGhost.png");
	grim_reaper_spritesheet = CP_Image_Load("./Assets/GrimReaper.png");
	blue_portal_spritesheet = CP_Image_Load("./Assets/FriendlyPortal.png");
	red_portal_spritesheet = CP_Image_Load("./Assets/GhostPortal.png");
	basic_turret_spritesheet = CP_Image_Load("./Assets/BasicTurret.png");
	homing_missle_turret_spritesheet = CP_Image_Load("./Assets/HomingMissleTurret.png");
	mine_spritesheet = CP_Image_Load("./Assets/Mine.png");
	turret_bullet_spritesheet = CP_Image_Load("./Assets/TurretBullets.png");
	turret_bullet_radius_spritesheet = CP_Image_Load("./Assets/BulletsRadius.png");
	currency_spritesheet = CP_Image_Load("./Assets/Currency.png");
	grid_environment_objects_spritesheet = CP_Image_Load("./Assets/GridEnvironmentObject.png");
	background_spritesheet = CP_Image_Load("./Assets/Background.png");
	portal_enter_effect_spritesheet = CP_Image_Load("./Assets/PortalEnterEffect.png");
	portal_spawn_effect_spritesheet = CP_Image_Load("./Assets/EnemySpawnEffect.png");
	power_up_spritesheet = CP_Image_Load("./Assets/Powerup.png");
	battlefield_effect_spritesheet = CP_Image_Load("./Assets/BattlefieldEffects.png");
	fat_UI_background_spritesheet = CP_Image_Load("./Assets/BigUIDisplay.png");
	interactable_UI_buttons_spritesheet = CP_Image_Load("./Assets/InteractableButtons.png");
	turret_stats_spritesheet = CP_Image_Load("./Assets/TurretStatsIcons.png");
	non_grid_environment_objects_spritesheet = CP_Image_Load("./Assets/EnvBackgroundObject.png");
}


void InitVariablesForSpriteFunctions(void)
{
	portal_current_sprite = 0;
	portal_animation_timer = 0;
	bullet_radius_node_key_number = 0;
	bullet_radius_head_node = NULL;
	portal_enter_head_node = NULL;
	portal_enter_node_key_number = 0;
	portal_spawn_head_node = NULL;
	portal_spawn_node_key_number = 0;
	is_portal_spawn_effect_sprite = 0;
	battlefield_effect_text_max_word_size = 45 * scalingFactor;
	battlefield_effect_text_min_word_size = 0;
	battlefield_text_timer = 0;
}

#pragma region LinkedList

void InsertNewNodeBulletRadius(struct LinkedListNode** list, float xPosInput, float yPosInput, int typeOfBullet)
{

	struct LinkedListNode* newNode;
	newNode = malloc(sizeof(struct LinkedListNode));
	if (newNode != NULL)
	{
		newNode->node_image_alpha = 255;
		newNode->key = bullet_radius_node_key_number;
		newNode->next_node = bullet_radius_head_node;
		newNode->node_x_coordinate = xPosInput;
		newNode->node_y_coordinate = yPosInput;

		switch (typeOfBullet)
		{
		case 3:
			newNode->node_counter = 3;
			break;
		case 2:
			newNode->node_counter = 2;
			break;
		case 1:
			newNode->node_counter = 1;
			break;
		case 0:
			newNode->node_counter = 0;
			break;
		}

		newNode->next_node = *list;
		bullet_radius_node_key_number++;
		*list = newNode;
	}

}

void InsertNewNodePortalEffect(struct LinkedListNode** list, float xPosInput, float yPosInput,int portalEffect)
{

	struct LinkedListNode* newNode;
	newNode = malloc(sizeof(struct LinkedListNode));
	if (newNode != NULL)
	{
		switch (portalEffect)
		{
		case 0:
			newNode->key = portal_spawn_node_key_number;
			portal_spawn_node_key_number++;
			break;
		case 1:
			newNode->key = portal_enter_node_key_number;
			portal_enter_node_key_number++;
			break;
		}
		newNode->next_node = *list;
		newNode->node_x_coordinate = xPosInput;
		newNode->node_y_coordinate = yPosInput;
		newNode->node_timer = 0;
		newNode->node_counter = 0;
		
		newNode->next_node = *list;
		*list = newNode;
		
	}

}

//delete a link with given key
struct LinkedListNode* DeleteNode(struct LinkedListNode* list, int key)
{
	struct LinkedListNode* currentNode;
	struct LinkedListNode* previousNode;
	for (currentNode = list, previousNode = NULL; currentNode != NULL && currentNode->key != key;
		previousNode = currentNode, currentNode = currentNode->next_node);
	if (currentNode == NULL)
	{
		return list;
	}

	if (previousNode == NULL)
	{
		list = list->next_node;
	}
	else
	{
		previousNode->next_node = currentNode->next_node;
	}
	free(currentNode);
	return list;

}

int IsLinkedListEmpty(struct LinkedListNode* currentNode) {
	return currentNode == NULL;
}
#pragma endregion


#pragma region SpriteSheetRelated

void InitSpritesheetArray(void)
{
	SpritesheetCalculation(basic_ghost_spritesheet_array, basic_ghost_spritesheet, 128, 128, 1);
	SpritesheetCalculation(fast_ghost_spritesheet_array, fast_ghost_spritesheet, 128, 128, 1);
	SpritesheetCalculation(fat_ghost_spritesheet_array, fat_ghost_spritesheet, 128, 128, 1);
	SpritesheetCalculation(grim_reaper_spritesheet_array, grim_reaper_spritesheet, 128, 128, 1);
	SpritesheetCalculation(blue_portal_spritesheet_array, blue_portal_spritesheet, 128, 128, 1);
	SpritesheetCalculation(red_portal_spritesheet_array, red_portal_spritesheet, 128, 128, 1);
	SpritesheetCalculation(basic_turret_spritesheet_array, basic_turret_spritesheet, 128, 128, 0);
	SpritesheetCalculation(mine_spritesheet_array, mine_spritesheet, 128, 128, 0);
	SpritesheetCalculation(homing_missle_turret_spritesheet_array, homing_missle_turret_spritesheet, 128, 128, 0);
	SpritesheetCalculation(turret_bullet_spritesheet_array, turret_bullet_spritesheet, 128, 128, 1);
	SpritesheetCalculation(turret_bullet_radius_spritesheet_array, turret_bullet_radius_spritesheet, 128, 128, 0);
	SpritesheetCalculation(currency_spritesheet_array, currency_spritesheet, 128, 128, 1);
	SpritesheetCalculation(grid_environment_objects_spritesheet_array, grid_environment_objects_spritesheet, 128, 128, 2);
	SpritesheetCalculation(background_spritesheet_array, background_spritesheet,1920,1080, 0);
	SpritesheetCalculation(portal_enter_effect_spritesheet_array, portal_enter_effect_spritesheet, 128, 128, 0);
	SpritesheetCalculation(portal_spawn_effect_spritesheet_array, portal_spawn_effect_spritesheet, 128, 128, 0);
	SpritesheetCalculation(power_up_spritesheet_array, power_up_spritesheet, 128, 128, 0);
	SpritesheetCalculation(battlefield_effect_spritesheet_array, battlefield_effect_spritesheet, 128, 128, 0);
	SpritesheetCalculation(fat_UI_background_spritesheet_array, fat_UI_background_spritesheet, 128, 128, 0);
	SpritesheetCalculation(interactable_UI_buttons_spritesheet_array, interactable_UI_buttons_spritesheet, 128, 72, 1);
	SpritesheetCalculation(turret_stats_spritesheet_array, turret_stats_spritesheet, 128, 128, 1);
	SpritesheetCalculation(non_grid_environment_objects_spritesheet_array, non_grid_environment_objects_spritesheet, 512, 512, 0);
}

void SpritesheetCalculation(struct SpriteSheetImage* s, CP_Image image, int pixelWidth, int pixelHeight, int stopPoint)
{

	int width = CP_Image_GetWidth(image);
	int height = CP_Image_GetHeight(image);
	int counter = 0;
	for (int j = 0; j < height / pixelHeight; j++)
	{

		for (int i = 0; i < width / pixelWidth; i++)
		{
			if (!(j == height / pixelHeight- 1 && i > width / pixelWidth - stopPoint - 1))
			{
				s[counter].image_width= pixelWidth;
				s[counter].image_height = pixelHeight;
				s[counter].left_pixel_value = (float)(i * s[counter].image_width);
				s[counter].right_pixel_value = (float)((i + 1) * s[counter].image_width);
				s[counter].top_pixel_value = (float)(j * s[counter].image_height);
				s[counter].bottom_pixel_value = (float)((j + 1) * s[counter].image_height);
				counter++;
			}
		}
	}
}


void RenderPortalSprite(struct SpriteSheetImage s, struct PortalVariables* pv, CP_Image image)
{
	CP_Image_DrawSubImage(image, pv->portal_x_coordinate, pv->portal_y_coordinate, pv->portal_image_width, pv->portal_image_height,
		s.left_pixel_value, s.top_pixel_value, s.right_pixel_value, s.bottom_pixel_value, 255);
}

void RenderImageFromSpriteSheetWithAlpha(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY, int alphaValue)
{
	//put size of image as the turret size
	CP_Image_DrawSubImage(image, xPos, yPos, sizeOfImageX, sizeOfImageY,
		s.left_pixel_value, s.top_pixel_value, s.right_pixel_value, s.bottom_pixel_value, alphaValue);
}

void RenderImageFromSpriteSheet(CP_Image image, struct SpriteSheetImage s, float xPos, float yPos, float sizeOfImageX, float sizeOfImageY)
{
	RenderImageFromSpriteSheetWithAlpha(image, s, xPos, yPos, sizeOfImageX, sizeOfImageY,255);
}

#pragma endregion

void RenderAndUpdateBulletCircles(void)
{
	if (!IsLinkedListEmpty(bullet_radius_head_node))
	{
		struct LinkedListNode* current = bullet_radius_head_node;
		while (1)
		{

			if (current != NULL)
			{
				RenderImageFromSpriteSheetWithAlpha(turret_bullet_radius_spritesheet, turret_bullet_radius_spritesheet_array[current->node_counter], current->node_x_coordinate, current->node_y_coordinate,
					100.0f, 100.0f,current->node_image_alpha);
				current->node_image_alpha -= 50;
				if (current->node_image_alpha < 0)
				{
					bullet_radius_head_node = DeleteNode(current, current->key);
					break;
				}
				if (current->next_node == NULL)
				{
					break;
				}

				else
				{
					current = current->next_node;
				}
			}

			else
			{
				break;
			}

		}
	}

}

void RenderAndUpdateSinglePortalEffect(struct LinkedListNode* nodeToChange, int portalEffect)
{
	if (!IsLinkedListEmpty(nodeToChange))
	{
		struct LinkedListNode* current = nodeToChange;
		while (1)
		{

			if (current != NULL)
			{
				switch (portalEffect)
				{
				case 0:
					RenderImageFromSpriteSheet(portal_spawn_effect_spritesheet, 
						portal_spawn_effect_spritesheet_array[current->node_counter], 
						current->node_x_coordinate, current->node_y_coordinate,50, 50);
					break;
				case 1:
					RenderImageFromSpriteSheet(portal_enter_effect_spritesheet,
						portal_enter_effect_spritesheet_array[current->node_counter],
						current->node_x_coordinate, current->node_y_coordinate, 50, 50);
					break;
				}

				
				current->node_timer += CP_System_GetDt();
				if (current->node_timer > 0.25)
				{
					current->node_timer = 0;
					if (current->node_counter >= 3)
					{
						
						switch (portalEffect)
						{
						case 0:
							portal_spawn_head_node = DeleteNode(current, current->key);
							break;
						case 1:
							portal_enter_head_node = DeleteNode(current, current->key);
							break;
						}
						break;
					}
					else
					{
						current->node_counter++;
					}
					
					
				}
				if (current->next_node == NULL)
				{
					break;
				}

				else
				{
					current = current->next_node;
				}
			}

			else
			{
				break;
			}

		}
	}

}

void RenderAllPortalEffects(void)
{
	RenderAndUpdateSinglePortalEffect(portal_enter_head_node,!is_portal_spawn_effect_sprite);
	RenderAndUpdateSinglePortalEffect(portal_spawn_head_node, is_portal_spawn_effect_sprite);
}

void UpdatePortalAnimation(void)
{
	if (portal_animation_timer >= 0.25)
	{
		if (portal_current_sprite == 2)
		{
			portal_current_sprite = 0;
			portal_animation_timer = 0;
		}

		else
		{
			portal_current_sprite++;
			portal_animation_timer = 0;
		}
	}
	portal_animation_timer += CP_System_GetDt();
	RenderPortalSprite(blue_portal_spritesheet_array[portal_current_sprite], &portal_variables_array[0], blue_portal_spritesheet);
	RenderPortalSprite(red_portal_spritesheet_array[portal_current_sprite], &portal_variables_array[1], red_portal_spritesheet);

}

void RenderLevelEnvironment(const int currentLevel)
{
	switch (currentLevel)
	{
	case 0:
		# pragma region Level 1 Objects
		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderImageFromSpriteSheetWithAlpha(background_spritesheet, background_spritesheet_array[1], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[1], 
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.85f, 
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowWidth() * 0.3f);

		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.11f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.65f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.75f, CP_System_GetWindowHeight() * 0.11f,
			128 * scalingFactor, 128 * scalingFactor, 200);

		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.89f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.45f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.55f, CP_System_GetWindowHeight() * 0.89f,
			128 * scalingFactor, 128 * scalingFactor, 200);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.3f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.6f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.95f, CP_System_GetWindowHeight() * 0.55f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.69f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.75f,
			64 * scalingFactor, 64 * scalingFactor);
#pragma endregion
		break;
	case 1:
		#pragma region Level 2 Objects
		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderImageFromSpriteSheetWithAlpha(background_spritesheet, background_spritesheet_array[2], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);
		RenderImageFromSpriteSheetWithAlpha(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor, 200);


		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.3f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.22f, CP_System_GetWindowHeight() * 0.72f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.65f, CP_System_GetWindowHeight() * 0.50f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[4],
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.45f,
			128 * scalingFactor, 128 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[4],
			CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.1f,
			128 * scalingFactor, 128 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[4],
			CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.9f,
			128 * scalingFactor, 128 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[6],
			CP_System_GetWindowWidth() * 0.45f, CP_System_GetWindowHeight() * 0.85f,
			80 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[6],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.75f,
			80 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.55f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.75f, CP_System_GetWindowHeight() * 0.3f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.6f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[9],
			CP_System_GetWindowWidth() * 0.8f, CP_System_GetWindowHeight() * 0.8f,
			64 * scalingFactor, 64 * scalingFactor);
#pragma endregion
		break;
	case 2:
		#pragma region Level 3 Objects
		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderImageFromSpriteSheetWithAlpha(background_spritesheet, background_spritesheet_array[3], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 200);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[3],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.19f,
			300* scalingFactor, 300 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[7],
			CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.65f,
			200 * scalingFactor, 200 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.85f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.2f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.75f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.8f, CP_System_GetWindowHeight() * 0.85f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.7f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[9],
			CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.8f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[9],
			CP_System_GetWindowWidth() * 0.65f, CP_System_GetWindowHeight() * 0.5f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[9],
			CP_System_GetWindowWidth() * 0.35f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[9],
			CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[9],
			CP_System_GetWindowWidth() * 0.9f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.6f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);
#pragma endregion
		break;
	case 3:
		#pragma region Level 4 Objects
		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderImageFromSpriteSheetWithAlpha(background_spritesheet, background_spritesheet_array[4], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 200);
		
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.15f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.25f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.4f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.14f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[5],
			CP_System_GetWindowWidth() * 0.23f, CP_System_GetWindowHeight() * 0.8f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[9],
			CP_System_GetWindowWidth() * 0.25f, CP_System_GetWindowHeight() * 0.65f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[9],
			CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[9],
			CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.3f, CP_System_GetWindowHeight() * 0.1f,
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.4f, CP_System_GetWindowHeight() * 0.9f,
			64 * scalingFactor, 64 * scalingFactor);

		for (int i = 0; i < 8; i++)
		{
			RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[4],
				CP_System_GetWindowWidth() * 0.7f, (CP_System_GetWindowHeight() * (0.05f + i*0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
			RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[5],
				CP_System_GetWindowWidth() * 0.7f, (CP_System_GetWindowHeight() * (0.1f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
		}
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[1],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.85f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[1],
			CP_System_GetWindowWidth() * 0.87f, (CP_System_GetWindowHeight() * 0.85f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[1],
			CP_System_GetWindowWidth() * 0.89f, (CP_System_GetWindowHeight() * 0.85f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[3],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.65f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[3],
			CP_System_GetWindowWidth() * 0.75f, (CP_System_GetWindowHeight() * 0.45f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.45f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.3f, (CP_System_GetWindowHeight() * 0.5f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[6],
			CP_System_GetWindowWidth() * 0.5f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);

#pragma endregion
		break;
	case 4:
		#pragma region Level 5 Objects

		CP_Settings_Tint(CP_Color_Create(0, 0, 0, 75));
		RenderImageFromSpriteSheetWithAlpha(background_spritesheet, background_spritesheet_array[5], CP_System_GetWindowWidth() * 0.5f,
			CP_System_GetWindowHeight() * 0.5f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 200);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.85f, CP_System_GetWindowHeight() * 0.85f,
			75 * scalingFactor, 75 * scalingFactor);

		RenderImageFromSpriteSheet(non_grid_environment_objects_spritesheet, non_grid_environment_objects_spritesheet_array[8],
			CP_System_GetWindowWidth() * 0.15f, CP_System_GetWindowHeight() * 0.35f,
			75 * scalingFactor, 75 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[3],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.45f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[7],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.55f),
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[6],
			CP_System_GetWindowWidth() * 0.9f, (CP_System_GetWindowHeight() * 0.6f),
			48 * scalingFactor, 48 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[6],
			CP_System_GetWindowWidth() * 0.85f, (CP_System_GetWindowHeight() * 0.65f),
			48 * scalingFactor, 48 * scalingFactor);


		for (int i = 0; i < 8; i++)
		{
			RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[4],
				CP_System_GetWindowWidth() * 0.3f, (CP_System_GetWindowHeight() * (0.05f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
			RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[5],
				CP_System_GetWindowWidth() * 0.3f, (CP_System_GetWindowHeight() * (0.1f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
			RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[4],
				CP_System_GetWindowWidth() * 0.7f, (CP_System_GetWindowHeight() * (0.05f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);
			RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[5],
				CP_System_GetWindowWidth() * 0.7f, (CP_System_GetWindowHeight() * (0.1f + i * 0.12f)),
				48 * scalingFactor, 48 * scalingFactor);

		}
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[7],
			CP_System_GetWindowWidth() * 0.15f, (CP_System_GetWindowHeight() * 0.1f),
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[6],
			CP_System_GetWindowWidth() * 0.2f, (CP_System_GetWindowHeight() * 0.1f),
			48 * scalingFactor, 48 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[6],
			CP_System_GetWindowWidth() * 0.15f, (CP_System_GetWindowHeight() * 0.15f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[7],
			CP_System_GetWindowWidth() * 0.15f, (CP_System_GetWindowHeight() * 0.8f),
			64 * scalingFactor, 64 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[6],
			CP_System_GetWindowWidth() * 0.2f, (CP_System_GetWindowHeight() * 0.8f),
			48 * scalingFactor, 48 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[6],
			CP_System_GetWindowWidth() * 0.15f, (CP_System_GetWindowHeight() * 0.85f),
			48 * scalingFactor, 48 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.4f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[1],
			CP_System_GetWindowWidth() * 0.35f, (CP_System_GetWindowHeight() * 0.2f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[1],
			CP_System_GetWindowWidth() * 0.45f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.5f, (CP_System_GetWindowHeight() * 0.1f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.6f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[3],
			CP_System_GetWindowWidth() * 0.55f, (CP_System_GetWindowHeight() * 0.15f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[3],
			CP_System_GetWindowWidth() * 0.65f, (CP_System_GetWindowHeight() * 0.12f),
			56 * scalingFactor, 56 * scalingFactor);

		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[1],
			CP_System_GetWindowWidth() * 0.41f, (CP_System_GetWindowHeight() * 0.85f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[0],
			CP_System_GetWindowWidth() * 0.35f, (CP_System_GetWindowHeight() * 0.9f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[1],
			CP_System_GetWindowWidth() * 0.42f, (CP_System_GetWindowHeight() * 0.94f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[3],
			CP_System_GetWindowWidth() * 0.52f, (CP_System_GetWindowHeight() * 0.8f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[2],
			CP_System_GetWindowWidth() * 0.63f, (CP_System_GetWindowHeight() * 0.91f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[1],
			CP_System_GetWindowWidth() * 0.51f, (CP_System_GetWindowHeight() * 0.83f),
			56 * scalingFactor, 56 * scalingFactor);
		RenderImageFromSpriteSheet(grid_environment_objects_spritesheet, grid_environment_objects_spritesheet_array[3],
			CP_System_GetWindowWidth() * 0.65f, (CP_System_GetWindowHeight() * 0.82f),
			56 * scalingFactor, 56 * scalingFactor);
#pragma endregion
		break;
		
	}
	
}


float BattlefieldEffectLinearLerp(float start, float end, float value)
{
	return (1.f - value) * start + value * end;
}

void StartBattleFieldEffectTimer(int effect)
{
	battlefield_text_timer = 0;
	battlefield_effect_text_min_word_size = 0;
	switch (effect)
	{
	case 1:
		battlefield_effect_text_max_word_size = 60 * scalingFactor;
		break;
	case 2:
		battlefield_effect_text_max_word_size = 60 * scalingFactor;
		break;
	case 3:
		battlefield_effect_text_max_word_size = 60 * scalingFactor;
		break;
	case 4:
		battlefield_effect_text_max_word_size = 60 * scalingFactor;
		break;
	case 5:
		battlefield_effect_text_max_word_size = 50 * scalingFactor;
		break;
	case 6:
		battlefield_effect_text_max_word_size = 50* scalingFactor;
		break;
	case 7:
		battlefield_effect_text_max_word_size = 60 * scalingFactor;
		break;
	case 8:
		battlefield_effect_text_max_word_size = 60 * scalingFactor;
		break;
	case 9:
		battlefield_effect_text_max_word_size = 45 * scalingFactor;
		break;
	case 10:
		battlefield_effect_text_max_word_size = 45 * scalingFactor;
		break;
	case 11:
		battlefield_effect_text_max_word_size = 55 * scalingFactor;
		break;
	}
}
void RenderBattlefieldEffectText(const int effect)
{
	char temp[100];
	static float timer = 0;
	float sizeTimer = 0;
	float duration = 0.5;
	float stayOnScreenDuration = 3;

	if (battlefield_text_timer <= (stayOnScreenDuration + 2 * duration))
	{
		timer += CP_System_GetDt();
		battlefield_text_timer += CP_System_GetDt();
		sizeTimer = timer;
		if (timer >= duration)
		{
			sizeTimer = duration;
			if (timer >= duration + stayOnScreenDuration)
			{
				float temp = 0;
				temp = battlefield_effect_text_max_word_size;
				battlefield_effect_text_max_word_size = battlefield_effect_text_min_word_size;
				battlefield_effect_text_min_word_size = temp;
				timer = 0;
			}
		}

		float currentSize = BattlefieldEffectLinearLerp(battlefield_effect_text_min_word_size, battlefield_effect_text_max_word_size, sizeTimer/ duration);

		CP_Settings_Fill(COLOR_WHITE);
		CP_Settings_TextSize(currentSize);
		switch (effect)
		{
		case kIncreasedPhantomQuartz:
			sprintf_s(temp, sizeof(temp), "MORE QUARTZ EARNED THIS WAVE");
			break;
		case kDecreasedPhantomQuartz:
			sprintf_s(temp, sizeof(temp), "LESS QUARTZ EARNED THIS WAVE");
			break;
		case kFasterEnemies:
			sprintf_s(temp, sizeof(temp), "FASTER ENEMIES THIS WAVE");
			break;
		case kSlowerEnemies:
			sprintf_s(temp, sizeof(temp), "SLOWER ENEMIES THIS WAVE");
			break;
		case kIncreasedTurretDamage:
			sprintf_s(temp, sizeof(temp), "TURRET DAMAGE INCREASED THIS WAVE");
			break;
		case kDecreasedTurretDamage:
			sprintf_s(temp, sizeof(temp), "TURRET DAMAGE DECREASED THIS WAVE");
			break;
		case kMoreHP:
			sprintf_s(temp, sizeof(temp), "GHOST HAS MORE HP THIS WAVE");
			break;
		case kLessHP:
			sprintf_s(temp, sizeof(temp), "GHOST HAS LESS HP THIS WAVE");
			break;
		case kIncreasedTurretAttackSpeed:
			sprintf_s(temp, sizeof(temp), "TURRET ATTACK SPEED INCREASED THIS WAVE");
			break;
		case kDecreasedTurretAttackSpeed:
			sprintf_s(temp, sizeof(temp), "TURRET ATTACK SPEED DECREASED THIS WAVE");
			break;
		case kNoPhantomQuartz:
			sprintf_s(temp, sizeof(temp), "NO QUARTZ WILL DROP THIS WAVE");
			break;
		}


		CP_Font_DrawText(temp, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 5);
	}

	else
	{
		timer = 0;
		
	}




}