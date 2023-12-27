#include "BasePostEffect.h"

std::vector<GraphicsPipelineManager::DrawSet> BasePostEffect::pipeline;

using namespace DirectX;

BasePostEffect::BasePostEffect()
	:Sprite()
{
}

BasePostEffect::~BasePostEffect()
{
}

void BasePostEffect::Initialize(const EffectTyep _type, const std::string& _texName)
{
	HRESULT result;

	type = _type;

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VERTEX) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// 頂点データ
	VERTEX vertices
		[vertNum] = {
		{{ -1,-1,0.0f }, {0.0f,1.0f}}, // 左下
		{{ -1,+1,0.0f }, {0.0f,0.0f}}, // 左上
		{{ +1,-1,0.0f }, {1.0f,1.0f}}, // 右下
		{{ +1,+1,0.0f }, {1.0f,0.0f}}, // 右上
	};

	// 頂点バッファへのデータ転送
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VERTEX) * 4;
	vbView.StrideInBytes = sizeof(VERTEX);

	texture = std::make_unique<TextureManager>(_texName);
}

void BasePostEffect::Draw()
{
	int modeNum = int(type);

	ObjectBase::Draw(pipeline[modeNum]);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	//シェーダーリソースビュー
	cmdList->SetGraphicsRootDescriptorTable(pipeline[modeNum].constBuffNum, texture->GetDescriptor()->gpu);

	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}