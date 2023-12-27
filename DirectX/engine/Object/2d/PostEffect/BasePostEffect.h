#pragma once
#include "Object/2d/Sprite.h"

class BasePostEffect :public Sprite
{
public:

	enum class EffectTyep {
		bloom,
		outline,
		fog,
		size,
	};

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BasePostEffect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BasePostEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const EffectTyep _type, const std::string& _texName);

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	void Draw();

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { BasePostEffect::pipeline = _pipeline; }

private://静的メンバ変数

	//パイプライン情報
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;

private://メンバ変数
	
	//エフェクト種類
	EffectTyep type;

};