
#include "Gabriel.h"
void init_all_images(void)
{
	triangleTurretImageArray[0] = CP_Image_Load("./Assets/Triangle1.png");
	triangleTurretImageArray[1] = CP_Image_Load("./Assets/Triangle2.png");
	triangleTurretImageArray[2] = CP_Image_Load("./Assets/Triangle3.png");
	triangleTurretImageArray[3] = CP_Image_Load("./Assets/Triangle4.png");
	triangleTurretImageArray[4] = CP_Image_Load("./Assets/Triangle5.png");
	triangleTurretImageArray[5] = CP_Image_Load("./Assets/Triangle6.png");

	redArrowImageArray[0] = CP_Image_Load("./Assets/RedArrow1.png");
	redArrowImageArray[1] = CP_Image_Load("./Assets/RedArrow2.png");
	redArrowImageArray[2] = CP_Image_Load("./Assets/RedArrow3.png");

	tempStar = CP_Image_Load("./Assets/Star1.png");
	tempCircle = CP_Image_Load("./Assets/Circle1.png");
	tempPercentage = CP_Image_Load("./Assets/Percentage1.png");
	tempBullet = CP_Image_Load("./Assets/TriangleBullet.png");
	tempBulletRadius = CP_Image_Load("./Assets/BulletRadius.png");

	keyNumber = 0;
	firstNode = NULL;
}


void insert_new_node(struct node** list, float xPosInput, float yPosInput)
{
	struct node* newNode;
	newNode = malloc(sizeof(struct node));
	newNode->circleAlphaValue = 255;
	newNode->key = keyNumber;
	newNode->next = firstNode;
	newNode->xPos = xPosInput;
	newNode->yPos = yPosInput;

	newNode->next = *list;
	keyNumber++;
	*list = newNode;
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
