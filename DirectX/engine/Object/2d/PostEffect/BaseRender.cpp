#include "../Object/2d/PostEffect/BaseRender.h"
#include "WindowApp.h"

ID3D12Device* BaseRender::device = nullptr;
ID3D12GraphicsCommandList* BaseRender::cmdList = nullptr;
const float BaseRender::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

void BaseRender::StaticInitialize(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList)
{
	device = _device;
	cmdList = _cmdList;
}

BaseRender::BaseRender(const std::string& _texName)
{
	// テクスチャ用バッファの生成
	texture = std::make_unique<TextureManager>(_texName);

	CreateDepthBuffer();
}

BaseRender::~BaseRender()
{
	descHeapDSV.Reset();
	depthBuffer.Reset();
}

void BaseRender::PreDrawScene()
{
	//リソースバリアを変更
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texture->GetTexBuffer(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//レンダーターゲットビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs = texture->GetDescriptor()->GetRtvHandle();

	//深度ステンシルビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvHs, false, &dsvH);

	//ビューポート設定
	CD3DX12_VIEWPORT viewports =
		CD3DX12_VIEWPORT(0.0f, 0.0f, FLOAT(WindowApp::GetWindowWidth()), FLOAT(WindowApp::GetWindowHeight()));
	CD3DX12_RECT scissorRects =
		CD3DX12_RECT(0, 0, LONG(WindowApp::GetWindowWidth()), LONG(WindowApp::GetWindowHeight()));

	cmdList->RSSetViewports(1, &viewports);
	//シザリング矩形設定
	cmdList->RSSetScissorRects(1, &scissorRects);

	//全画面クリア
	cmdList->ClearRenderTargetView(rtvHs, clearColor, 0, nullptr);

	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void BaseRender::PostDrawScene()
{
	//リソースバリアを変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture->GetTexBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void BaseRender::CreateDepthBuffer()
{
	HRESULT result;

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32_TYPELESS,
			WindowApp::GetWindowWidth(),
			(UINT)WindowApp::GetWindowHeight(),
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//深度バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));
	if (FAILED(result)) { assert(0); }

	//DSV用デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビュー
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは1つ

	//デスクリプタヒープにDSV生成
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	if (FAILED(result)) { assert(0); }

	//深度バッファリソース設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}