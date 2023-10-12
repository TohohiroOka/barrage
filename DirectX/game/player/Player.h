#pragma once
#include "Object/3d/Object3d.h"
#include "Math/Vector3.h"
#include "game/ui/Gauge.h"

class GameCamera;

class Player
{
private:

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	Player();
	~Player(){};

	void Update();
	void Draw();
	void Damage(int damageNum);
	void Heal(int healNum);

private:

	void Move();
	void Dash();
	void Fall();
	void Jump();
	void Collider();
	void Attack();
	void HealHPMove();
	void UseEndurance(const int enduranceUseNum, const int enduranceRecoveryStartTime, bool isDecreaseDiffMode);
	void EnduranceRecovery();

public:

	const Vector3& GetPosition() { return pos; }
	int GetJumpCount() { return jumpCount; }
	int GetJumpMaxNum() { return jumpMaxNum; }
	Object3d* GetObject3d() { return object.get(); }

	void SetGameCamera(GameCamera* gameCamera) { this->gameCamera = gameCamera; }


private: //静的メンバ変数
	//最大移動スピード
	static const float moveSpeedMax;
	//最大ダッシュスピード
	static const float dashSpeedMax;

private: //メンバ変数
	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Object3d> object = nullptr;

	GameCamera* gameCamera = nullptr;

	Vector3 pos;
	Vector3 moveVec;
	Vector3 rota;

	bool onGround = true;
	// 落下ベクトル
	DirectX::XMVECTOR fallV;

	//y軸の押し戻し
	bool returnY;

	//移動スピード
	float moveSpeed = 0.0f;

	//ダッシュしているか
	bool isDash = false;
	//ジャンプ可能回数
	int jumpMaxNum;
	//ジャンプ回数カウント
	int jumpCount = 0;

	//攻撃中か
	bool isAttack = false;

	//ノックバック中か

	//回復中か
	bool isHeal = false;
	//回復タイマー
	int healTimer = 0;
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
	int enduranceRecoveryStartTimer;
	//持久力ゲージ
	std::unique_ptr<Gauge> enduranceGauge;

	//死亡フラグ
	bool isDead = false;
};