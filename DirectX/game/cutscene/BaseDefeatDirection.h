#pragma once

#include "camera/CutSceneCamera.h"

#include "Particle/Emitter.h"
#include "Object/2d/Sprite.h"

struct ParticleGrainState
{
	float aliveFrame = 0;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 vel;
	DirectX::XMFLOAT3 acc;
	float s_scale;
	float e_scale;
	DirectX::XMFLOAT4 s_color;
	DirectX::XMFLOAT4 e_color;
};

class BaseDefeatDirection
{
protected:


public:
	BaseDefeatDirection() = default;
	virtual ~BaseDefeatDirection() = default;

	virtual void Init();

	virtual void Update();

	virtual void Draw();

	virtual void Draw2D();

	void Reset();

	void StartDefeatDirection(const DirectX::XMFLOAT3& bossPosition);

	bool GetDirectionEnd() { return isEnded && isCommonEnded; }

protected:
	//継承先で共通で使用する

	//パーティクル関数簡略化
	void InEmitter(Emitter* ptr, ParticleGrainState& pgState);

	void StartCommonDirection();

	//時間
	int frame = 0;
	//演出フラグ
	bool isDirecting = false;
	//終了フラグ
	bool isEnded = false;
	//ボスの座標を保持
	DirectX::XMFLOAT3 bossPosition;

private:
	//全シーンで使用する演出
	int commonDirectFrame = 0;
	//フェードイン時間
	const int TEXT_FADEIN_MAXFRAME = 30;
	bool isFadeInEnd = false;
	//持続時間
	const int TEXT_DISPLAY_MAXFRAME = 120;
	bool isDisplayEnd = false;
	//フェードアウト時間
	const int TEXT_FADEOUT_MAXFRAME = 30;
	bool isFadeOutEnd = false;
	//演出の合計時間
	const int COMMON_DIRECTION_MAXFRAME = TEXT_FADEIN_MAXFRAME + TEXT_FADEOUT_MAXFRAME + TEXT_DISPLAY_MAXFRAME;
	//全シーン共通演出中
	bool isCommonDirecting = false;
	//全シーン共通演出終了
	bool isCommonEnded = false;
	//テキスト
	std::unique_ptr<Sprite> victoryTextUpper;

};