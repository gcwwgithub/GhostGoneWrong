/*!
All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       main.c
@author     Ng Zheng Wei (zhengwei.ng@digipen.edu)	
@co-author	Chiok Wei Wen Gabriel (chiok.w@digipen.edu)
			Lim Jing Rui John (l.jingruijohn@digipen.edu)
			Phua Tai Dah Anderson (a.phua@digipen.edu)
			Wong Zhi Hao Samuel (w.zhihaosamuel@digipen.edu)
@course     CSD 1400
@section    C
@date       1/12/2021
@brief    	main.c functions to run game
*//*__________________________________________________________________________*/
#include "cprocessing.h"

void game_init(void);
void game_update(void);
void game_exit(void);

/* Entry point */
int main(void)
{
    CP_Engine_SetNextGameState(game_init, game_update, game_exit);
    CP_Engine_Run();
    return 0;
}
