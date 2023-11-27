#pragma once
#include "Scene/InterfaceScene.h"
#include "engine/camera/DebugCamera.h"
#include "engine/Camera/LightCamera.h"

#include "ui/ChoiceEmphasisDrawer.h"
#include "system/ActionInputConfig.h"

#include <memory>

class TitleScene : public InterfaceScene
{
private:// �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMINT3 = DirectX::XMINT3;

public:

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
	//�I��
	enum class PLAYER_SELECT
	{
		SELECT_STARTGAME,
		SELECT_CONFIG,
		SELECT_EXIT,
	};
	PLAYER_SELECT selecting = PLAYER_SELECT::SELECT_STARTGAME;

	bool isPressed = false;
	bool isSelected = false;
	
	bool isSceneChangeWait = false;

	ChoiceEmphasisDrawer choiceDrawer;
	//�\�����W�n
	const float OPTIONS_START_Y = 550.f;
	const float OPTIONS_DISTANCE_Y = 50.f;
	//�J����
	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<LightCamera> lightCamera;
	//�X�v���C�g
	std::unique_ptr<Sprite> titleLogoSprite;
	std::unique_ptr<Sprite> pressAnyButtonSprite;
	std::unique_ptr<Sprite> gamestartSprite;
	std::unique_ptr<Sprite> configSprite;
	std::unique_ptr<Sprite> exitgameSprite;
	//����
	const int FADEIN_FRAME = 150;
	int fadeinFrame = 0;
	//�R���t�B�O
	std::unique_ptr<ActionInputConfig> actionInputConfig;
	bool isConfigMode = false;

	//���͔���p
	bool IsUp();
	bool IsDown();
	bool IsEnter();

};

