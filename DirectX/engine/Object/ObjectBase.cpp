#include "ObjectBase.h"
#include "Object/3d/Base3D.h"
#include "Object/2d/Sprite.h"
#include "Object/2d/ShaoeSprite.h"

ID3D12Device* ObjectBase::device = nullptr;
ID3D12GraphicsCommandList* ObjectBase::cmdList = nullptr;

void ObjectBase::StaticInitialize(ID3D12Device* _device)
{
	// 初期化チェック
	assert(!ObjectBase::device);

	// nullptrチェック
	assert(_device);

	ObjectBase::device = _device;

	// モデルの静的初期化
	Model::StaticInitialize(_device);

	//スプライトの初期化
	Sprite::StaticInitialize();

	ShaoeSprite::StaticInitialize();
}

void ObjectBase::Draw(const GraphicsPipelineManager::DrawSet& _drawSet)
{
	GraphicsPipelineManager::PreDraw(cmdList, _drawSet);
}
