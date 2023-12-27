#pragma once
#include "DirectXCommon.h"
#include "Scene/SceneManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "Object/2d/PostEffect/BaseRender.h"
#include "Object/2d/PostEffect/PostEffect.h"
#include "Object/2d/PostEffect/Bloom.h"
#include "Object/2d/PostEffect/Outline.h"
#include "Object/2d/PostEffect/Fog.h"
#include "Object/2d/PostEffect/DepthRender.h"
#include "Object/3d/ShadowMap.h"
#include "FrameRateKeep.h"

#include <sstream>
#include <iomanip>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

class WindowApp;

class MainEngine
{
public:

	MainEngine() = default;
	~MainEngine();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <returns>�Q�[���𑱂��邩</returns>
	bool Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui�`��
	/// </summary>
	void ImguiDraw();

	/// <summary>
	/// �t���[�����[�g�L�[�v
	/// </summary>
	void FrameControl();

private:

	//DirectXCommon�̃C���X�^���X
	std::unique_ptr<DirectXCommon> dXCommon = nullptr;
	//Input�̃C���X�^���X
	DirectInput* input = nullptr;
	//XInputManager
	XInputManager* xinput = nullptr;
	//GameScene�̃C���X�^���X
	std::unique_ptr<SceneManager> scene = nullptr;
	//�����_�[�^�[�Q�b�g
	std::array<std::unique_ptr<BaseRender>, int(Engine::EngineUseRTVCreateTextureName::size)> render;
	//�`��
	std::unique_ptr<Sprite> mainSprite;
	//�|�X�g�G�t�F�N�g�̃C���X�^���X
	std::unique_ptr<PostEffect> postEffect;
	//�|�X�g�G�t�F�N�g���H�p
	std::unique_ptr<Bloom> bloom;
	//�|�X�g�G�t�F�N�g���H�p
	std::unique_ptr<Outline> outline;
	//�|�X�g�G�t�F�N�g���H�p
	std::unique_ptr<Fog> fog;
	//�|�X�g�G�t�F�N�g�Ŏg�p����[�x
	std::unique_ptr<DepthRender> depthRender;
	//�V���h�E�}�b�v
	std::unique_ptr<DepthRender> shadowMapRender;
	//Fps�Œ�p�N���X�̃C���X�^���X
	std::unique_ptr<FrameRateKeep> fps = nullptr;
	//�����\���f�o�b�O�p
	wchar_t str[256] = {};
};