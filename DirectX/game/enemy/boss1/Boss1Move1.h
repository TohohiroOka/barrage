#pragma once
#include "../game/enemy/BaseAction.h"
#include <DirectXMath.h>

/// <summary>
/// �����ړ��i�C�[�W���O����j
/// </summary>
class Boss1Move1 : public BaseAction
{
public:
	Boss1Move1(DirectX::XMFLOAT3 _pos);
	~Boss1Move1() {};

	void Update() override;

	void Draw() {};

	void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) override {};


private:

	//�J�n�n�_
	DirectX::XMFLOAT3 startPos;
	//�ړ���n�_
	DirectX::XMFLOAT3 endPos;
	//�C�[�W���O�^�C�}�[
	float timer;
};

