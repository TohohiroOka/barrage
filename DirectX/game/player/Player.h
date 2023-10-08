#pragma once
#include "Object/3d/Object3d.h"
#include "Math/Vector3.h"

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

private:

	void Move();
	void Dash();
	void Fall();
	void Jump();
	void Collider();

public:

	const Vector3& GetPos() { return pos; }
	int GetJumpCount() { return jumpCount; }
	int GetJumpMaxNum() { return jumpMaxNum; }

private: //静的メンバ変数
	//最大移動スピード
	static const float moveSpeedMax;
	//最大ダッシュスピード
	static const float dashSpeedMax;

private: //メンバ変数

	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Object3d> object = nullptr;

	Vector3 pos;
	Vector3 moveVec;
	float moveRota;

	bool onGround = true;
	// 落下ベクトル
	DirectX::XMVECTOR fallV;

	//移動スピード
	float moveSpeed = 0.0f;

	//ダッシュしているか
	bool isDash = false;
	//ジャンプ可能回数
	int jumpMaxNum;
	//ジャンプ回数カウント
	int jumpCount = 0;
};