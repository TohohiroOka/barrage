#pragma once
#include "Scene/InterfaceScene.h"
#include "Math/Vector3.h"
#include "Player.h"
#include "../game/enemy/boss1/Boss1.h"


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
	/// �X�V
	/// </summary>
	void CameraUpdate(const int _cameraNum, Camera* camera) override;

	/// <summary>
	///	�`��
	/// </summary>
	/// <param name="_cameraNum">�J�����ԍ�</param>
	void Draw(const int _cameraNum) override;
	
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

private:

	Camera* camera;

	//�J�������W
	XMFLOAT3 cameraPos;
	//�J�������W
	XMFLOAT3 cameraTarget;

	//�^�[�Q�b�g�p�x
	XMFLOAT3 cameraRota;

	std::unique_ptr<Sprite> sprite;

	std::shared_ptr<Player> player;

	//���f��
	std::shared_ptr<TerrainModel> gmodel;
	//�I�u�W�F�N�g
	std::shared_ptr<HeightMap> gobject;

	std::unique_ptr<Boss1>boss;

};