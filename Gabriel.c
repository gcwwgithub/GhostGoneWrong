
#include "Gabriel.h"
void init_all_images(void)
{
	triangleTurretImageArray[0] = CP_Image_Load("./Assets/BasicTurret1.png");
	triangleTurretImageArray[1] = CP_Image_Load("./Assets/BasicTurret2.png");
	triangleTurretImageArray[2] = CP_Image_Load("./Assets/BasicTurret3.png");
	triangleTurretImageArray[3] = CP_Image_Load("./Assets/BasicTurret1.png");
	triangleTurretImageArray[4] = CP_Image_Load("./Assets/BasicTurret4.png");
	triangleTurretImageArray[5] = CP_Image_Load("./Assets/BasicTurret5.png");

	redArrowImageArray[0] = CP_Image_Load("./Assets/BasicGhost1.png");
	redArrowImageArray[1] = CP_Image_Load("./Assets/BasicGhost2.png");
	redArrowImageArray[2] = CP_Image_Load("./Assets/BasicGhost3.png");

	tempStar = CP_Image_Load("./Assets/Star1.png");
	tempCircle = CP_Image_Load("./Assets/Circle1.png");
	tempPercentage = CP_Image_Load("./Assets/Percentage1.png");
	tempBullet = CP_Image_Load("./Assets/BasicTurretBullet.png");
	tempBulletRadius = CP_Image_Load("./Assets/BulletRadius.png");
	bluePortalSpriteSheet = CP_Image_Load("./Assets/SummationPortal.png"); 
	redPortalSpriteSheet = CP_Image_Load("./Assets/EnemyPortal.png");  
	portalCounter = 0;
	portalTimer = 0;
	keyNumber = 0;
	firstNode = NULL;
}


void insert_new_node(struct node** list, float xPosInput, float yPosInput)
{
	struct node* newNode;
	newNode = malloc(sizeof(struct node));
	if (newNode != NULL)
	{
		newNode->circleAlphaValue = 255;
		newNode->key = keyNumber;
		newNode->next = firstNode;
		newNode->xPos = xPosInput;
		newNode->yPos = yPosInput;

		newNode->next = *list;
		keyNumber++;
		*list = newNode;
	}
	
}


//delete a link with given key
struct node* delete_node(struct node* list, int key)
{
	struct node* currentNode;
	struct node* previousNode;
	for (currentNode = list, previousNode = NULL; currentNode != NULL && currentNode->key != key;
		previousNode = currentNode, currentNode = currentNode->next);
	if (currentNode == NULL)
	{
		return list;
	}

	if (previousNode == NULL)
	{
		list = list->next;
	}
	else
	{
		previousNode->next = currentNode->next;
	}
	free(currentNode);
	return list;

}

void render_bullet_circles()
{
	if (!isEmpty())
	{
		struct node* current = firstNode;
		while (1)
		{
			
			if (current != NULL)
			{
				CP_Image_DrawAdvanced(tempBulletRadius, current->xPos, current->yPos,
					100.0f, 100.0f, current->circleAlphaValue, 0);
				current->circleAlphaValue -= 50;
				if (current->circleAlphaValue < 0)
				{
					firstNode=delete_node(current, current->key);
					break;
				}
				if (current->next == NULL)
				{
					break;
				}

				else
				{
					current = current->next;
				}
			}

			else
			{
				break;
			}

		}
	}

}

int isEmpty() {
	return firstNode == NULL;
}


void SpreadsheetCalculation(struct SpreadSheetImage* s, CP_Image image, int pixel, int stopPoint)
{
	int width = CP_Image_GetWidth(image);
	int height = CP_Image_GetHeight(image);
	int counter = 0;

	for (int j = 0; j < height /pixel; j++)
	{
		
		for (int i = 0; i < width / pixel; i++)
		{	
			if (!(j == height / pixel - 1 && i > width/pixel-stopPoint-1))
			{

				s[counter].pixelOfImage = pixel;
				s[counter].leftXPixel = (float)(i * s[counter].pixelOfImage);
				s[counter].rightXPixel = (float)((i + 1) * s[counter].pixelOfImage);
				s[counter].topYPixel = (float)(j * s[counter].pixelOfImage);
				s[counter].bottomYPixel = (float)((j + 1) * s[counter].pixelOfImage);
				counter++;
			}
		}
	}
}

void UpdatePortal(void)
{
	if (portalTimer >= 0.25)
	{
		if (portalCounter == 2)
		{
			portalCounter = 0;
			portalTimer = 0;
		}

		else
		{
			portalCounter++;
			portalTimer = 0;
		}
	}
	portalTimer += CP_System_GetDt();
	SpreadsheetDraw(bluePortalArray[portalCounter], &portalVariablesArray[0], bluePortalSpriteSheet);
	SpreadsheetDraw(redPortalArray[portalCounter], &portalVariablesArray[1], redPortalSpriteSheet);

}

void SpreadsheetDraw(struct SpreadSheetImage s, struct PortalVariables* pv, CP_Image image)
{
	CP_Image_DrawSubImage(image, pv->portalXPos, pv->portalYPos, pv->sizeX, pv->sizeY,
		s.leftXPixel, s.topYPixel, s.rightXPixel, s.bottomYPixel, 255);
}

void SpreadsheetInit(void)
{
	SpreadsheetCalculation(bluePortalArray, bluePortalSpriteSheet, 128, 1);
	SpreadsheetCalculation(redPortalArray, redPortalSpriteSheet, 128, 1);
}
