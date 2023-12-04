#include "PlayerActionAttack.h"
#include "Player.h"
#include "Camera/GameCamera.h"
#include "system/GameInputManager.h"

using namespace DirectX;


PlayerActionAttack::PlayerActionAttack(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //持久力を使用

	//予め次の行動を設定しておく(終了後は通常移動)
	nextAction = PlayerActionName::MOVENORMAL;
}

PlayerActionAttack::~PlayerActionAttack()
{
}

void PlayerActionAttack::Update()
{
	//毎フレーム初期化
	player->GetData()->isMoveKey = false;
	player->GetData()->isMovePad = false;

	//攻撃の方向を変更
	ChangeRotate();

	//次の攻撃を入力
	NextAttack();

	//攻撃の内容を更新
	player->GetData()->attackAction->Update();
	//攻撃の行動が終了したら攻撃行動終了
	if (player->GetData()->attackAction->GetIsAttackActionEnd()) {
		isActionEnd = true;

		//予約していた次の行動をセット
		player->GetData()->action = nextAction;
	}

	//いつでも回避で攻撃を中断できる
	if (AvoidStart()) {
		isActionEnd = true;

		//回避のベクトルを設定
		player->GetData()->avoidBlinkMoveVec = player->GetData()->moveVec;

		//次の行動を回避にセット
		nextAction = PlayerActionName::AVOID;
		player->GetData()->action = nextAction;
	}
}

void PlayerActionAttack::ChangeRotate()
{
	DirectInput* input = DirectInput::GetInstance();

	//キー入力を判定
	player->GetData()->isMoveKey = (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key));

	//ある程度スティックを傾けないとパッド入力判定しない
	const XMFLOAT2 padIncline = GameInputManager::GetPadLStickIncline();
	player->GetData()->isMovePad = (fabsf(padIncline.x) >= GameInputManager::GetPadStickInputIncline() || fabsf(padIncline.y) >= GameInputManager::GetPadStickInputIncline());


	//入力判定がなければ抜ける
	if (!(player->GetData()->isMoveKey || player->GetData()->isMovePad)) { return; }

	Vector3 inputMoveVec{};

	if (player->GetData()->isMoveKey) {
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key)) {
			inputMoveVec.x = 1;
		}
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key)) {
			inputMoveVec.x = -1;
		}
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key)) {
			inputMoveVec.z = 1;
		}
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key)) {
			inputMoveVec.z = -1;
		}
	}
	if (player->GetData()->isMovePad) {
		//パッドスティックの方向をベクトル化
		inputMoveVec.x = GameInputManager::GetPadLStickIncline().x;
		inputMoveVec.z = GameInputManager::GetPadLStickIncline().y;
	}

	//ベクトルをカメラの傾きで回転させる
	inputMoveVec.normalize();
	const float cameraRotaRadian = XMConvertToRadians(-player->GetGameCamera()->GetCameraRota().y);
	player->GetData()->moveVec.x = inputMoveVec.x * cosf(cameraRotaRadian) - inputMoveVec.z * sinf(cameraRotaRadian);
	player->GetData()->moveVec.z = inputMoveVec.x * sinf(cameraRotaRadian) + inputMoveVec.z * cosf(cameraRotaRadian);

	//進行方向を向くようにする
	const Vector3 moveRotaVelocity = { player->GetData()->moveVec.x, 0, player->GetData()->moveVec.z }; //プレイヤー回転にジャンプは関係ないので、速度Yは0にしておく
	player->SetMoveRotate(moveRotaVelocity, 5.0f);
}

void PlayerActionAttack::NextAttack()
{
	//次の行動を入力可能でなければ抜ける
	if (!player->GetData()->attackAction->GetIsNextActionInput()) { return; }

	//連続で攻撃をセット
	if (GameInputManager::TriggerInputAction(GameInputManager::Attack)) {
		if (!player->GetData()->attackAction->NextAttack()) { return; }

		player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //持久力を使用

		//予め次の行動を設定しておく(終了後は通常移動)
		nextAction = PlayerActionName::MOVENORMAL;
	}
	//ジャンプの先行入力も受け付ける
	else if (JumpStart()) {
		nextAction = PlayerActionName::JUMP;
	}
}
