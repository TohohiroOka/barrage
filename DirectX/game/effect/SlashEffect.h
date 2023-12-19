#pragma once
#include "Particle/Emitter.h"
#include "Object/3d/Object3d.h"
#include "Math/Vector3.h"

/// <summary>
/// 斬撃演出
/// </summary>
class SlashEffect
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="texName">パーティクルテクスチャ名</param>
	/// <param name="emitNum">線を構成するパーティクルエミッター数</param>
	/// <param name="frameParticleNum">1フレーム前の発生座標との間に発生させるパーティクル数</param>
	/// <param name="maxFrame">パーティクル生存時間</param>
	/// <param name="startScale">生成時の大きさ</param>
	/// <param name="endScale">消滅時の大きさ</param>
	/// <param name="startColor">生成時の色</param>
	/// <param name="endColor">消滅時の色</param>
	/// <param name="isParticleEmit">パーティクルを発生させるか</param>
	SlashEffect(const std::string& texName, int emitNum, int frameParticleNum, float maxFrame, float startScale, float endScale,
		DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor, bool isParticleEmit = true);
	~SlashEffect() {};

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="point1">線の端1</param>
	/// <param name="point2">線の端2</param>
	void Update(const Vector3& point1, const Vector3& point2);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	//setter
	void SetEmitNum(int emitNum) { emitPosLists.resize(emitNum); }
	void SetFrameParticleNum(int frameParticleNum) { this->frameParticleNum = frameParticleNum; }
	void SetIsParticleEmit(bool isParticleEmit) { this->isParticleEmit = isParticleEmit; }
	void SetMaxFrame(float maxFrame) { this->maxFrame = maxFrame; }
	void SetStartScale(float startScale) { this->startScale = startScale; }
	void SetEndScale(float endScale) { this->endScale = endScale; }
	void SetStartColor(const DirectX::XMFLOAT4& startColor) { this->startColor = startColor; }
	void SetEndColor(const DirectX::XMFLOAT4& endColor) { this->endColor; }

private: //メンバ関数
	/// <summary>
	/// 発生座標更新
	/// </summary>
	/// <param name="point1">線の端1</param>
	/// <param name="point2">線の端2</param>
	void UpdateEmitPos(const Vector3& point1, const Vector3& point2);

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void AddParticle();

private: //メンバ変数
	//パーティクル発生エミッター
	std::array<std::unique_ptr<Emitter>, 3> particle;
	//発生座標保存用list
	std::vector<std::list<Vector3>> emitPosLists;
	//1フレーム前の発生座標との間に発生させるパーティクル数
	int frameParticleNum;
	//パーティクルを発生させるか
	bool isParticleEmit;

	//パーティクル生存時間
	float maxFrame;
	//生成時の大きさ
	float startScale;
	//消滅時の大きさ
	float endScale;
	//生成時の色
	DirectX::XMFLOAT4 startColor;
	//消滅時の色
	DirectX::XMFLOAT4 endColor;
};

