#include "PlayerSwordAttack1.h"
#include "Player.h"
#include "Camera/GameCamera.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Math/Easing/Easing.h"
#include "engine/Audio/Audio.h"
#include "system/GameInputManager.h"

using namespace DirectX;

int PlayerSwordAttack1::attackUseEnduranceNum = 20;
int PlayerSwordAttack1::attackPower = 50;
const DirectX::XMFLOAT4 PlayerSwordAttack1::attackColor = { 1, 0, 0, 0.3f };
const DirectX::XMFLOAT4 PlayerSwordAttack1::nonAttackColor = { 0, 0, 1, 0.3f };
const float PlayerSwordAttack1::attackStartMoveSpeedMax = 1.5f;
const float PlayerSwordAttack1::attackStartMoveSpeedMin = 0.5f;

PlayerSwordAttack1::PlayerSwordAttack1(Player* player)
{
	//プレイヤーをセット
	this->player = player;
	player->GetSword()->GetSlashEffect()->SetIsParticleEmit(true); //仮で斬撃演出生成を開始しておく。
	player->GetSword()->GetSlashEffect()->SetStartColor(DirectX::XMFLOAT4{ 0.02f, 0.05f, 0.2f, 1.0f });
	//当たり判定可視化用オブジェクト生成
	model = Model::CreateFromOBJ("bullet");

	object = Object3d::Create(model.get());
	object->SetScale({ 1.2f, 1.2f, 1.2f });
	object->SetColor(nonAttackColor);

	//タイマー初期化
	timer = std::make_unique<Engine::Timer>();

	//持久力の使用料をセット
	useEnduranceNum = attackUseEnduranceNum;
	func_.emplace_back([this] {return AttackAction1(); });
	func_.emplace_back([this] {return AttackAction2(); });
	func_.emplace_back([this] {return AttackAction3(); });
}

PlayerSwordAttack1::~PlayerSwordAttack1()
{
}

void PlayerSwordAttack1::Update()
{
	if (state == NONE) { return; }

	//毎フレーム初期化
	player->GetData()->isMoveKey = false;
	player->GetData()->isMovePad = false;

	//プレイヤー回転
	PlayerChangeRotate();

	//オブジェクト更新
	func_[int(state)]();
	object->Update();

	//衝突判定用変数の更新
	DirectX::XMFLOAT3 swordTopButtonVec = player->GetFbxObject()->GetAttachPos("sword2") - player->GetFbxObject()->GetAttachPos("sword1"); //剣先から持つ部分までのベクトル
	DirectX::XMFLOAT3 objectPos = player->GetFbxObject()->GetAttachPos("sword1") + (swordTopButtonVec * 0.75f);
	object->SetPosition(objectPos);
	attackCollisionData.startPosition = player->GetFbxObject()->GetAttachPos("sword1");
	attackCollisionData.endPosition = player->GetFbxObject()->GetAttachPos("sword2");
	attackCollisionData.radius = object->GetScale().x;
}

void PlayerSwordAttack1::Draw()
{
	if (state == NONE) { return; }

	object->Draw();
}

bool PlayerSwordAttack1::NextAttack()
{
	//攻撃行動を最大数まで行っていたら抜ける
	if (attackNum >= maxAttackNum) { return false; }

	//連続攻撃回数を増やす
	attackNum++;

	//持久力が足りなかったら次の攻撃ができずに抜ける
	if (player->GetData()->endurance < useEnduranceNum) {
		//何もしていない初期段階なら攻撃自体を終了する
		if (state == NONE) { isAttackActionEnd = true; }

		return false;
	}

	//アニメーションリセット
	player->GetFbxObject()->AnimationReset();

	if (attackNum == 1) {
		//攻撃1へ
		state = State::ATTACK1;

		//攻撃力変更
		attackCollisionData.power = attackPower;

		//攻撃1アニメーション
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK1_ANIMATION);
	}
	else if (attackNum == 2) {
		//攻撃2へ
		state = State::ATTACK2;

		//攻撃2アニメーション
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK2_ANIMATION);
	}

	else if (attackNum == 3) {
		//攻撃3へ
		state = State::ATTACK3;

		//攻撃3アニメーション
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK3_ANIMATION);
	}

	//攻撃で移動するとき用に移動スピードをセット(既にスピードがある場合は変更しない)
	player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, attackStartMoveSpeedMax);
	player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, attackStartMoveSpeedMin);
	attackStartMoveSpeed = player->GetData()->moveSpeed;


	//攻撃音再生
	Audio::Instance()->SoundPlayWava(Sound::SoundName::attack, false, 0.1f);

	isNextActionInput = false;
	timer->Reset();
	object->SetColor(nonAttackColor);
	isCollisionValid = false;
	isHitAttack = false;
	object->SetColor(attackColor);

	return true;
}

void PlayerSwordAttack1::AttackCollision()
{
	//毎フレーム攻撃が当たるとおかしいので衝突判定フラグを下げる
	isCollisionValid = false;

	//攻撃が当たった
	isHitAttack = true;

	object->SetColor(nonAttackColor);
}

void PlayerSwordAttack1::AttackAction1()
{
	//タイマー更新
	timer->Update();

	//攻撃に合わせてプレイヤーを動かす
	MovePlayer(actionChangeStartTime1);

	//攻撃判定をONにする
	if (*timer.get() == collisionValidStartTime1 && !isHitAttack) {
		isCollisionValid = true;
	}

	//次の行動を入力可能にする
	if (*timer.get() == actionChangeStartTime1) {
		isNextActionInput = true;
	}

	//タイマーが指定した時間になったら
	if (*timer.get() >= attackTime1) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction2()
{
	//タイマー更新
	timer->Update();

	//攻撃に合わせてプレイヤーを動かす
	MovePlayer(actionChangeStartTime2);

	//攻撃判定をONにする
	if (*timer.get() == collisionValidStartTime2 && !isHitAttack) {
		isCollisionValid = true;
	}

	//次の行動を入力可能にする
	if (*timer.get() == actionChangeStartTime2) {
		isNextActionInput = true;
	}

	//タイマーが指定した時間になったら
	if (*timer.get() >= attackTime2) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction3()
{
	//タイマー更新
	timer->Update();

	//攻撃に合わせてプレイヤーを動かす
	MovePlayer(actionChangeStartTime3);

	//攻撃判定をONにする
	if (*timer.get() == collisionValidStartTime3 && !isHitAttack) {
		isCollisionValid = true;
	}

	//次の行動を入力可能にする
	if (*timer.get() == actionChangeStartTime3) {
		isNextActionInput = true;
	}

	//タイマーが指定した時間になったら
	if (*timer.get() >= attackTime3) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::MovePlayer(int moveTime)
{
	//移動時間を過ぎていたら抜ける
	if (*timer.get() > moveTime) { return; }

	//スピードを落としていく
	const float easeTime = *timer.get() / (float)moveTime;
	player->GetData()->moveSpeed = Easing::OutSine(attackStartMoveSpeed, 0, easeTime);

	//速度をセット
	player->GetData()->velocity.x = player->GetData()->moveVec.x * player->GetData()->moveSpeed;
	player->GetData()->velocity.z = player->GetData()->moveVec.z * player->GetData()->moveSpeed;
}

void PlayerSwordAttack1::PlayerChangeRotate()
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
