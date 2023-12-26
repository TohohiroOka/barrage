#include "ShaoeSprite.h"
#include "WindowApp.h"
#include <cassert>

using namespace DirectX;

std::vector<GraphicsPipelineManager::DrawSet> ShaoeSprite::pipeline;
XMMATRIX ShaoeSprite::matProjection;

ShaoeSprite::~ShaoeSprite()
{
	constBuff.Reset();
}

void ShaoeSprite::StaticInitialize()
{
	// 射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, float(WindowApp::GetWindowWidth()),
		float(WindowApp::GetWindowHeight()), 0.0f,
		0.0f, 1.0f);
}

std::unique_ptr<ShaoeSprite> ShaoeSprite::Create(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT4& _color)
{
	// Spriteのインスタンスを生成
	ShaoeSprite* instance = new ShaoeSprite();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize(_name, _position, _color);

	//更新
	instance->Update();

	//ユニークポインタを返す
	return std::unique_ptr<ShaoeSprite>(instance);
}

void ShaoeSprite::Initialize(const std::string& _name, const XMFLOAT2& _position, const XMFLOAT4& _color)
{
	texture = std::make_unique<TextureManager>(_name);
	position = _position;
	color = _color;

	// 定数バッファの生成
	HRESULT result = device->CreateCommittedResource(
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

void ShaoeSprite::SetModel(Model* _model)
{
	HRESULT result = S_FALSE;

	std::vector<Mesh::VERTEX> modelVertex = _model->GetMeshes()[0]->GetVertices();
	std::vector<SHAOE_VERTEX> vertices;

	//頂点配列の大きさ
	size_t vertSize = modelVertex.size();
	vertices.resize(vertSize);

	for (int i = 0; i < vertSize; i++) {
		vertices[i].pos = modelVertex[i].pos;
		vertices[i].uv = modelVertex[i].uv;
	}

	vertBuff.Reset();
	indexBuff.Reset();

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	const UINT sizeVB = static_cast<UINT>(sizeof(SHAOE_VERTEX) * vertSize);

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// 頂点バッファへのデータ転送
	SHAOE_VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの生成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(SHAOE_VERTEX);

	std::vector<unsigned long> indices = _model->GetMeshes()[0]->GetIndices();
	indexNum = int(indices.size());
	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	const UINT sizeIB = static_cast<UINT>(sizeof(unsigned long) * indexNum);

	//インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) { assert(0); }

	//インデックスバッファへのデータ転送
	unsigned long* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeIB;
}

void ShaoeSprite::Update()
{
	// ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixScaling(scale.x, scale.y, 0.0f);
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// 定数バッファにデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (FAILED(result)) { assert(0); }
	constMap->color = this->color;
	constMap->mat = this->matWorld * matProjection;	// 行列の合成
	this->constBuff->Unmap(0, nullptr);
}

void ShaoeSprite::Draw(const DrawMode _drawMode)
{
	//テクスチャがセットされていなければ抜ける
	if (texture->GetTexture() == "") { return; }

	int modeNum = int(_drawMode)+2;

	ObjectBase::Draw(pipeline[modeNum]);

	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, texture->GetDescriptor()->gpu);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(UINT(indexNum), 1, 0, 0, 0);
}

void ShaoeSprite::Draw(const Texture* _tex, const DrawMode _drawMode)
{
	int modeNum = int(_drawMode);

	ObjectBase::Draw(pipeline[modeNum]);

	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, _tex->descriptor->gpu);
	// 描画コマンド
	cmdList->DrawIndexedInstanced(UINT(indexNum), 1, 0, 0, 0);
}