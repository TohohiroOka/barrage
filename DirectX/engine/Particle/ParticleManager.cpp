#include "ParticleManager.h"
#include <DirectXTex.h>
#include"Camera/Camera.h"

using namespace DirectX;

Camera* ParticleManager::camera = nullptr;
std::vector<GraphicsPipelineManager::DrawSet> ParticleManager::pipeline;
std::map<std::string, ParticleManager::INFORMATION> ParticleManager::texture;
XMMATRIX ParticleManager::matBillboard = XMMatrixIdentity();
XMMATRIX ParticleManager::matBillboardY = XMMatrixIdentity();

ParticleManager::~ParticleManager()
{
	vertBuff.Reset();
	constBuff.Reset();
}

//XMFLOAT3���m�̉��Z����
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& _lhs, const DirectX::XMFLOAT3& _rhs)
{
	XMFLOAT3 result;
	result.x = _lhs.x + _rhs.x;
	result.y = _lhs.y + _rhs.y;
	result.z = _lhs.z + _rhs.z;

	return result;
}

void ParticleManager::LoadTexture(const std::string& _keepName, const std::string& _filename, bool _isDelete)
{
	// nullptr�`�F�b�N
	assert(device);

	//�����L�[������΃G���[���o��
	assert(!texture.count(_keepName));

	//�e�N�X�`���ǂݍ���
	texture[_keepName].instance = Texture::Create(_filename);
	texture[_keepName].isDelete = _isDelete;
}

void ParticleManager::Initialize()
{
	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }
}

std::unique_ptr<ParticleManager> ParticleManager::Create(const std::string& _name)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	ParticleManager* instance = new ParticleManager();

	instance->name = _name;

	// ������
	instance->Initialize();

	return std::unique_ptr<ParticleManager>(instance);
}

void ParticleManager::Add(int _maxFrame, const XMFLOAT3& _position, const XMFLOAT3& _velocity,
	const XMFLOAT3& _accel, float _startScale, float _endScale, const XMFLOAT4& _startColor, const XMFLOAT4& _endColor)
{
	//���X�g�ɗv�f��ǉ�
	particle.emplace_front();
	//�ǉ������v�f�̎Q��
	PARTICLE& p = particle.front();
	//�l�̃��Z�b�g
	p.position = _position;
	p.velocity = _velocity;
	p.accel = _accel;
	p.numFrame = _maxFrame;
	p.scale = _startScale;
	p.startScale = _startScale;
	p.endScale = _endScale;
	p.color = _startColor;
	p.startColor = _startColor;
	p.endColor = _endColor;
}

XMMATRIX ParticleManager::UpdateViewMatrix()
{
	//���ӓ_
	XMVECTOR eyePosition = XMLoadFloat3(&camera->GetEye());
	//���ӓ_���W
	XMVECTOR targetPosition = XMLoadFloat3(&camera->GetTarget());
	//�����
	XMVECTOR upVector = XMLoadFloat3(&camera->GetUp());

	//�J����Z��
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3Equal(upVector, XMVectorZero()));

	//�x�N�g�����K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//�J������X��
	XMVECTOR cameraAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//�x�N�g�����K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//�J������Y��
	XMVECTOR cameraAxisY;
	//Y����X����Z���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	//�x�N�g�����K��
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	//�J������]�s��
	XMMATRIX matCameraRot;
	//�J�������W�n
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//�]�u
	XMMATRIX matView = XMMatrixTranspose(matCameraRot);

	//���_���W��-1���|�������W
	XMVECTOR reverssEyePosition = XMVectorNegate(eyePosition);
	//�J�����̈ʒu���烏�[���h���_�ւ̕ϊ�
	XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverssEyePosition);
	XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverssEyePosition);
	XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverssEyePosition);
	//��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//�r���[�ϊ��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

#pragma region �������r���{�[�h�s��̌v�Z
	//�r���{�[�h�s��
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y�����̃r���{�[�h�s��̌v�Z
	//�J����X��,Y���AZ��
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X���͋���
	ybillCameraAxisX = cameraAxisX;
	//Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross(cameraAxisY, cameraAxisX);
	;
	//Y�����r���{�[�h�s��
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

	return matView;
}

void ParticleManager::Update()
{
	HRESULT result;

	//�\�����Ԃ����߂����p�[�e�B�N�����폜
	particle.remove_if([](PARTICLE& x) {
		return x.frame >= x.numFrame;
		}
	);

	//�S�p�[�e�B�N���X�V
	for (std::forward_list<PARTICLE>::iterator it = particle.begin();
		it != particle.end(); it++) {
		//�o�߃t���[�������J�E���g
		it->frame++;
		//���x�ɉ����x�����Z
		it->velocity = it->velocity + it->accel;
		//���x�ɂ��ړ�
		it->position = it->position + it->velocity;
		//�傫���̕ύX
		it->scale = it->scale - (it->startScale - it->endScale) / it->numFrame;
		//�F�̕ύX
		it->color.x = it->color.x - (it->startColor.x - it->endColor.x) / it->numFrame;//��
		it->color.y = it->color.y - (it->startColor.y - it->endColor.y) / it->numFrame;//��
		it->color.z = it->color.z - (it->startColor.z - it->endColor.z) / it->numFrame;//��
		it->color.w = it->color.w - (it->startColor.w - it->endColor.w) / it->numFrame;//���x
	}

	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	int count = 0;
	if (SUCCEEDED(result)) {
		//�p�[�e�B�N���̏���������f
		for (std::forward_list<PARTICLE>::iterator it = particle.begin();
			it != particle.end(); it++) {
			if (count >= vertexCount) { break; }
			//���W
			vertMap->pos = it->position;
			//�X�P�[��
			vertMap->scale = it->scale;
			//�F
			vertMap->color = it->color;
			//���̒��_��
			vertMap++;
			count++;
		}
	}

	// �萔�o�b�t�@�փf�[�^�]��
	CONST_BUFFER_DATA* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = UpdateViewMatrix() * camera->GetProjection();// �s��̍���
	constMap->matBillboard = matBillboard;// �s��̍���
	constMap->isBloom = isBloom;
	constBuff->Unmap(0, nullptr);
}

void ParticleManager::Draw(const DrawMode _drawMode)
{
	int modeNum = int(_drawMode);

	ObjectBase::Draw(pipeline[modeNum]);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, texture[name].instance->descriptor->gpu);

	//�`��R�}���h
	cmdList->DrawInstanced((UINT)std::distance(particle.begin(), particle.end()), 1, 0, 0);
}

void ParticleManager::ParticlAllDelete()
{
	particle.clear();
}

void ParticleManager::SceneFinalize()
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

void ParticleManager::Finalize()
{
	for (auto itr = texture.begin(); itr != texture.end(); ++itr)
	{
		(*itr).second.instance.reset();
	}
	texture.clear();
}