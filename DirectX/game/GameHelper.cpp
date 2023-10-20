#include "GameHelper.h"

void GameHelper::Update()
{
	if (slowTimer == 0) { return; }
	slowTimer--;
	if (slowTimer != 0) { return; }
	gameSpeed = 1;
}