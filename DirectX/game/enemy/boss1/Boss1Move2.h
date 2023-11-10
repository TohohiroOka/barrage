#pragma once
#include "../BaseAction.h"
#include <DirectXMath.h>
#include "../Math/Timer.h"
#include <functional>

/// <summary>
/// �Ǎۂ̎��ɒ������Ɉړ�����
/// </summary>
class Boss1Move2 : public BaseAction
{
private:

	enum class State {
		up,
		side,
		size,
	};

public:
	Boss1Move2();
	~Boss1Move2() {};

	void Update() override;

	void Draw() override {};

	void FrameReset() override {};

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void UpMove();

	void SideMove();

private:

	//�J�n�n�_
	DirectX::XMFLOAT3 startPos;
	//�ړ���n�_
	DirectX::XMFLOAT3 endPos;
	//�C�[�W���O�^�C�}�[
	std::unique_ptr<Engine::Timer> timer;
	//���݂̓���
	State state;
	std::vector<std::function<void()>> func_;

};