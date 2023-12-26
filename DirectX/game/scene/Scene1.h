#pragma once
#include "Scene/InterfaceScene.h"
#include "Math/Vector3.h"
#include "engine/camera/DebugCamera.h"
#include "player/Player.h"
#include "game/camera/GameCamera.h"
#include "engine/Camera/LightCamera.h"
#include "../enemy/boss1/Boss1.h"
#include "../field/Field.h"
#include "game/ui/GameOver.h"
#include "system/ActionInputConfig.h"
#include "game/cutscene/Boss1Defeat.h"
#include "ui/LockonUI.h"
#include "../game/effect/ScreenCut.h"

class Scene1 : public InterfaceScene
{
private:// �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMINT3 = DirectX::XMINT3;

public:

	Scene1(){};
	~Scene1(){};

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
	///	���
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// imgui�̕\��
	/// </summary>
	void ImguiDraw() override;

	/// <summary>
	/// 
	/// </summary>
	void FrameReset() override;

	void CollisionCheck();

private:
	bool isNormalCamera = true;

	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<GameCamera> camera;
	std::unique_ptr<LightCamera> lightCamera;

	std::unique_ptr<Sprite> sprite;

	std::unique_ptr<Player> player;

	std::unique_ptr<ScreenCut> screenCut;

	//�n�`
	std::unique_ptr<Field> field;

	//�{�X
	std::unique_ptr<Boss1>boss;
	//�Q�[���I�[�o�[UI
	std::unique_ptr<GameOver> gameoverUi;

	//���S���o
	std::unique_ptr<Boss1Defeat> defeatDirection;

	//�s�����͐ݒ�
	std::unique_ptr<ActionInputConfig> actionInputConfig;

	//���b�N�I��
	std::unique_ptr<LockonUI> lockonUI;

	//�s�����͐ݒ蒆��
	bool isInputConfigMode = false;

	//�q�b�g�X�g�b�v�t���[����
	int hitStopFrame = 5;

	//�V�[���J�ڑҋ@����
	bool isSceneChangeWait = false;
};