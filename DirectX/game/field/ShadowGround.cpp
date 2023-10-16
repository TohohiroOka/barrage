#include "ShadowGround.h"

ShadowGround::ShadowGround()
{
	model = Model::CreateFromOBJ("ground");
	object = Object3d::Create(model.get());
	object->SetPosition({100, 1, 100});

	//シャドウマップで影を付ける
	object->SetShadowMap(true);
}

void ShadowGround::Update()
{
	object->Update();
}

void ShadowGround::Draw()
{
	object->Draw();
}

void ShadowGround::DrawLightView()
{
	object->DrawLightView();
}
