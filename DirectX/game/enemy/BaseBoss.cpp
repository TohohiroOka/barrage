#include "BaseBoss.h"

void BaseBoss::Initialize()
{
	center = Object3d::Create();
	center->SetPosition({ 400.0f,10.0f,400.0f });
}

void BaseBoss::Update()
{
	center->Update();
}
