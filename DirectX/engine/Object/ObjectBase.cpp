#include "ObjectBase.h"
#include "Object/3d/Base3D.h"
#include "Object/2d/Sprite.h"

ID3D12Device* ObjectBase::device = nullptr;
ID3D12GraphicsCommandList* ObjectBase::cmdList = nullptr;

void ObjectBase::StaticInitialize(ID3D12Device* _device)
{
	// �������`�F�b�N
	assert(!ObjectBase::device);

	// nullptr�`�F�b�N
	assert(_device);

	ObjectBase::device = _device;

	// ���f���̐ÓI������
	Model::StaticInitialize(_device);

	//�X�v���C�g�̏�����
	Sprite::StaticInitialize();
}

void ObjectBase::Draw(const GraphicsPipelineManager::DrawSet& _drawSet)
{
	GraphicsPipelineManager::PreDraw(cmdList, _drawSet);
}
