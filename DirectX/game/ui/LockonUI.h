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

	void Update();

	void Draw();

	void StartLockOnAnimation(const DirectX::XMFLOAT2 *enemyPos);

	void EndLockOnDraw() { isDraw = false; }

private:
	std::unique_ptr<Model>		lockonModel;
	std::unique_ptr<Object3d>	lockOnObject;
	std::unique_ptr<Sprite>		lockonSprite;

	//�J�����Q��
	Camera* camera;
	
	//�G���W�i�Q�Ɓj
	const DirectX::XMFLOAT2 *position;

	//���t���[��
	int frame = 0;

	//�T�C�Y�ϓ�����
	const int SCALE_ANIM_FRAME = 30;
	//�\������
	const int DISPLAY_FRAME = 60;

	//�\���t���O
	bool isDraw = false;


};