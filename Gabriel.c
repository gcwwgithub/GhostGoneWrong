
#include "cprocessing.h"
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
}