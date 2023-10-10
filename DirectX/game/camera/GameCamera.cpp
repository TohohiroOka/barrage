#include "GameCamera.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "WindowApp.h"
#include "GameHelper.h"

using namespace DirectX;

Player* GameCamera::player = nullptr;

std::unique_ptr<GameCamera> GameCamera::Create(const bool _mode)
{
	// �C���X�^���X�𐶐�
	GameCamera* instance = new GameCamera(_mode);
	if (instance == nullptr) {
		return nullptr;
	}

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<GameCamera>(instance);
}

GameCamera::GameCamera(const bool _mode) :
	Camera(_mode)
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	Move();
	Camera::Update();
}

void GameCamera::Move()
{
	DirectInput* input = DirectInput::GetInstance();
	//���E�ړ�
	const float Tgspeed = 1.0f;

	//�L�[����
	{
		if (input->PushKey(DIK_LEFT)) { cameraRota.x -= Tgspeed; }//�E����
		if (input->PushKey(DIK_RIGHT)) { cameraRota.x += Tgspeed; }//������
		if (input->PushKey(DIK_DOWN)) { cameraRota.y -= Tgspeed; }//������
		if (input->PushKey(DIK_UP)) { cameraRota.y += Tgspeed; }//�����
	}
	//�R���g���[���[����
	{
		//������x�X�e�B�b�N���X���Ȃ��Ɣ��肵�Ȃ�
		const float moveStickIncline = 0.3f;
		const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadRStickIncline();
		if (fabsf(padIncline.x) >= moveStickIncline || fabsf(padIncline.y) >= moveStickIncline) {
			const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadRStickIncline();
			const float stickRadian = XMConvertToRadians(XInputManager::GetInstance()->GetPadRStickAngle() - 90);
			cameraRota.x += cosf(stickRadian) * fabsf(padIncline.x) * Tgspeed;
			cameraRota.y += -sinf(stickRadian) * fabsf(padIncline.y) * Tgspeed;
		}
	}

	//�㉺�����̊p�x����
	cameraRota.y = max(cameraRota.y, -89);
	cameraRota.y = min(cameraRota.y, 89);

	//����]�̉�]�p��0�`360�ȓ��Ɏ��܂�悤�ɂ���
	if (cameraRota.x > 360) { cameraRota.x -= 360; }
	if (cameraRota.x < 0) { cameraRota.x += 360; }

	//�J�����ړ�
	XMFLOAT2 radian = { XMConvertToRadians(cameraRota.x),XMConvertToRadians(cameraRota.y) };
	XMFLOAT2 radian2 = { XMConvertToRadians(cameraRota.x + 180),XMConvertToRadians(cameraRota.y + 180) };

	Vector3 ppos = player->GetPos();

	const float dist = 25.0f;
	eye = { ppos.x + sinf(radian2.x) * dist,ppos.y + sinf(radian2.y) * dist,ppos.z + cosf(radian2.x) * dist };
	target = { ppos.x + sinf(radian.x) * dist,ppos.y + sinf(radian.y) * dist,ppos.z + cosf(radian.x) * dist };

	player->SetMoveRota(cameraRota.x);
}
