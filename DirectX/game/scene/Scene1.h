#pragma once
#include "Scene/InterfaceScene.h"
#include "Math/Vector3.h"
#include "engine/camera/DebugCamera.h"
#include "player/Player.h"
#include "game/camera/GameCamera.h"
#include "engine/Camera/LightCamera.h"
#include "../enemy/boss1/Boss1.h"
#include "../field/Field.h"
#include "../field/ShadowGround.h"

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
	bool isNormalCamera = false;
	float worldTimer;

	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<GameCamera> camera;
	std::unique_ptr<LightCamera> lightCamera;

	std::unique_ptr<Sprite> sprite;

	std::unique_ptr<Player> player;

	//�n�`
	std::unique_ptr<Field> field;

	//�{�X
	std::unique_ptr<Boss1>boss;

	std::unique_ptr<FbxModel> fbxModel1;
	std::unique_ptr<FbxModel> fbxModel2;
	std::unique_ptr<Fbx> fbx;

	bool isBlend;
	float rate;

	std::unique_ptr<ShadowGround> ground;
};