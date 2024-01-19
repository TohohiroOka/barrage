#include "PlayerActionMoveNormal.h"
#include "Player.h"
#include "Camera/GameCamera.h"
#include "system/GameInputManager.h"
#include "GameHelper.h"

using namespace DirectX;

float PlayerActionMoveNormal::moveSpeedMax = 0.6f;
float PlayerActionMoveNormal::dashSpeedMax = 1.0f;

PlayerActionMoveNormal::PlayerActionMoveNormal(Player* player)
	: PlayerActionBase(player)
{
	//ジャンプアニメーション以外なら待機アニメーションに変更
	if (player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::JUMP_ANIMATION) { return; }
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
}

PlayerActionMoveNormal::~PlayerActionMoveNormal()
{
}

void PlayerActionMoveNormal::Update()
{
	//毎フレーム初期化
	player->GetData()->isMoveKey = false;
	player->GetData()->isMovePad = false;

	//移動
	Move();

	//次の行動が開始可能か判定
	NextActionStart();
}

void PlayerActionMoveNormal::Move()
{
	DirectInput* input = DirectInput::GetInstance();

	//移動キー入力を判定
	player->GetData()->isMoveKey = player->GetData()->actionInput.isMove && (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key));

	//ある程度スティックを傾けないと移動パッド入力判定しない
	const XMFLOAT2 padIncline = GameInputManager::GetPadLStickIncline();
	player->GetData()->isMovePad = player->GetData()->actionInput.isMove && (fabsf(padIncline.x) >= GameInputManager::GetPadStickInputIncline() || fabsf(padIncline.y) >= GameInputManager::GetPadStickInputIncline());

	//ダッシュ
	Dash();

	//入力があれば
	const float moveAccel = 0.05f * GameHelper::Instance()->GetGameSpeed();
	if (player->GetData()->isMoveKey || player->GetData()->isMovePad) {
		player->GetData()->moveSpeed += moveAccel;
		if (isDash) { player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, dashSpeedMax); }
		else { player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, moveSpeedMax); }

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
		player->SetMoveRotate(moveRotaVelocity, 15.0f);

		//待機アニメーションのときのみ走るアニメーションを開始する(現在は待機アニメーションがないので代用中)
		if (player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::STAY_ANIMATION) {
			player->GetFbxObject()->AnimationReset();
			player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::RUN_ANIMATION);
		}
	}
	else {
		player->GetData()->moveSpeed -= moveAccel * 2;
		player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, 0);

		//スピードがなくなったら待機アニメーションに変更(設計中)
		if (player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::RUN_ANIMATION && player->GetData()->moveSpeed <= 0) {
			player->GetFbxObject()->AnimationReset();
			player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
		}
	}

	//速度をセット
	player->GetData()->velocity.x = player->GetData()->moveVec.x * player->GetData()->moveSpeed;
	player->GetData()->velocity.z = player->GetData()->moveVec.z * player->GetData()->moveSpeed;
}

void PlayerActionMoveNormal::Dash()
{
	//地面にいない場合は、変更を受け付けないで抜ける
	if (!player->GetData()->onGround) { return; }

	if (!isDash) {
		//ダッシュ開始可能で地面接地しているかつ、ダッシュ入力があって移動した場合にダッシュ状態にする
		if (isDashStart && (player->GetData()->isMoveKey || player->GetData()->isMovePad) && player->GetData()->endurance > 0 && GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
			isDash = true;
			isDashStart = false;
		}

		//ダッシュ開始不能時は、ダッシュボタン入力を一度離すことで可能になる
		if ((!isDashStart) && (!GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash))) {
			isDashStart = true;
		}
	}
	else {
		//移動 & 入力中はダッシュ状態を維持
		if ((player->GetData()->isMoveKey || player->GetData()->isMovePad) && player->GetData()->endurance > 0 && GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
			player->UseEndurance(PlayerData::dashUseEndurance, 10, false); //持久力を使用
		}
		//入力が途切れたときにダッシュを終了する
		else {
			isDash = false;
		}
	}
}

void PlayerActionMoveNormal::NextActionStart()
{
	//いずれかの行動を開始できる状態か判定
	if (JumpStart()) { nextAction = PlayerActionName::JUMP; }
	else if (player->GetData()->actionInput.isLightAttack && GameInputManager::TriggerInputAction(GameInputManager::LightAttack) && LightAttackStart()) { nextAction = PlayerActionName::LIGHT_ATTACK; }
	else if (player->GetData()->actionInput.isStrongAttack && GameInputManager::TriggerInputAction(GameInputManager::StrongAttack) && StrongAttackStart()) { nextAction = PlayerActionName::STRONG_ATTACK; }
	else if (AvoidStart()) { nextAction = PlayerActionName::AVOID; player->GetData()->avoidBlinkMoveVec = player->GetData()->moveVec; }
	else if (BlinkStart()) { nextAction = PlayerActionName::BLINK; player->GetData()->avoidBlinkMoveVec = player->GetData()->moveVec; }
	else { return; }

	//行動開始可能なら、その行動をセット
	player->GetData()->action = nextAction;

	//通常移動行動終了
	isActionEnd = true;
}
