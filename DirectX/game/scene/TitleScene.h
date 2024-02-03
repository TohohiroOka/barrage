#pragma once
#include "Scene/InterfaceScene.h"
#include "engine/Camera/LightCamera.h"

#include "ui/ChoiceEmphasisDrawer.h"
#include "system/ActionInputConfigKey.h"
#include "system/ActionInputConfigPad.h"
#include "camera/TitleCamera.h"
#include "player/Player.h"
#include "field/Field.h"
#include "titleObject/Portal.h"
#include "titleObject/SpeechBubble.h"
#include "ui/PressSelectButtonUI.h"

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

	TitleScene() = default;
	~TitleScene();

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

	/// <summary>
	/// �|�[�^���ɓ��邩�`�F�b�N
	/// </summary>
	void IntoPortalCheck();

	/// <summary>
	/// �|�[�^���ɓ���s�����J�n
	/// </summary>
	void IntoPortalStart();

	/// <summary>
	/// �|�[�^���ɓ���s��
	/// </summary>
	void IntoPortal();

private:
	//�J����
	std::unique_ptr<TitleCamera> camera;
	std::unique_ptr<LightCamera> lightCamera;

	//�X�v���C�g
	std::unique_ptr<Sprite> titleLogoSprite;	//�^�C�g�����S
	std::unique_ptr<Sprite> intoPortalSprite;	//�u����v

	//�n�`
	std::unique_ptr<Field> field;
	//�v���C���[
	std::unique_ptr<Player> player;
	//�V�[���ύX�|�[�^��
	std::array<std::unique_ptr<Portal>, 3> portals;
	//�|�[�^�������p�����o���I�u�W�F�N�g
	std::array<std::unique_ptr<SpeechBubble>, 3> speechBubbles;

	//�s�����͐ݒ�
	std::unique_ptr<ActionInputConfigKey> actionInputConfigKey;
	std::unique_ptr<ActionInputConfigPad> actionInputConfigPad;
	bool isInputConfigKey = false;
	bool isInputConfigPad = false;

	//�I���{�^���������\UI
	std::unique_ptr<PressSelectButtonUI> pressSelectButtonUI;

	//����|�[�^��
	Portal* selectPortal = nullptr;
	//�|�[�^���ɓ���s�������Ă��邩
	bool isIntoPortal = false;

	//�V�[���J�ڑҋ@����
	bool isSceneChangeWait = false;
};

