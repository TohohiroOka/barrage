#pragma once
#include "Object/3d/Object3d.h"

/// <summary>
/// 吹き出しオブジェクト
/// </summary>
class SpeechBubble
{
public: //メンバ関数
	SpeechBubble(Camera* camera, const std::string& modelName, const DirectX::XMFLOAT3& position, float scale);
	~SpeechBubble() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: //メンバ変数
	//オブジェクト
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;

	//カメラ参照
	Camera* camera;
};