#pragma once
#include "Object/ObjectBase.h"

class Sprite : public ObjectBase
{
public: // サブクラス

	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VERTEX
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};

private:

	/// <summary>
	/// 定数バッファ用データ構造体
	/// </summary>
	struct CONST_BUFFER_DATA
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="_name">テクスチャ保存名</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<Sprite> Create(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT2& _anchorpoint = { 0, 0 }, const XMFLOAT4& _color = { 1,1,1,1 }, bool _isFlipX = false, bool _isFlipY = false);

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_name">テクスチャ保存名</param>
	/// <param name="_anchorpoint">アンカーポイント</param>
	/// <param name="_isFlipX">左右反転するか</param>
	/// <param name="_isFlipY">上下反転するか</param>
	void Initialize(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT2& _anchorpoint = { 0, 0 }, const XMFLOAT4& _color = { 1,1,1,1 }, bool _isFlipX = false, bool _isFlipY = false);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const DrawMode _drawMode = DrawMode::alpha);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Texture* _tex, const DrawMode _drawMode = DrawMode::alpha);

private:

	//パイプライン情報
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;

protected: // メンバ変数

	// 頂点数
	static const int vertNum = 4;
	// 射影行列
	static XMMATRIX matProjection;

	//テクスチャ名
	std::unique_ptr<TextureManager> texture;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position{ 0, 0 };
	// スプライト幅、高さ
	XMFLOAT2 size = { 100.0f, 100.0f };
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ワールド行列
	XMMATRIX matWorld{};
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	XMFLOAT2 texLeftTop = { 0, 0 };
	// テクスチャ幅、高さ
	XMFLOAT2 texSize = { 500.0f, 500.0f };

protected: // メンバ関数

	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVertices();

	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVerticesNoTex();

public:

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { Sprite::pipeline = _pipeline; }
	const XMFLOAT2& GetPosition() { return position; }
	const float GetRotation() { return rotation; }
	const XMFLOAT2& GetSize() { return size; }
	const XMFLOAT2& GetTexSize() { return texSize; }
	const XMFLOAT2& GetTexLeftTop() { return texLeftTop; }
	const XMFLOAT4& GetColor() { return color; }
	const XMFLOAT2& GetAnchorpoint() { return anchorpoint; }
	bool GetIsFlipX() { return isFlipX; }
	bool GetIsFlipY() { return isFlipY; }
	TextureManager* GetTexture() { return texture.get(); }
	void SetTexture(const std::string& _name, bool isChangeSize = true) {
		texture->SetTexture(_name);
		if (!isChangeSize) { return; }
		XMFLOAT2 _size = texture->GetTexSize();
		size = { _size.x, _size.y };
		texSize = { _size.x,_size.y };
	};
	void DeleteTexture() { texture->SetTexture(""); }
	void SetPosition(const XMFLOAT2& _position) { this->position = _position; }
	void SetRotation(float _rotation) { this->rotation = _rotation; }
	void SetSize(const XMFLOAT2& _size) { this->size = _size; }
	void SetScale(float scale){ size = { texSize.x * scale, texSize.y * scale }; }
	void SetTexSize(const XMFLOAT2& _texSize) { this->texSize = _texSize; }
	void SetTexLeftTop(const XMFLOAT2& _texLeftTop) { this->texLeftTop = _texLeftTop; }
	void SetColor(const XMFLOAT4& _color) { this->color = _color; }
	void SetAnchorpoint(const XMFLOAT2& _anchorpoint) { this->anchorpoint = _anchorpoint; }
	void SetIsFlipX(bool _isFlipX) { this->isFlipX = _isFlipX; };
	void SetIsFlipY(bool _isFlipY) { this->isFlipY = _isFlipY; };
};