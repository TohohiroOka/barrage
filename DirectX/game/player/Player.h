#pragma once
#include "Object/3d/Object3d.h"
#include "Object/3d/Fbx.h"
#include "PlayerData.h"
#include "PlayerActionBase.h"
#include "Math/Vector3.h"
#include "game/ui/Gauge.h"
#include "BasePlayerAttack.h"
#include "Math/Timer.h"
#include "PlayerSword.h"

class GameCamera;

class Player
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

public: //メンバ関数
	Player();
	~Player() {};

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 影用光源カメラ視点描画
	/// </summary>
	void DrawLightView();
	
	/// <summary>
	/// スプライト描画
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// Imgui描画
	/// </summary>
	void ImguiDraw();

	/// <summary>
	/// 毎フレームリセット
	/// </summary>
	void FrameReset();

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	/// <param name="knockbackVec">ノックバックベクトル</param>
	/// <param name="knockbackPower">ノックバックの強さ</param>
	/// <param name="knockbackTime">ノックバックをする時間</param>
	/// <param name="isKnockbackStart">ノックバックを開始するか</param>
	void Damage(int damageNum, const Vector3& knockbackVec, int knockbackPower, int knockbackTime, bool isKnockbackStart);

	/// <summary>
	/// 回復
	/// </summary>
	/// <param name="healNum">回復量</param>
	void Heal(int healNum);

	/// <summary>
	/// 敵に触れた際の押し戻し
	/// </summary>
	/// <param name="reject">押し戻し量</param>
	void PushBack(const XMVECTOR& reject);

	/// <summary>
	/// 移動方向を向く回転開始
	/// </summary>
	/// <param name="vec"></param>
	/// <param name="rotSpeed"></param>
	void SetMoveRotate(const Vector3& vec, float rotSpeed);

	/// <summary>
	/// 持久力を使用
	/// </summary>
	/// <param name="enduranceUseNum">使用する量</param>
	/// <param name="enduranceRecoveryStartTime">回復開始までかかる時間</param>
	/// <param name="isDecreaseDiffMode">使用量を黄色いバーで可視化するか</param>
	void UseEndurance(const int enduranceUseNum, const int enduranceRecoveryStartTime, bool isDecreaseDiffMode);

	/// <summary>
	/// タイトルシーン用行動演出開始
	/// </summary>
	void TitlePhaseStart();


	//getter
	Fbx* GetFbxObject() { return object.get(); }
	PlayerSword* GetSword() { return sword.get(); }
	GameCamera* GetGameCamera() { return gameCamera; }
	PlayerData* GetData() { return data.get(); }

	//setter
	void SetGameCamera(GameCamera* gameCamera) { this->gameCamera = gameCamera; }
	void SetPortalPos(const Vector3& portalPos) { this->portalPos = portalPos; }

private:
	/// <summary>
	/// オブジェクト更新
	/// </summary>
	void ObjectUpdate();

	/// <summary>
	/// 行動変更
	/// </summary>
	void ActionChange();
	
	/// <summary>
	/// 移動方向を向くように回転
	/// </summary>
	void MoveRotate();

	/// <summary>
	/// 落下
	/// </summary>
	void Fall();

	/// <summary>
	/// HP回復
	/// </summary>
	void HealHPMove();
	
	/// <summary>
	/// 持久力回復
	/// </summary>
	void EnduranceRecovery();

private: //静的メンバ変数
	//移動制限
	static const XMFLOAT3 moveMinPos;
	static const XMFLOAT3 moveMaxPos;
	//落下用重力加速度
	static float gravityAccel;

private: //メンバ変数
	std::unique_ptr<FbxModel> model = nullptr;
	std::unique_ptr<Fbx> object = nullptr;

	//剣
	std::unique_ptr<PlayerSword> sword;
	
	//ゲームカメラ
	GameCamera* gameCamera = nullptr;

	//プレイヤーに必要な主なデータ
	std::unique_ptr<PlayerData> data;

	//行動
	std::unique_ptr<PlayerActionBase> action;

	//移動回転用
	Vector3 moveVelRota;
	float moveRotSpeed = 0.0f;
	bool isMoveRotate = false;
	
	//回復中か
	bool isHeal = false;
	//回復タイマー
	std::unique_ptr<Engine::Timer> healTimer = 0;
	//回復前の体力
	int healBeforeHP;
	//回復完了後の体力
	int healAfterHP;
	//体力ゲージ
	std::unique_ptr<Gauge> hpGauge;
	
	//持久力回復開始までのタイマー
	std::unique_ptr<Engine::Timer> enduranceRecoveryStartTimer;
	////持久力ゲージ
	std::unique_ptr<Gauge> enduranceGauge;

	//タイトルシーン用ポータル座標
	Vector3 portalPos;
};