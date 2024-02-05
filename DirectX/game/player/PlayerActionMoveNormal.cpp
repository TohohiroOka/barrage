#include "PlayerActionMoveNormal.h"
#include "Player.h"
#include "Camera/GameCamera.h"
#include "system/GameInputManager.h"
#include "GameHelper.h"
#include "engine/Audio/Audio.h"

using namespace DirectX;

float PlayerActionMoveNormal::moveSpeedMax = 0.6f;
float PlayerActionMoveNormal::dashSpeedMax = 1.0f;

PlayerActionMoveNormal::PlayerActionMoveNormal(Player* player)
	: PlayerActionBase(player)
{
	//足音用タイマー生成
	moveSoundTimer = std::make_unique<Engine::Timer>();

	//ジャンプアニメーション以外なら待機アニメーションに変更
	if (player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::JUMP_ANIMATION) { return; }
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
	player->GetFbxObject()->SetIsRoop(true);
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

		if (player->GetData()->onGround) {
			player->GetData()->moveSpeed += moveAccel;
			if (player->GetData()->isDash) { player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, dashSpeedMax); }
			else { player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, moveSpeedMax); }
		}

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
			player->GetFbxObject()->SetIsRoop(true);
			moveSoundTimer->Reset();
		}
	}
	else {
		//減速量(空中時は数値が小さくなるようにする)
		float moveDecAccel = moveAccel * 2;
		if (!player->GetData()->onGround) { moveDecAccel /= 10; }
		player->GetData()->moveSpeed -= moveDecAccel;
		player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, 0);

		//スピードがなくなったら待機アニメーションに変更(設計中)
		if (player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::RUN_ANIMATION && player->GetData()->moveSpeed <= 0) {
			player->GetFbxObject()->AnimationReset();
			player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
			player->GetFbxObject()->SetIsRoop(true);
		}
	}

	//速度をセット
	player->GetData()->velocity.x = player->GetData()->moveVec.x * player->GetData()->moveSpeed;
	player->GetData()->velocity.z = player->GetData()->moveVec.z * player->GetData()->moveSpeed;

	//足音を再生
	if (player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::RUN_ANIMATION) {
		//タイマー更新
		if (player->GetFbxObject()->GetIsAnimationEnd()) {
			if (player->GetFbxObject()->GetIsAnimationEnd()) {
				moveSoundTimer->Reset();
			}
		}
		moveSoundTimer->Update();


		//一定間隔で足音再生
		const int soundTime1 = 2;
		const int soundTime2 = 17;
		const int soundTime3 = 32;
		const int soundTime4 = 47;
		if (player->GetData()->onGround && ((int)moveSoundTimer.get()->time == soundTime1 || (int)moveSoundTimer.get()->time == soundTime2 ||
			(int)moveSoundTimer.get()->time == soundTime3 || (int)moveSoundTimer.get()->time == soundTime4)) {
			Audio::Instance()->SoundPlayWava(Sound::SoundName::select_cursor, false, 0.3f);
		}
	}
}
void PlayerActionMoveNormal::Dash()
{
	//地面にいない場合は、変更を受け付けないで抜ける
	if (!player->GetData()->onGround) { return; }

	if (!player->GetData()->isDash) {
		//地面接地しているかつ、ダッシュ入力があって移動した場合にダッシュ状態にする
		if ((player->GetData()->isMoveKey || player->GetData()->isMovePad) && player->GetData()->endurance > 0 && GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
			player->GetData()->isDash = true;
		}
	}
	else {
		//移動 & 入力中はダッシュ状態を維持
		if ((player->GetData()->isMoveKey || player->GetData()->isMovePad) && player->GetData()->endurance > 0 && GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
			player->UseEndurance(PlayerData::dashUseEndurance, 10, false); //持久力を使用
		}
		//入力が途切れたときにダッシュを終了する
		else {
			player->GetData()->isDash = false;
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
