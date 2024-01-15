#pragma once
#include "Scene/InterfaceScene.h"
#include "engine/camera/DebugCamera.h"
#include "engine/Camera/LightCamera.h"

#include "system/ActionInputConfig.h"
#include "camera/TitleCamera.h"
#include "player/Player.h"
#include "field/Field.h"

#include <memory>

class TutorialScene : public InterfaceScene
{
private:// �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMINT3 = DirectX::XMINT3;

private: //enum
	/// <summary>
	/// �`���[�g���A���̗���t�F�[�Y
	/// </summary>
	enum class TutorialPhase
	{
		JUMP_
	};

public:

	TutorialScene() = default;
	~TutorialScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	///	�X�V
	/// </summary>
	void Update()override;

	/// <summary>
	///	�`��
	/// </summary>
	/// <param name="_cameraNum">�J�����ԍ�</param>
	void Draw(const int _cameraNum) override;

	/// <summary>
	///	�`��
	/// </summary>
	/// <param name="_cameraNum">�J�����ԍ�</param>
	void DrawLightView(const int _cameraNum) override;

	/// <summary>
	/// �|�X�g�G�t�F�N�g�������Ȃ��`��
	/// </summary>
	void NonPostEffectDraw(const int _cameraNum) override;

	/// <summary>
	/// imgui�̕\��
	/// </summary>
	void ImguiDraw() override;

	/// <summary>
	/// ���t���[�����Z�b�g
	/// </summary>
	void FrameReset() override;

	/// <summary>
	/// �����蔻��
	/// </summary>
	void CollisionCheck();

private:
	//�J����
	bool isNormalCamera = true;
	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<TitleCamera> camera;
	std::unique_ptr<LightCamera> lightCamera;

	//�X�v���C�g

	//�n�`
	std::unique_ptr<Field> field;
	//�v���C���[
	std::unique_ptr<Player> player;

	//�s�����͐ݒ�
	std::unique_ptr<ActionInputConfig> actionInputConfig;
	bool isInputConfigMode = false;

	//�V�[���J�ڑҋ@����
	bool isSceneChangeWait = false;
};

