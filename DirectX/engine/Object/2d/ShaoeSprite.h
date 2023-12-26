#pragma once
#include "../ObjectBase.h"
#include "../Object/3d/Model.h"

class ShaoeSprite : public ObjectBase
{
private:

	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct SHAOE_VERTEX
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};

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
	static std::unique_ptr<ShaoeSprite> Create(const std::string& _name, const XMFLOAT2& _position,const XMFLOAT4& _color = { 1,1,1,1 });

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ShaoeSprite() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ShaoeSprite();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_name">テクスチャ保存名</param>
	void Initialize(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT4& _color = { 1,1,1,1 });

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

	// 射影行列
	static XMMATRIX matProjection;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//インデックス数
	int indexNum;
	//テクスチャ名
	std::unique_ptr<TextureManager> texture;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position = { 0, 0 };
	//大きさ
	XMFLOAT2 scale = { 1, 1 };
	// ワールド行列
	XMMATRIX matWorld{};
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };

public:

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { ShaoeSprite::pipeline = _pipeline; }
	const XMFLOAT2& GetPosition() { return position; }
	const float GetRotation() { return rotation; }
	const XMFLOAT4& GetColor() { return color; }
	TextureManager* GetTexture() { return texture.get(); }
	void DeleteTexture() { texture->SetTexture(""); }
	void SetPosition(const XMFLOAT2& _position) { this->position = _position; }
	void SetRotation(float _rotation) { this->rotation = _rotation; }
	void SetScale(const XMFLOAT2& _scale) { this->scale = _scale; }
	void SetColor(const XMFLOAT4& _color) { this->color = _color; }
	void SetModel(Model* _model);
};

