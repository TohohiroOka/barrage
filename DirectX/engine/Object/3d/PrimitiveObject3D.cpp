#include "PrimitiveObject3D.h"
#include "Camera/Camera.h"
#include "SafeDelete.h"

#include <vector>

using namespace DirectX;

std::vector<GraphicsPipelineManager::DrawSet> PrimitiveObject3D::pipeline;

std::unique_ptr<PrimitiveObject3D> PrimitiveObject3D::Create()
{
	//�C���X�^���X�𐶐�
	PrimitiveObject3D* instance = new PrimitiveObject3D();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	instance->Initialize();

	//���[���h�s�񐶐�
	instance->UpdateWorldMatrix();

	return std::unique_ptr<PrimitiveObject3D>(instance);
}

PrimitiveObject3D::~PrimitiveObject3D()
{
	DeleteCollider();
}

void PrimitiveObject3D::Initialize()
{
	HRESULT result = S_FALSE;

	if (vertices.size() != 0) {
		VertexInit();
	}
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));
	if (FAILED(result)) { assert(0); }
}

void PrimitiveObject3D::Update()
{
	//�萔�o�b�t�@�Ƀf�[�^��]��
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	if (SUCCEEDED(result)) {
		constMap->color = baseColor;
		constMap->matWorld = matWorld;
		if (camera != nullptr)
		{
			constMap->viewproj = camera->GetView() * camera->GetProjection();
		} else {
			constMap->viewproj = XMMatrixIdentity();
		}
		constBuffB0->Unmap(0, nullptr);
	}
}

void PrimitiveObject3D::Draw(const DrawMode _drawMode)
{
	Update();

	int modeNum = int(_drawMode);

	ObjectBase::Draw(pipeline[modeNum]);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
}

void PrimitiveObject3D::VertexInit()
{
	HRESULT result = S_FALSE;

	//���_�z��̑傫��
	size_t vertSize = vertices.size();

	vertBuff.Reset();
	indexBuff.Reset();

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	const UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * vertSize);

	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̐���
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(XMFLOAT3);

	//�g���C���f�b�N�X�̍쐬
	indices.resize(vertSize);
	for (int i = 0; i < static_cast<int>(vertSize); i++)
	{
		indices[i] = i;
	}

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	const UINT sizeIB = static_cast<UINT>(sizeof(unsigned long) * indices.size());

	//�C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB), // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) { assert(0); }

	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned long* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}
	
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeIB;
}