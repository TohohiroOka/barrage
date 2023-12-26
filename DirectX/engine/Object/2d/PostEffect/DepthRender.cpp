#include "DepthRender.h"
#include "WindowApp.h"

ID3D12Device* DepthRender::device = nullptr;
ID3D12GraphicsCommandList* DepthRender::cmdList = nullptr;
const float DepthRender::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

using namespace DirectX;

void DepthRender::StaticInitialize(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList)
{
	device = _device;
	cmdList = _cmdList;
}

DepthRender::DepthRender(const std::string& _texName)
{
	// テクスチャ用バッファの生成
	texture = std::make_unique<TextureManager>(_texName);
}

void DepthRender::PreDrawScene()
{
	//リソースバリアを変更(シェーダリソース→描画可能)
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texture->GetTexBuffer(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = texture->GetDescriptor()->GetDsvHandle();

	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(0, nullptr, false, &dsvH);

	std::array<UINT, 2> size = texture->GetSize();

	//ビューポートの設定
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		FLOAT(size[0]), FLOAT(size[1])));
	//シザリング矩形の設定
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0,
		LONG(size[0]), LONG(size[1])));

	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
		nullptr);
}

void DepthRender::PostDrawScene()
{
	//リソースバリアを変更(描画可能→シェーダリソース)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture->GetTexBuffer(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ));
}
