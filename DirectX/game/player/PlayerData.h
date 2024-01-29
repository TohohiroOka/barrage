#pragma once
#include "Math/Vector3.h"
#include "BasePlayerAttack.h"
#include <memory> 

/// <summary>
/// プレイヤーの行動名
/// </summary>
enum PlayerActionName
{
	MOVE_NORMAL,	//通常移動(入力に制限がかからない)
	JUMP,	//ジャンプ(1フレーム)
	LIGHT_ATTACK,	//弱攻撃
	STRONG_ATTACK,	//強攻撃
	AVOID,	//回避
	BLINK,	//ブリンク
	KNOCKBACK,//ノックバック
	DEAD,	//死亡
	TITLE_FLOAT,	//タイトルシーン用浮遊
	TITLE_INTO_PORTAL,	//タイトルシーン用ポータルに入る
};

/// <summary>
/// アニメーション名
/// </summary>
enum PlayerAnimationName
{
	ATTACK1_ANIMATION,//1
	ATTACK2_ANIMATION,//2
	ATTACK3_ANIMATION,//3
	BLINK_ANIMATION,//4
	DEAD_ANIMATION,//5
	STAY_ANIMATION,//6
	JUMP_ANIMATION,//7
	ROLL_ANIMATION,//8
	RUN_ANIMATION,//9
	NO_DATA1,//10
	NO_DATA2,//11
	NO_DATA3,//12
	FEAR_ANIMATION,//13
	HEAVY_ATTACK1_ANIMATION,//14
	HEAVY_ATTACK2_ANIMATION,//15
	HEAVY_ATTACK3_ANIMATION,//16
	
};

/// <summary>
/// プレイヤーに必要な変数
/// </summary>
class PlayerData
{
public: //struct
	/// <summary>
	/// 入力で行動が可能か
	/// </summary>
	struct PlayerActionInput
	{
		bool isMove;
		bool isJump;
		bool isLightAttack;
		bool isStrongAttack;
		bool isAvoid;
		bool isBlink;
	};

public: //メンバ関数
	PlayerData();
	~PlayerData();

	//setter
	void SetAllActionInput(bool isInput) {
		actionInput.isMove = isInput;
		actionInput.isJump = isInput;
		actionInput.isLightAttack = isInput;
		actionInput.isStrongAttack = isInput;
		actionInput.isAvoid = isInput;
		actionInput.isBlink = isInput;
	}

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

	PlayerActionInput actionInput; //入力で行動が可能か

	bool isMoveKey; //移動入力がされているか(キーボード)
	bool isMovePad; //移動入力がされているか(ゲームパッド)
	
	bool isDash; //ダッシュ中か
	float moveSpeed; //移動スピード

	int jumpMaxNum; //ジャンプ可能回数
	int jumpCount; //ジャンプ回数カウント
	bool isInputJump; //ジャンプ力調整用、ボタン長押し中か

	std::unique_ptr<BasePlayerAttack> attackAction; //攻撃行動内容

	Vector3 avoidBlinkMoveVec; //回避ブリンク用移動ベクトル
	int blinkCount; //ブリンク回数

	bool isEnemyAttackAvoid; //敵の攻撃を避けたか
	bool isEnemyAttackAvoidJudge; //敵の攻撃を避けたか判定するか

	int maxHP; //最大体力
	int HP; //体力

	int maxEndurance; //最大持久力
	int endurance; //持久力

	bool isNoGravity; //落下無効フラグ
	bool isNoDamage; //無敵フラグ
	bool isUseEndurance; //持久力消費フラグ
	bool isDead; //死亡フラグ
};