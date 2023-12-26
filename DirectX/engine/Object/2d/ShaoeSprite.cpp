#include "ShaoeSprite.h"
#include "WindowApp.h"
#include <cassert>

using namespace DirectX;

std::vector<GraphicsPipelineManager::DrawSet> ShaoeSprite::pipeline;
XMMATRIX ShaoeSprite::matProjection;

ShaoeSprite::~ShaoeSprite()
{
	constBuff.Reset();
}

void ShaoeSprite::StaticInitialize()
{
	// �ˉe�s��v�Z
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, float(WindowApp::GetWindowWidth()),
		float(WindowApp::GetWindowHeight()), 0.0f,
		0.0f, 1.0f);
}

std::unique_ptr<ShaoeSprite> ShaoeSprite::Create(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT4& _color)
{
	// Sprite�̃C���X�^���X�𐶐�
	ShaoeSprite* instance = new ShaoeSprite();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize(_name, _position, _color);

	//�X�V
	instance->Update();

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<ShaoeSprite>(instance);
}

void ShaoeSprite::Initialize(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT4& _color)
{
	texture = std::make_unique<TextureManager>(_name);
	position = _position;
	color = _color;

	// �萔�o�b�t�@�̐���
	HRESULT result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	// �萔�o�b�t�@�Ƀf�[�^�]��
	CONST_BUFFER_DATA* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}
}

void ShaoeSprite::SetModel(Model* _model)
{
	HRESULT result = S_FALSE;

	std::vector<Mesh::VERTEX> modelVertex = _model->GetMeshes()[0]->GetVertices();
	std::vector<SHAOE_VERTEX> vertices;

	//���_�z��̑傫��
	size_t vertSize = modelVertex.size();
	vertices.resize(vertSize);

	for (int i = 0; i < vertSize; i++) {
		vertices[i].pos = modelVertex[i].pos;
		vertices[i].uv = modelVertex[i].uv;
	}

	vertBuff.Reset();
	indexBuff.Reset();

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	const UINT sizeVB = static_cast<UINT>(sizeof(SHAOE_VERTEX) * vertSize);

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
	SHAOE_VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̐���
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(SHAOE_VERTEX);

	std::vector<unsigned long> indices = _model->GetMeshes()[0]->GetIndices();
	indexNum = int(indices.size());
	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	const UINT sizeIB = static_cast<UINT>(sizeof(unsigned long) * indexNum);

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

void ShaoeSprite::Update()
{
	// ���[���h�s��̍X�V
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixScaling(scale.x, scale.y, 0.0f);
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (FAILED(result)) { assert(0); }
	constMap->color = this->color;
	constMap->mat = this->matWorld * matProjection;	// �s��̍���
	this->constBuff->Unmap(0, nullptr);
}

void ShaoeSprite::Draw(const DrawMode _drawMode)
{
	//�e�N�X�`�����Z�b�g����Ă��Ȃ���Δ�����
	if (texture->GetTexture() == "") { return; }

	int modeNum = int(_drawMode)+2;

	ObjectBase::Draw(pipeline[modeNum]);

	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, texture->GetDescriptor()->gpu);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced(UINT(indexNum), 1, 0, 0, 0);
}

void ShaoeSprite::Draw(const Texture* _tex, const DrawMode _drawMode)
{
	int modeNum = int(_drawMode);

	ObjectBase::Draw(pipeline[modeNum]);

	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, _tex->descriptor->gpu);
	// �`��R�}���h
	cmdList->DrawIndexedInstanced(UINT(indexNum), 1, 0, 0, 0);
}