#include "GameCamera.h"
#include "Input/DirectInput.h"
#include "WindowApp.h"
#include "GameHelper.h"

using namespace DirectX;

Player* GameCamera::player = nullptr;

std::unique_ptr<GameCamera> GameCamera::Create(const bool _mode)
{
	// インスタンスを生成
	GameCamera* instance = new GameCamera(_mode);
	if (instance == nullptr) {
		return nullptr;
	}

	//ユニークポインタを返す
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
	const float camera_sa = 10.0f;
	const float LR_sa = 163.0f;
	const float UD_sa = 53.0f;

	DirectInput* input = DirectInput::GetInstance();
	//視界移動
	const float Tgspeed = 1.0f;
	if (input->PushKey(DIK_LEFT)) { cameraRota.x -= Tgspeed; }//右入力
	if (input->PushKey(DIK_RIGHT)) { cameraRota.x += Tgspeed; }//左入力
	if (input->PushKey(DIK_DOWN)) { cameraRota.y -= Tgspeed; }//下入力
	if (input->PushKey(DIK_UP)) { cameraRota.y += Tgspeed; }//上入力

	//上下方向の角度制限
	if (cameraRota.y <= -13) { cameraRota.y = -13; }//下制限
	if (cameraRota.y >= 13) { cameraRota.y = 13; }//上制限

	//横の制限
	if (cameraRota.x > 360) { cameraRota.x = 0; }
	if (cameraRota.x < -360) { cameraRota.x = 0; }

	//カメラ移動
	float Pspeed = 1.0f;
	XMFLOAT2 radian = { XMConvertToRadians(cameraRota.x),XMConvertToRadians(cameraRota.y) };
	XMFLOAT2 radian2 = { XMConvertToRadians(cameraRota.x + 180),XMConvertToRadians(cameraRota.y + 180) };

	Vector3 ppos = player->GetPos();

	const float dist = 15.0f;
	eye = { ppos.x + sinf(radian2.x) * dist,ppos.y + sinf(radian2.y) * dist,ppos.z + cosf(radian2.x) * dist };
	target = { ppos.x + sinf(radian.x) * dist,ppos.y + sinf(radian.y) * dist,ppos.z + cosf(radian.x) * dist };
}
