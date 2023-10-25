#pragma once
#include "../game/enemy/BaseAction.h"
#include <DirectXMath.h>
#include "../Math/Timer.h"

/// <summary>
/// �����ړ��i�C�[�W���O����j
/// </summary>
class Boss1Move1 : public BaseAction
{
public:
	Boss1Move1(const DirectX::XMFLOAT3& _pos = {});
	~Boss1Move1() {};

	void Update() override;

	void Draw() override {};

	void FrameReset() override {};

	void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) override {};

private:

	//�J�n�n�_
	DirectX::XMFLOAT3 startPos;
	//�ړ���n�_
	DirectX::XMFLOAT3 endPos;
	//�C�[�W���O�^�C�}�[
	std::unique_ptr<Engine::Timer> timer;
};
