#pragma once
#include "Math/Vector3.h"
#include "BasePlayerAttack.h"
#include <memory> 

/// <summary>
/// プレイヤーの行動名
/// </summary>
enum PlayerActionName
{
	MOVENORMAL,	//通常移動(入力に制限がかからない)
	JUMP,	//ジャンプ(1フレーム)
	LIGHTATTACK,	//弱攻撃
	STRONGATTACK,	//強攻撃
	AVOID,	//回避
	BLINK,	//ブリンク
	KNOCKBACK,//ノックバック
	DEAD,	//死亡
};

/// <summary>
/// アニメーション名
/// </summary>
enum PlayerAnimationName
{
	ATTACK1_ANIMATION,
	ATTACK2_ANIMATION,
	ATTACK3_ANIMATION,
	BLINK_ANIMATION,
	DEAD_ANIMATION,
	STAY_ANIMATION,
	JUMP_ANIMATION,
	ROLL_ANIMATION,
	RUN_ANIMATION,
};

/// <summary>
/// プレイヤーに必要な変数
/// </summary>
class PlayerData
{
public: //メンバ関数
	PlayerData();
	~PlayerData();

public: //静的メンバ変数 (変更しやすいようにpublic)
	//各行動で使用する持久力
	static int dashUseEndurance;
	static int avoidUseEndurance;
	static int jumpUseEndurance;
	static int blinkUseEndurance;

public: //メンバ変数 (変更しやすいようにpublic)
	PlayerActionName action; //プレイヤーの行動

	Vector3 pos; //座標
	Vector3 moveVec; //移動ベクトル(方向)
	Vector3 velocity; //速度
	Vector3 rota; //回転

	bool onGround ; //地面に接地しているか
	float fallSpeed; // 落下スピード

	bool isMoveKey; //移動入力がされているか(キーボード)
	bool isMovePad; //移動入力がされているか(ゲームパッド)

	float moveSpeed; //移動スピード

	int jumpMaxNum; //ジャンプ可能回数
	int jumpCount; //ジャンプ回数カウント
	bool isInputJump; //ジャンプ力調整用、ボタン長押し中か

	std::unique_ptr<BasePlayerAttack> attackAction; //攻撃行動内容

	Vector3 avoidBlinkMoveVec; //回避ブリンク用移動ベクトル
	int blinkCount; //ブリンク回数

	int maxHP; //最大体力
	int HP; //体力

	int maxEndurance; //最大持久力
	int endurance; //持久力

	bool isNoGravity; //落下無効フラグ
	bool isNoDamage; //無敵フラグ
	bool isDead; //死亡フラグ
};