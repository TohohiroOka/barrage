#include "InstanceObject.h"
#include "Light/LightGroup.h"
#include "Camera/Camera.h"
#include <string>
#include "SafeDelete.h"

using namespace Microsoft::WRL;
using namespace DirectX;

std::vector<GraphicsPipelineManager::DrawSet> InstanceObject::pipeline;

std::unique_ptr<InstanceObject> InstanceObject::Create(Model* _model)
{
	//�C���X�^���X�𐶐�
	InstanceObject* instance = new InstanceObject();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	instance->Initialize(_model);

	return std::unique_ptr<InstanceObject>(instance);
}

void InstanceObject::Initialize(Model* _model)
{
	model = _model;

	HRESULT result = S_FALSE;

	instanceDrawNum = 0;

	for (int i = 0; i < draw_max_num; i++)
	{
		objInform.baseColor[i] = { float(i) / float(draw_max_num),0,0,0 };
		objInform.matWorld[i] = XMMatrixIdentity();
	}

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA_B0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));
	if (FAILED(result)) { assert(0); }

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(OBJECT_INFO) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1));
	if (FAILED(result)) { assert(0); }
}

InstanceObject::InstanceObject()
{
	outlineColor = { 1.0f,1.0f, 1.0f };
}

InstanceObject::~InstanceObject()
{
	constBuffB0.Reset();
	constBuffB1.Reset();
}

void InstanceObject::DrawInstance(const XMFLOAT3& _pos, const XMFLOAT3& _scale,
	const XMFLOAT3& _rotation, const XMFLOAT4& _color)
{
	//���[���h�s��ϊ�
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	matWorld *= matScale;

	XMMATRIX matRot;//�p�x
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(_rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(_rotation.y));
	matWorld *= matRot;

	XMMATRIX matTrans;//���s����
	matTrans = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
	matWorld *= matTrans;

	objInform.baseColor[instanceDrawNum] = _color;
	objInform.matWorld[instanceDrawNum] = matWorld;
	instanceDrawNum++;
}

void InstanceObject::DrawInstance(const XMMATRIX& _worldMat, const XMFLOAT4& _color)
{
	objInform.baseColor[instanceDrawNum] = _color;
	objInform.matWorld[instanceDrawNum] = _worldMat;
	instanceDrawNum++;
}

void InstanceObject::Update()
{
	//�萔�o�b�t�@�Ƀf�[�^��]��
	CONST_BUFFER_DATA_B0* constMap = nullptr;
	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	if (SUCCEEDED(result)) {
		if (camera)
		{
			constMap->viewproj = camera->GetView() * camera->GetProjection();
			constMap->cameraPos = camera->GetEye();
		} else
		{
			constMap->viewproj = XMMatrixIdentity();
			constMap->cameraPos = { 0,0,0 };
		}
		constMap->isBloom = isBloom;
		constMap->isToon = isToon;
		constMap->isOutline = isOutline;
		constMap->isLight = isLight;
		constMap->outlineColor = outlineColor;
		constBuffB0->Unmap(0, nullptr);
	}

	OBJECT_INFO* constMapB1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMapB1);//�}�b�s���O
	if (SUCCEEDED(result)) {
		for (int i = 0; i < draw_max_num; i++)
		{
			constMapB1->baseColor[i] = objInform.baseColor[i];
			constMapB1->matWorld[i] = objInform.matWorld[i];
		}
		constBuffB1->Unmap(0, nullptr);
	}
}

void InstanceObject::Draw(const DrawMode _drawMode)
{
	Update();

	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (model == nullptr) {
		return;
	}

	int modeNum = int(_drawMode);

	Base3D::Draw(pipeline[modeNum]);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(3, constBuffB1->GetGPUVirtualAddress());

	// ���C�g�̕`��
	light->Draw(cmdList, 2);

	// ���f���`��
	model->Draw(cmdList, 4, instanceDrawNum);
}

void InstanceObject::FrameReset()
{
	//�`������Z�b�g
	instanceDrawNum = 0;

	for (int i = 0; i < draw_max_num; i++)
	{
		objInform.baseColor[i] = { 1,1,1,1 };
		objInform.matWorld[i] = XMMatrixIdentity();
	}
}