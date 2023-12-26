#include "PostEffect.h"
#include "WindowApp.h"

using namespace DirectX;

const float PostEffect::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
std::vector<GraphicsPipelineManager::DrawSet> PostEffect::pipeline;

PostEffect::PostEffect()
	:Sprite()
{
}

PostEffect::~PostEffect()
{
	descHeapDSV.Reset();
}

void PostEffect::Initialize()
{
	HRESULT result;

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

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	//使用する画像指定
	for (int i = 0; i<int(TexType::size); i++) {
		texture[i] = std::make_unique<TextureManager>(Engine::EngineUseRTVTexture[i]);
	}
}

void PostEffect::CreateDepthBuffer()
{
	HRESULT result;

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
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
		IID_PPV_ARGS(&depthBuff));
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
	device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

std::unique_ptr<PostEffect> PostEffect::Create()
{
	// Spriteのインスタンスを生成
	PostEffect* instance = new PostEffect();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();
	//深度バッファ生成
	instance->CreateDepthBuffer();

	//ユニークポインタを返す
	return std::unique_ptr<PostEffect>(instance);
}

void PostEffect::Draw(const std::vector<TextureManager*> _tex)
{
	// 定数バッファへデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->outline = 1.0f;
		constBuff->Unmap(0, nullptr);
	}

	ObjectBase::Draw(pipeline[0]);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	//シェーダーリソースビュー
	for (int i = 0; i < pipeline[0].texNum; i++) {
		cmdList->SetGraphicsRootDescriptorTable(i + 1, _tex[i]->GetDescriptor()->gpu);
	}
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDrawScene()
{
	const int renderNum = int(TexType::size);

	//リソースバリアを変更
	for (int i = 0; i < renderNum; i++) {
		cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texture[i]->GetTexBuffer(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
	}

	//レンダーターゲットビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[renderNum];
	for (int i = 0; i < renderNum; i++)
	{
		rtvHs[i] = texture[i]->GetDescriptor()->GetRtvHandle();
	}

	//深度ステンシルビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(renderNum, rtvHs, false, &dsvH);

	//ビューポート設定
	CD3DX12_VIEWPORT viewports[renderNum];
	CD3DX12_RECT scissorRects[renderNum];
	for (int i = 0; i < renderNum; i++)
	{
		XMFLOAT2 size = texture[i]->GetTexSize();
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, FLOAT(size.x), FLOAT(size.y));
		scissorRects[i] = CD3DX12_RECT(0, 0, LONG(size.x), LONG(size.y));
	}

	cmdList->RSSetViewports(renderNum, viewports);
	//シザリング矩形設定
	cmdList->RSSetScissorRects(renderNum, scissorRects);

	//全画面クリア
	for (int i = 0; i < renderNum; i++)
	{
		cmdList->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene()
{
	const int renderNum = int(TexType::size);

	//リソースバリアを変更
	for (int i = 0; i < renderNum; i++)
	{
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[i]->GetTexBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}