#pragma once
#include "Object/3d/Object3d.h"

/// <summary>
/// �����o���I�u�W�F�N�g
/// </summary>
class SpeechBubble
{
public: //�����o�֐�
	SpeechBubble(Camera* camera, const std::string& modelName, const DirectX::XMFLOAT3& position, float scale);
	~SpeechBubble() = default;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private: //�����o�ϐ�
	//�I�u�W�F�N�g
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;

	//�J�����Q��
	Camera* camera;
};