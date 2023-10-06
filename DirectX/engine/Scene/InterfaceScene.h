#pragma once
#include <DirectXMath.h>
#include "Camera/Camera.h"
#include "Audio/Audio.h"

//�I�u�W�F�N�g�n
#include "Object/3d/Object3d.h"
#include "Object/2d/Sprite.h"
#include "Particle/Emitter.h"
#include "Light/LightGroup.h"
#include "Math/Easing/Easing.h"
#include "Object/3d/HeightMap.h"
#include "Object/3d/PrimitiveObject3D.h"
#include "Object/3d/InstanceObject.h"

#include "Pipeline/GraphicsPipelineManager.h"

//�v�Z�V�F�[�_�[
#include "Pipeline/ComputeShaderManager.h"

class InterfaceScene
{
protected:// �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	virtual ~InterfaceScene() {};

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void CameraUpdate(const int _cameraNum, Camera* camera) {};

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="_cameraNum">�J�����ԍ�</param>
	virtual void Draw(const int _cameraNum) {};

	/// <summary>
	/// �|�X�g�G�t�F�N�g�������Ȃ��`��
	/// </summary>
	virtual void NonPostEffectDraw(const int _cameraNum){};

	/// <summary>
	/// ���
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// imgui�̕\��
	/// </summary>
	virtual void ImguiDraw() {};

	/// <summary>
	/// �R���s���[�g�V�F�[�_�[����̌v�Z���ʎ擾
	/// </summary>
	virtual void GetConstbufferNum() {};

	/// <summary>
	/// �R�}���h���X�g�̃Z�b�g
	/// </summary>
	/// <param name="inCmdList">�R�}���h���X�g</param>
	void SetCmdList(ID3D12GraphicsCommandList* inCmdList) { cmdList = inCmdList; }

	/// <summary>
	/// ���C�g�̃Z�b�g
	/// </summary>
	/// <param name="light">���C�g�N���X�̃C���X�^���X</param>
	void SetLight(LightGroup* light) { this->light = light; }

	/// <summary>
	/// 
	/// </summary>
	virtual void FrameReset() {};

protected:

	//�R�}���h���X�g
	ID3D12GraphicsCommandList* cmdList = nullptr;
	//���C�g
	LightGroup* light = nullptr;
};