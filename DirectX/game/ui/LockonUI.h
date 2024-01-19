#pragma once
#include "Object/3d/Object3d.h"
#include "Object/2d/Sprite.h"

#include <memory>


class LockonUI
{
public:
	LockonUI();
	~LockonUI() = default;

	void Init(Camera *camera);

	void Update(const DirectX::XMFLOAT3 &enemyPos);

	void Draw();

	void StartLockOnAnimation();

	void EndLockOnDraw() { isDraw = false; }

private:
	std::unique_ptr<Model>		lockonModel;
	std::unique_ptr<Object3d>	lockOnObject;
	std::unique_ptr<Sprite>		lockonSprite;

	//�J�����Q��
	Camera* camera;

	//���f���̒��S�ɍ��킹�邽�߂�y���W�I�t�Z�b�g
	const float MODEL_Y_OFFSET = 8.f;

	//���t���[��
	int frame = 0;

	//�T�C�Y�ϓ�����
	const int SCALE_ANIM_FRAME = 30;
	//�\������
	const int DISPLAY_FRAME = 60;

	//�\���t���O
	bool isDraw = false;


};