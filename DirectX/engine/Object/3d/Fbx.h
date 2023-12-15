#pragma once
#include "FbxModel.h"
#include "Base3D.h"
#include "InstanceObject.h"

class Camera;
class LightGroup;

class Fbx : public Base3D
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
		XMFLOAT4 color; // �r���[�v���W�F�N�V�����s��
		XMMATRIX viewproj; // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
		XMMATRIX lightViewproj;	//���C�g�r���[�v���W�F�N�V�����s��
		unsigned int isShadowMap;	//�e��t���邩
		unsigned int isSkinning;//�X�L�j���O���s����
		unsigned int isBloom;//�u���[���̗L��
		unsigned int isToon;//�g�D�[���̗L��
		unsigned int isOutline;//�A�E�g���C���̗L��
		unsigned int isLight;//���C�g�̗L��
		XMFLOAT3 outlineColor;//�A�E�g���C���̐F
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

	//�e�p
	struct  CONST_BUFFER_DATA_LIGHTVIEW_B0
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		XMFLOAT3 cameraPos;	//�J�������W(���[���h���W)
		unsigned int isSkinning;//�X�L�j���O���s����
	};

	struct BoneObjectInfo {
		bool isDraw;
		std::string boneName;//�Z�b�g����{�[����
		XMMATRIX matWorld;//�Z�b�g�����I�u�W�F�N�g�̍s��i���̃I�u�W�F�N�g�̂݁j
		XMMATRIX attachWorld;//�Z�b�g�����I�u�W�F�N�g�̍s��i�e�܂ߌv�Z�ς݁j
		std::string instanceName;//�Z�b�g����I�u�W�F�N�g��
	};

public://�ÓI�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <returns></returns>
	Fbx();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Fbx();

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <param name="model">���f��</param
	static std::unique_ptr<Fbx> Create(FbxModel* model = nullptr);

public:

	/// <summary>
	/// ����
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="_motionBlendRate1">���݂̃��f���̔䗦</param>
	/// <param name="_motionBlendRate2">�u�����h���郂�f���̔䗦</param>
	void Update(const float _motionBlendRate1 = 2.0f, const float _motionBlendRate2 = 2.0f);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const DrawMode _drawMode = DrawMode::alpha);

	/// <summary>
	/// �e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	void DrawLightView();

	/// <summary>
	/// �`��
	/// </summary>
	void BoneDraw(const DrawMode _drawMode = DrawMode::alpha);

	/// <summary>
	/// �`��
	/// </summary>
	void BoneDrawLightView();

	/// <summary>
	/// �}�e���A������萔�o�b�t�@�ɑ���
	/// </summary>
	void TransferMaterial();

	/// <summary>
	/// bone�p���f���Z�b�g
	/// </summary>
	/// <param name="_boneName">�{�[����</param>
	/// <param name="_modelName">���f����</param>
	/// <param name="_getObjectName">�Z�b�g�����I�u�W�F�N�g�̍s����擾����ۂ̖��O</param>
	/// <param name="_model">���f��</param>
	/// <param name="_matWorld">���[���h�s��</param>
	std::string SetBoneObject(const std::string& _boneName, const std::string& _modelName, Model* _model = nullptr,
		const XMMATRIX& _matWorld = DirectX::XMMatrixIdentity(), bool _isDraw = true, const std::string& _getObjectName = "null");

	void ChangeBoneObjectInfo(const std::string& _name, const XMMATRIX& _matWorld = DirectX::XMMatrixIdentity());

	void FrameReset();

	void AnimationReset() {
		model->AnimationReset(useAnimation);
		isAnimationEnd=false;
	}

private://�����o�ϐ�

	//�p�C�v���C�����
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;
	static std::vector<GraphicsPipelineManager::DrawSet> lightviewPipeline;

	//���f���`��
	bool isModelDraw;
	//���f��
	FbxModel* model = nullptr;
	//motionblend�p
	FbxModel* motionBlendModel = nullptr;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1;
	//�e�p�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffLightViewB0;
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

	//�g�p�A�j���[�V�����ԍ��w��
	int useAnimation;
	//�A�j���[�V�����̈ړ�����
	XMFLOAT3 animationMove;
	//���[�v�A�j���[�V����
	bool isRoop = true;
	//�A�j���[�V�������I��������
	bool isAnimationEnd;
	//�{�[���`��
	bool isBoneDraw;
	std::unordered_map<std::string, BoneObjectInfo> boneObjectInfo;
	std::unordered_map<std::string, std::unique_ptr<InstanceObject>> boneObject;

public:

	FbxModel* GetModel() { return model; }
	const XMMATRIX& GetMatWorld() { return matWorld; }
	const XMFLOAT3& GetBaseColor() { return baseColor; }
	float GetMetalness() { return metalness; }
	float GetSpecular() { return specular; }
	float GetRoughness() { return roughness; }
	int GetUseAnimation() { return useAnimation; }
	InstanceObject* GetBrneObject(const std::string& _name) { return boneObject[_name].get(); }
	XMFLOAT3 GetModelMove() { return animationMove; }
	XMFLOAT3 GetAttachPos(const std::string& _name) {
		if (boneObjectInfo.find(_name) == boneObjectInfo.end()) { return {}; }
		return XMFLOAT3{ boneObjectInfo[_name].attachWorld.r[3].m128_f32[0],
		boneObjectInfo[_name].attachWorld.r[3].m128_f32[1],
		boneObjectInfo[_name].attachWorld.r[3].m128_f32[2] };
	}
	bool GetIsAnimationEnd() { return isAnimationEnd; }
	void SetIsModelDraw(bool _isModelDraw) { isModelDraw = _isModelDraw; }
	void SetModel(FbxModel* model) { this->model = model; }
	void SetMotionBlendModel(FbxModel* _model) { motionBlendModel = _model; }
	void SetAnimation(bool isAnimation) { model->isAnimation = isAnimation; }
	void SetIsBoneDraw(bool _isBoneDraw) { isBoneDraw = _isBoneDraw; }
	void SetIsRoop(const bool _isRoop) { isRoop = _isRoop; }
	void SetBaseColor(const XMFLOAT3& baseColor) {
		this->baseColor = baseColor;
		isTransferMaterial = true;
	}
	void SetMetalness(float metalness) { 
		this->metalness = metalness;
		isTransferMaterial = true;
	}
	void SetSpecular(float specular) { 
		this->specular = specular;
		isTransferMaterial = true;
	}
	void SetRoughness(float roughness) { 
		this->roughness = roughness;
		isTransferMaterial = true;
	}
	void SetUseAnimation(int _useAnimation) { useAnimation = _useAnimation; }
	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { Fbx::pipeline = _pipeline; }
	static void SetLightviewPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { Fbx::lightviewPipeline = _pipeline; }
};