#include "Sprite.h"
#include "WindowApp.h"
#include <cassert>

using namespace DirectX;

std::vector<GraphicsPipelineManager::DrawSet> Sprite::pipeline;
std::map<std::string, Sprite::INFORMATION> Sprite::texture;
XMMATRIX Sprite::matProjection;

Sprite::~Sprite()
{
	vertBuff.Reset();
	constBuff.Reset();
}

void Sprite::StaticInitialize()
{
	// �ˉe�s��v�Z
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, float(WindowApp::GetWindowWidth()),
		float(WindowApp::GetWindowHeight()), 0.0f,
		0.0f, 1.0f);

	Sprite::LoadTexture("debugfont", "Resources/LetterResources/debugfont.png", false);
}

void Sprite::LoadTexture(const std::string& _keepName, const std::string& _filename, bool _isDelete)
{
	// nullptr�`�F�b�N
	assert(device);

	//�����L�[������΃G���[���o��
	assert(!texture.count(_keepName));

	//�e�N�X�`���ǂݍ���
	texture[_keepName].instance = Texture::Create(_filename);
	texture[_keepName].isDelete = _isDelete;
}

std::unique_ptr<Sprite> Sprite::Create(const std::string& _name)
{
	// Sprite�̃C���X�^���X�𐶐�
	Sprite* instance = new Sprite();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize(_name, { 0.0f,0.0f });

	//�X�V
	instance->Update();

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<Sprite>(instance);
}

void Sprite::Initialize(const std::string& _name, const XMFLOAT2& _anchorpoint, bool _isFlipX, bool _isFlipY)
{
	this->name = _name;
	this->anchorpoint = _anchorpoint;
	this->isFlipX = _isFlipX;
	this->isFlipY = _isFlipY;

	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VERTEX) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	if (name != "") {
		TransferVertices();
	} else {
		TransferVerticesNoTex();
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VERTEX) * 4;
	vbView.StrideInBytes = sizeof(VERTEX);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
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

void Sprite::Update()
{
	// ���[���h�s��̍X�V
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	//���_�o�b�t�@�ɔ��f
	if (name != "") {
		TransferVertices();
	} else {
		TransferVerticesNoTex();
	}

	// �萔�o�b�t�@�Ƀf�[�^�]��
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (FAILED(result)) { assert(0); }
	constMap->color = this->color;
	constMap->mat = this->matWorld * matProjection;	// �s��̍���
	this->constBuff->Unmap(0, nullptr);
}

void Sprite::Draw(const DrawMode _drawMode)
{
	int modeNum = int(_drawMode);

	ObjectBase::Draw(pipeline[modeNum]);

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, texture[name].instance->descriptor->gpu);

	// �`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::Draw(const Texture* _tex, const DrawMode _drawMode)
{
	int modeNum = int(_drawMode);

	ObjectBase::Draw(pipeline[modeNum]);

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, _tex->descriptor->gpu);
	// �`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.x) * size.y;
	float bottom = (1.0f - anchorpoint.x) * size.y;
	if (isFlipX)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	VERTEX vertices[vertNum];

	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	// �e�N�X�`�����擾
	if (texture[name].instance->texBuffer)
	{
		D3D12_RESOURCE_DESC resDesc = texture[name].instance->texBuffer->GetDesc();

		float texLeft = texLeftTop.x / resDesc.Width;
		float texRight = (texLeftTop.x + texSize.x) / resDesc.Width;
		float texTop = texLeftTop.y / resDesc.Height;
		float texBottom = (texLeftTop.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { texLeft,	texBottom }; // ����
		vertices[LT].uv = { texLeft,	texTop }; // ����
		vertices[RB].uv = { texRight,	texBottom }; // �E��
		vertices[RT].uv = { texRight,	texTop }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

void Sprite::TransferVerticesNoTex()
{
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.x) * size.y;
	float bottom = (1.0f - anchorpoint.x) * size.y;
	if (isFlipX)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	VERTEX vertices[vertNum];

	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	vertices[LB].uv = { 0,	1 }; // ����
	vertices[LT].uv = { 0,	0 }; // ����
	vertices[RB].uv = { 1,	1 }; // �E��
	vertices[RT].uv = { 1,	0 }; // �E��

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

void Sprite::SceneFinalize()
{
	for (auto itr = texture.begin(); itr != texture.end(); ++itr) {
		if ((*itr).second.isDelete)
		{
			(*itr).second.instance.reset();
			auto deleteItr = itr;
			itr--;
			texture.erase(deleteItr);
			if (itr == texture.end()) { break; }
		}
	}
}

void Sprite::Finalize()
{
	for (auto itr = texture.begin(); itr != texture.end(); ++itr)
	{
		(*itr).second.instance.reset();
	}
	texture.clear();
}