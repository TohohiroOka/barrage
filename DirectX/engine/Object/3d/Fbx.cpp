#include "Fbx.h"
#include "Camera.h"
#include "LightGroup.h"
#include "SafeDelete.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* Fbx::device = nullptr;
Camera* Fbx::camera = nullptr;
LightGroup* Fbx::lightGroup = nullptr;
ID3D12GraphicsCommandList* Fbx::cmdList = nullptr;
std::unique_ptr<GraphicsPipelineManager> Fbx::pipeline = nullptr;
Texture* Fbx::cubetex = nullptr;

Fbx::~Fbx()
{
	constBuffB0.Reset();
	constBuffB1.Reset();
}

void Fbx::CreateGraphicsPipeline()
{
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�{�[���ԍ�
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�{�[���e���x
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	pipeline = GraphicsPipelineManager::Create("Fbx",
		GraphicsPipelineManager::OBJECT_KINDS::FBX, inputLayout, _countof(inputLayout));
}

void Fbx::StaticInitialize(ID3D12Device* device)
{
	HRESULT result = S_FALSE;

	// �ď������`�F�b�N
	assert(!Fbx::device);

	// nullptr�`�F�b�N
	assert(device);

	Fbx::device = device;

	CreateGraphicsPipeline();

	FbxModel::StaticInitialize(device);
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

	//�}�e���A�����̎擾
	instance->baseColor = model->GetBaseColor();
	instance->metalness = model->GetMetalness();
	instance->specular = model->GetSpecular();
	instance->roughness = model->GetRoughness();

	instance->TransferMaterial();

	return std::unique_ptr<Fbx>(instance);
}

void Fbx::Update()
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ���[���h�s��̍���
	matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	const XMMATRIX& matViewProjection = camera->GetView() * camera->GetProjection();
	const XMFLOAT3& cameraPos = camera->GetEye();

	// �萔�o�b�t�@1�փf�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->viewproj = matViewProjection;
	constMap->world = matWorld;
	constMap->cameraPos = cameraPos;
	constMap->isSkinning = model->isSkinning;
	constMap->isBloom = isBloom;
	constMap->isToon = isToon;
	constMap->isOutline = isOutline;
	constBuffB0->Unmap(0, nullptr);

	if (isTransferMaterial)
	{
		TransferMaterial();
		isTransferMaterial = false;
	}

	model->Update();
}

void Fbx::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	Fbx::cmdList = cmdList;

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipeline->pipelineState.Get());

	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipeline->rootSignature.Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Fbx::PostDraw()
{
	// �R�}���h���X�g������
	Fbx::cmdList = nullptr;
}

void Fbx::Draw()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(Fbx::cmdList);

	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (model == nullptr) {
		return;
	}

	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	//�L���[�u�}�b�v�`��
	cmdList->SetGraphicsRootDescriptorTable(5, cubetex->descriptor->gpu);

	// ���C�g�̕`��
	lightGroup->Draw(cmdList, 3);

	// ���f���`��
	model->Draw(cmdList);
}

void Fbx::TransferMaterial()
{
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB1* constMap = nullptr;
	HRESULT result = constBuffB1->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->baseColor = baseColor;
		constMap->ambient = model->GetAmbient();
		constMap->diffuse = model->GetDiffuse();
		constMap->metalness = metalness;
		constMap->specular = specular;
		constMap->roughness = roughness;
		constMap->alpha = model->GetAlpha();
		constBuffB1->Unmap(0, nullptr);
	}
}

void Fbx::Finalize()
{
	FbxModel::Finalize();
	pipeline.reset();
}