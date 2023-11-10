#include "Fbx.h"
#include "Camera/Camera.h"
#include "Camera/LightCamera.h"
#include "Light/LightGroup.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace DirectX;

std::vector<GraphicsPipelineManager::DrawSet> Fbx::pipeline;
std::vector<GraphicsPipelineManager::DrawSet> Fbx::lightviewPipeline;

Fbx::~Fbx()
{
	constBuffB0.Reset();
	constBuffB1.Reset();
}

void Fbx::Initialize()
{
	HRESULT result = S_FALSE;
	// �萔�o�b�t�@1�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@Skin�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�V���h�E�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA_LIGHTVIEW_B0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffLightViewB0));
	assert(SUCCEEDED(result));
}

std::unique_ptr<Fbx> Fbx::Create(FbxModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Fbx* instance = new Fbx();

	//������
	instance->Initialize();

	//���f�����w�肳��Ă���΃Z�b�g����
	if (model) {
		instance->SetModel(model);
	}

	////�}�e���A�����̎擾
	//instance->baseColor = model->GetBaseColor();
	//instance->metalness = model->GetMetalness();
	//instance->specular = model->GetSpecular();
	//instance->roughness = model->GetRoughness();

	instance->TransferMaterial();

	return std::unique_ptr<Fbx>(instance);
}

void Fbx::Update(const float _motionBlendRate1, const float _motionBlendRate2)
{
	if (isTransferMaterial)
	{
		TransferMaterial();
		isTransferMaterial = false;
	}

	if (_motionBlendRate1 >= 2.0f) {
		model->Update(useAnimation);
	} else {
		model->Update(motionBlendModel, _motionBlendRate1, _motionBlendRate2, useAnimation);
	}

	//���f�����̂̈ړ��v�Z
	{
		// �X�P�[���A��]�A���s�ړ��s��̌v�Z
		DirectX::XMMATRIX matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

		XMFLOAT3 move = model->GetWorldMove(useAnimation);
		XMVECTOR matMove = XMVector3Transform({ move.x,move.y,move.z }, matRot);

		animationMove = XMFLOAT3{ matMove.m128_f32[0], matMove.m128_f32[1], matMove.m128_f32[2] };
		animationMove.y = 0.0f;
	}

	UpdateWorldMatrix();
	// �萔�o�b�t�@1�փf�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->color = { 1,1,1,1 };
	if (camera) {
		constMap->viewproj = camera->GetView() * camera->GetProjection();
		constMap->cameraPos = camera->GetEye();
	} else {
		constMap->viewproj = XMMatrixIdentity();
		constMap->cameraPos = {0.0f,0.0f,0.0f };
	}
	constMap->world = matWorld;
	if (lightCamera)
	{
		constMap->lightViewproj = lightCamera->GetView() * lightCamera->GetProjection();
	}
	else
	{
		constMap->lightViewproj = XMMatrixIdentity();
	}
	constMap->isShadowMap = isShadowMap;
	constMap->isSkinning = model->isSkinning;
	constMap->isBloom = isBloom;
	constMap->isToon = isToon;
	constMap->isOutline = isOutline;
	constMap->isLight = isLight;
	constMap->outlineColor = outlineColor;
	constBuffB0->Unmap(0, nullptr);

	//�萔�o�b�t�@�ւ̃f�[�^�]��(�����J�������_)
	CONST_BUFFER_DATA_LIGHTVIEW_B0* constMapLightView = nullptr;
	if (SUCCEEDED(constBuffLightViewB0->Map(0, nullptr, (void**)&constMapLightView))) {
		if (lightCamera)
		{
			constMapLightView->viewproj = lightCamera->GetView() * lightCamera->GetProjection();
			constMapLightView->cameraPos = lightCamera->GetEye();
		}
		else
		{
			constMapLightView->viewproj = XMMatrixIdentity();
			constMapLightView->cameraPos = { 0,0,0 };
		}

		constMapLightView->world = matWorld;
		constMapLightView->isSkinning = model->isSkinning;
		constBuffLightViewB0->Unmap(0, nullptr);
	}
}

void Fbx::Draw(const DrawMode _drawMode)
{
	// nullptr�`�F�b�N
	assert(device);
	assert(ObjectBase::cmdList);

	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (model == nullptr) {
		return;
	}

	int modeNum = int(_drawMode);

	Base3D::Draw(pipeline[modeNum]);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(5, lightDepthTexture->descriptor->gpu);

	// ���C�g�̕`��
	light->Draw(cmdList, 2);

	// ���f���`��
	model->Draw(cmdList);
}

void Fbx::DrawLightView()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(ObjectBase::cmdList);

	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (model == nullptr) {
		return;
	}

	Base3D::Draw(lightviewPipeline[0]);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffLightViewB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	// ���C�g�̕`��
	light->Draw(cmdList, 2);

	// ���f���`��
	model->Draw(cmdList);
}

void Fbx::BoneDraw(const DrawMode _drawMode)
{
	for (auto& i : boneObjectInfo) {
		boneObject[i.instanceName]->DrawInstance(i.matWorld * model->GetBornMatWorld(i.boneName), { 1,1,1,1 });
	}

	for (auto& i : boneObject) {
		i.second->Draw();
	}
}

void Fbx::TransferMaterial()
{
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB1* constMap = nullptr;
	HRESULT result = constBuffB1->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->baseColor = { 1,1,1 };
		constMap->ambient = { 0.5f ,0.5f ,0.5f };
		constMap->diffuse = { 0.5f ,0.5f ,0.5f };
		constMap->metalness = 0.5f;
		constMap->specular = 0.5f;
		constMap->roughness = 0.5f;
		constMap->alpha = 1.0f;
		constBuffB1->Unmap(0, nullptr);
	}
}

void Fbx::SetBoneObject(const std::string& _boneName, const std::string& _modelName,
	Model* _model, const XMMATRIX& _matWorld)
{
	int modelNuber = 0;
	if (!boneObject[_modelName]) {
		boneObject[_modelName] = InstanceObject::Create(_model);
	}

	BoneObjectInfo add;
	add.boneName = _boneName;
	add.instanceName = _modelName;
	add.matWorld = _matWorld;
	boneObjectInfo.emplace_back(add);
}

void Fbx::FrameReset()
{
	for (auto& i = boneObject.begin(); i != boneObject.end(); i++) {
		i->second->FrameReset();
	}
}
