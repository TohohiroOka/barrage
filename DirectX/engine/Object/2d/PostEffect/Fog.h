#pragma once
#include "BasePostEffect.h"

class Fog : public BasePostEffect
{
private:

	struct CONST_BUFFER_DATA {
		float strength;//フォグの強さ
		float blendRate;//カラーの割合
		XMFLOAT2 pad1;
		XMFLOAT3 color1;//フォグの色（中央）
		float pad2;
		XMFLOAT3 color2;//フォグの色（端）
	};

private:

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Fog();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fog() {};

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	static std::unique_ptr<Fog> Create(const std::string& _texName);

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	void Draw();

	void SetStrength(const float _strength) { strength = _strength; }
	void SetBlendRate(const float _blendRate) { blendRate = _blendRate; }
	void SetColor1(const XMFLOAT3& _color) { color1 = _color; }
	void SetColor2(const XMFLOAT3& _color) { color2 = _color; }

private:

	//フォグの強さ
	float strength;
	//カラーの割合
	float blendRate;
	//フォグの色
	XMFLOAT3 color1;
	//フォグの色
	XMFLOAT3 color2;
};