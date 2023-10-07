#pragma once
#include "Camera/Camera.h"
#include "player/Player.h"

/// <summary>
/// �Q�[���J����
/// </summary>
class GameCamera :public Camera
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns></returns>
	/// <param name="_mode">true->3D/false->2D</param>
	static std::unique_ptr<GameCamera> Create(const bool _mode = true);
	
	static void SetPlayer(Player* player) { GameCamera::player = player; }

public: //�����o�֐�
	GameCamera(const bool _mode);
	~GameCamera() override;

	void Update() override;
	void Move();

	const XMFLOAT3& GetCameraRota() { return cameraRota; }

private: //�ÓI�����o�ϐ�
	static Player* player;

private: //�����o�ϐ�
	//�J�������W
	XMFLOAT3 cameraPos;
	//�^�[�Q�b�g�p�x
	XMFLOAT3 cameraRota;
};