﻿#pragma once
#include "Base3D.h"

class Object3d : public Base3D
{
private:// サブクラス

	// 定数バッファ用データ構造体B0
	struct CONST_BUFFER_DATA_B0
	{
		XMFLOAT4 baseColor;//ベースカラー
		XMMATRIX viewproj; // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
		unsigned int isBloom;//ブルームの有無
		unsigned int isToon;//トゥーンの有無
		unsigned int isOutline;//アウトラインの有無
		unsigned int isLight;//ライティングの有無
		float pad;
		XMFLOAT3 outlineColor;
		float pad1;
	};

public: // 静的メンバ関数

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="_model">モデル</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<Object3d> Create(Model* _model);

private:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

public: // メンバ関数w

	Object3d();
	~Object3d();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const DrawMode _drawMode = DrawMode::alpha);

private: // 静的メンバ変数

	//パイプライン情報
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;

	// 名前
	const char* name = nullptr;
	// モデル
	Model* model = nullptr;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	//ベースカラー
	XMFLOAT4 baseColor = { 1,1,1,1 };

public:

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { Object3d::pipeline = _pipeline; }
	void SetModel(Model* _model) { this->model = _model; }
	inline Model* GetModel() { return model; }
};