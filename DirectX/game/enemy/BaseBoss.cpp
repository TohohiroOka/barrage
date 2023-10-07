#include "BaseBoss.h"

void BaseBoss::Initialize()
{
	center = Object3d::Create();
}

void BaseBoss::Update()
{
	center->Update();
}
