#pragma once
#include "BasePostEffect.h"

class Bloom : public BasePostEffect
{
private:

	struct CONST_BUFFER_DATA {
		float strength;//ブルームの強さ
		XMFLOAT4 pad;
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
	Bloom();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Bloom() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_texName">使用画像名</param>
	/// <returns></returns>
	static std::unique_ptr<Bloom> Create(const std::string& _texName);

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	void Draw();

	void SetStrength(const float _strength) { strength = _strength; }

private:

	//ブルームの強さ
	float strength;
};