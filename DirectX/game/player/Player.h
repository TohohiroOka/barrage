#pragma once
#include "Object/3d/Object3d.h"
#include "Object/3d/Fbx.h"
#include "Math/Vector3.h"
#include "game/ui/Gauge.h"
#include "BasePlayerAttack.h"
#include "Math/Timer.h"

class GameCamera;

class Player
{
private:

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

private: //enum

	enum AnimationName
	{
		RUN,
		ATTACK_RIGHT,
		JUMP,
		ATTACK_LEFT,
		ROLL,
	};

public:

	Player();
	~Player() {};

	void Update();
	void Draw();
	void DrawSprite();
	void FrameReset();
	void ImguiDraw();
	void DrawLightView();
	void Damage(int damageNum, const Vector3& subjectPos);
	void Heal(int healNum);
	void PushBack(const XMVECTOR& reject);

private:
	void ObjectUpdate();
	void Move();
	void SetMoveRotate();
	void MoveRotate();
	void Dash();
	void Fall();
	void AvoidStart();
	void Avoid();
	void Jump();
	void BlinkStart();
	void Blink();
	void Attack();
	void HealHPMove();
	void UseEndurance(const int enduranceUseNum, const int enduranceRecoveryStartTime, bool isDecreaseDiffMode);
	void EnduranceRecovery();
	void KnockbackStart(const Vector3& subjectPos, int power);
	void Knockback();

public:
	bool GetIsAvoid() { return isAvoid; }
	bool GetIsBlink() { return isBlink; }
	const Vector3& GetPosition() { return pos; }
	const Vector3& GetVelocity() { return velocity; }
	int GetJumpCount() { return jumpCount; }
	int GetJumpMaxNum() { return jumpMaxNum; }
	Fbx* GetObject3d() { return object.get(); }
	BasePlayerAttack* GetAttackAction() { return attackAction.get(); }
	bool GetIsDead() { return isDead; }


	void SetGameCamera(GameCamera* gameCamera) { this->gameCamera = gameCamera; }


private: //静的メンバ変数
	//移動制限
	static const XMFLOAT3 moveMinPos;
	static const XMFLOAT3 moveMaxPos;
	//最大移動スピード
	static const float moveSpeedMax;
	//ジャンプ力
	static float jumpPower;
	//落下用重力加速度
	static float gravityAccel;
	//最大ダッシュスピード
	static const float dashSpeedMax;
	//各行動で使用する持久力
	static const int dashUseEndurance = 1;
	static const int avoidUseEndurance = 10;
	static const int jumpUseEndurance = 0;
	static const int blinkUseEndurance = 0;

private: //メンバ変数
	std::unique_ptr<FbxModel> model = nullptr;
	std::unique_ptr<Fbx> object = nullptr;

	std::unique_ptr<Model> swordModel = nullptr;

	GameCamera* gameCamera = nullptr;

	Vector3 pos;
	Vector3 velocity;
	Vector3 rota;

	bool onGround = false;
	// 落下ベクトル
	float fallSpeed = 0;;

	//移動
	float moveSpeed = 0.0f;
	Vector3 moveVec;
	Vector3 moveVelRota;
	float moveRotSpeed = 0.0f;
	//移動入力があるか
	bool isMoveKey = false;
	bool isMovePad = false;
	bool isMoveRotate = false;

	//ダッシュしているか
	bool isDash = false;
	bool isDashStart = true;

	//回避中か
	bool isAvoid = false;
	//回避用タイマー
	std::unique_ptr<Engine::Timer> avoidTimer;
	//回避開始可能か
	bool isAvoidStart = true;

	//ジャンプ可能回数
	int jumpMaxNum;
	//ジャンプ回数カウント
	int jumpCount = 0;
	//ジャンプ力調整用、ボタン長押し中か
	bool isInputJump = false;

	//ブリンク中か
	bool isBlink = false;
	//ブリンク用タイマー
	std::unique_ptr<Engine::Timer> blinkTimer = 0;
	//ブリンク開始可能か
	bool isBlinkStart = true;

	//攻撃中か
	bool isAttack = false;
	//攻撃行動
	std::unique_ptr<BasePlayerAttack> attackAction;

	//ノックバック中か
	bool isKnockback = false;
	//ノックバックベクトル
	Vector3 knockbackVec;
	//ノックバックの強さ
	float knockbackPower;


	//回復中か
	bool isHeal = false;
	//回復タイマー
	std::unique_ptr<Engine::Timer> healTimer = 0;
	//回復前の体力
	int healBeforeHP;
	//回復完了後の体力
	int healAfterHP;
	//最大体力
	int maxHP;
	//体力
	int HP;
	//体力ゲージ
	std::unique_ptr<Gauge> hpGauge;
	//最大持久力
	int maxEndurance;
	//持久力
	int endurance;
	//持久力回復開始までのタイマー
	std::unique_ptr<Engine::Timer> enduranceRecoveryStartTimer;
	//持久力ゲージ
	std::unique_ptr<Gauge> enduranceGauge;

	//死亡フラグ
	bool isDead = false;
};