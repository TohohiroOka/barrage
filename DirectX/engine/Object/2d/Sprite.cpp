#include "Sprite.h"
#include "WindowApp.h"
#include <cassert>

using namespace DirectX;

std::vector<GraphicsPipelineManager::DrawSet> Sprite::pipeline;
XMMATRIX Sprite::matProjection;

Sprite::~Sprite()
{
	vertBuff.Reset();
	constBuff.Reset();
}

void Sprite::StaticInitialize()
{
	// 射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, float(WindowApp::GetWindowWidth()),
		float(WindowApp::GetWindowHeight()), 0.0f,
		0.0f, 1.0f);
}

std::unique_ptr<Sprite> Sprite::Create(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT2& _anchorpoint, const XMFLOAT4& _color, bool _isFlipX, bool _isFlipY)
{
	// Spriteのインスタンスを生成
	Sprite* instance = new Sprite();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize(_name, _position, _anchorpoint, _color, _isFlipX, _isFlipY);

	//更新
	instance->Update();

	//ユニークポインタを返す
	return std::unique_ptr<Sprite>(instance);
}

void Sprite::Initialize(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT2& _anchorpoint, const XMFLOAT4& _color, bool _isFlipX, bool _isFlipY)
{
	texture = std::make_unique<TextureManager>(_name);
	this->position = _position;
	this->anchorpoint = _anchorpoint;
	this-> color = _color;
	this->isFlipX = _isFlipX;
	this->isFlipY = _isFlipY;

	HRESULT result = S_FALSE;

	//テクスチャ情報取得
	XMFLOAT2 _size = texture->GetTexSize();
	//スプライトの大きさを画像の解像度に合わせる
	size = { _size.x, _size.y };
	texSize = { _size.x,_size.y };

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VERTEX) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	if (texture->GetTexture() != "") {
		TransferVertices();
	} else {
		TransferVerticesNoTex();
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VERTEX) * 4;
	vbView.StrideInBytes = sizeof(VERTEX);

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	// 定数バッファにデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}
}

void Sprite::Update()
{
	// ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	//頂点バッファに反映
	if (texture->GetTexture() != "") {
		TransferVertices();
	} else {
		TransferVerticesNoTex();
	}

	// 定数バッファにデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (FAILED(result)) { assert(0); }
	constMap->color = this->color;
	constMap->mat = this->matWorld * matProjection;	// 行列の合成
	this->constBuff->Unmap(0, nullptr);
}

void Sprite::Draw(const DrawMode _drawMode)
{
	//描画しない設定なら抜ける
	if (!isDraw) { return; }

	//テクスチャがセットされていなければ抜ける
	if (texture->GetTexture() == "") { return; }

	int modeNum = int(_drawMode);

	ObjectBase::Draw(pipeline[modeNum]);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, texture->GetDescriptor()->gpu);

	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::Draw(const Texture* _tex, const DrawMode _drawMode)
{
	//描画しない設定なら抜ける
	if (!isDraw) { return; }

	int modeNum = int(_drawMode);

	ObjectBase::Draw(pipeline[modeNum]);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, _tex->descriptor->gpu);
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	VERTEX vertices[vertNum];

	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	// テクスチャ情報取得
	XMFLOAT2 _size = texture->GetTexSize();
	if (_size.x != 0.0f && _size.y != 0.0f) {
		float texLeft = texLeftTop.x / _size.x;
		float texRight = (texLeftTop.x + texSize.x) / _size.x;
		float texTop = texLeftTop.y / _size.y;
		float texBottom = (texLeftTop.y + texSize.y) / _size.y;

		vertices[LB].uv = { texLeft,	texBottom }; // 左下
		vertices[LT].uv = { texLeft,	texTop }; // 左上
		vertices[RB].uv = { texRight,	texBottom }; // 右下
		vertices[RT].uv = { texRight,	texTop }; // 右上
	}

	// 頂点バッファへのデータ転送
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

void Sprite::TransferVerticesNoTex()
{
	HRESULT result = S_FALSE;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	VERTEX vertices[vertNum];

	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	vertices[LB].uv = { 0,	1 }; // 左下
	vertices[LT].uv = { 0,	0 }; // 左上
	vertices[RB].uv = { 1,	1 }; // 右下
	vertices[RT].uv = { 1,	0 }; // 右上

	// 頂点バッファへのデータ転送
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}