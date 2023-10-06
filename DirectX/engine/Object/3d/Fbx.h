#pragma once
#include "FbxModel.h"
#include "GraphicsPipelineManager.h"
#include "Texture.h"

class Camera;
class LightGroup;

class Fbx
{
protected:// �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private://�\���̐錾

	//�萔�o�b�t�@�p�f�[�^B0
	struct ConstBufferDataB0
	{
		XMFLOAT4 color;
		XMMATRIX viewproj; // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
		unsigned int isSkinning;//�X�L�j���O���s����
		unsigned int isBloom;//�u���[���̗L��
		unsigned int isToon;//�g�D�[���̗L��
		unsigned int isOutline;//�A�E�g���C���̗L��
	};

	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 baseColor;//�x�[�X�J���\
		float pad1; // �p�f�B���O
		XMFLOAT3 ambient; // �A���r�G���g�W��
		float pad2; // �p�f�B���O
		XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
		float metalness; // �����x(0 or 1)
		float specular; // ���ʔ��˓x
		float roughness; // �e��
		float alpha;	// �A���t�@
		//float pad[3];//�p�f�B���O
	};

private://�ÓI�����o�֐��֐�

	/// <summary>
	/// �p�C�v���C���ݒ�
	/// </summary>
	static void CreateGraphicsPipeline();

public://�ÓI�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <returns></returns>
	Fbx() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Fbx();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <param name="model">���f��</param
	static std::unique_ptr<Fbx> Create(FbxModel* model = nullptr);

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	/// <param name="camera">�J����</param>
	static void SetCamera(Camera* camera) { Fbx::camera = camera; }

	/// <summary>
	/// ���C�g�O���[�v�̃Z�b�g
	/// </summary>
	/// <param name="lightGroup">���C�g�O���[�v</param>
	static void SetLightGroup(LightGroup* lightGroup) { Fbx::lightGroup = lightGroup; }

	/// <summary>
	/// ���C�g�O���[�v�̃Z�b�g
	/// </summary>
	/// <param name="cubeTex">�L���[�u�}�b�v</param>
	static void SetCubeTex(Texture* cubetex) { Fbx::cubetex = cubetex; }

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// �������
	/// </summary>
	static void Finalize();

public:

	/// <summary>
	/// ����
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �}�e���A������萔�o�b�t�@�ɑ���
	/// </summary>
	void TransferMaterial();

private://�ÓI�����o�ϐ�

	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// �J����
	static Camera* camera;
	// ���C�g
	static LightGroup* lightGroup;
	//�p�C�v���C��
	static std::unique_ptr<GraphicsPipelineManager> pipeline;
	//�A�E�g���C���̐F
	static XMFLOAT4 outlineColor;
	//�A�E�g���C���̕�
	static float outlineWidth;
	//�L���[�u�}�b�v
	static Texture* cubetex;

private://�����o�ϐ�

	//���f��
	FbxModel* model = nullptr;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1;
	//���W
	XMFLOAT3 position = {};
	// ��]�p
	XMFLOAT3 rotation = {};
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// �F
	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld = {};
	//�}�e���A�����ω�������
	bool isTransferMaterial = false;
	//�x�[�X�J���\
	XMFLOAT3 baseColor = { 1,1,1 };
	//�����x(0 or 1)
	float metalness = 0.0f;
	//���ʔ��˓x
	float specular = 0.5f;
	//�e��
	float roughness = 0.0f;
	//�u���[���̗L��
	bool isBloom = false;
	//�g�D�[���̗L��
	bool isToon = false;
	//�A�E�g���C���̗L��
	bool isOutline = false;

public:

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns>��]</returns>
	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// �F�̎擾
	/// </summary>
	/// <returns>�F</returns>
	const XMFLOAT4& GetColor() { return color; }

	/// <summary>
	/// �傫���̎擾
	/// </summary>
	/// <returns>�傫��</returns>
	const XMFLOAT3& GetScale() { return scale; }

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	FbxModel* GetModel() { return model; }

	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// �x�[�X�J���[�擾
	/// </summary>
	/// <returns>�x�[�X�J���[</returns>
	const XMFLOAT3& GetBaseColor() { return baseColor; }

	/// <summary>
	/// �����x�擾
	/// </summary>
	/// <returns>�����x</returns>
	float GetMetalness() { return metalness; }

	/// <summary>
	/// ���ʔ��˓x�擾
	/// </summary>
	/// <returns>�����x</returns>
	float GetSpecular() { return specular; }

	/// <summary>
	/// �����x�擾
	/// </summary>
	/// <returns>�����x</returns>
	float GetRoughness() { return roughness; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// <summary>
	/// ��]�p�̐ݒ�
	/// </summary>
	/// <param name="rotation">��]�p</param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	/// <summary>
	/// �X�P�[���̐ݒ�
	/// </summary>
	/// <param name="position">�X�P�[��</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// �F�̐ݒ�
	/// </summary>
	/// <param name="color">�F</param>
	void SetColor(XMFLOAT4 color) { this->color = color; }

	/// <summary>
	/// �u���[���̃Z�b�g
	/// </summary>
	/// <param name="isBloom">�u���[���L->true / ��->false</param>
	void SetBloom(bool isBloom) { this->isBloom = isBloom; }

	/// <summary>
	/// �g�D�[���̃Z�b�g
	/// </summary>
	/// <param name="isToon">�g�D�[���L->true / ��->false</param>
	void SetToon(bool isToon) { this->isToon = isToon; }

	/// <summary>
	/// �A�E�g���C���̃Z�b�g
	/// </summary>
	/// <param name="isOutline">�A�E�g���C���L->true / ��->false</param>
	void SetOutline(bool isOutline) { this->isOutline = isOutline; }

	/// <summary>
	/// �A�E�g���C���̐F�Z�b�g
	/// </summary>
	/// <param name="outlineColor">��</param>
	static void SetOutlineColor(XMFLOAT4 outlineColor) { Fbx::outlineColor = outlineColor; }

	/// <summary>
	/// �A�E�g���C���̕��Z�b�g
	/// </summary>
	/// <param name="outlineWidth">��</param>
	static void SetOutlineWidth(float outlineWidth) { Fbx::outlineWidth = outlineWidth; }

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(FbxModel* model) { this->model = model; }

	/// <summary>
	/// �X�P�[���̐ݒ�
	/// </summary>
	/// <param name="position">�X�P�[��</param>
	void SetAnimation(bool isAnimation) { model->isAnimation = isAnimation; }

	/// <summary>
	/// �x�[�X�J���[�Z�b�g
	/// </summary>
	void SetBaseColor(const XMFLOAT3& baseColor) { 
		this->baseColor = baseColor;
		isTransferMaterial = true;
	}

	/// <summary>
	/// �����x�Z�b�g
	/// </summary>
	void SetMetalness(float metalness) { 
		this->metalness = metalness;
		isTransferMaterial = true;
	}

	/// <summary>
	/// ���ʔ��˓x�Z�b�g
	/// </summary>
	void SetSpecular(float specular) { 
		this->specular = specular;
		isTransferMaterial = true;
	}

	/// <summary>
	/// �����x�Z�b�g
	/// </summary>
	void SetRoughness(float roughness) { 
		this->roughness = roughness;
		isTransferMaterial = true;
	}
};