#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Texture/TextureManager.h"

class BaseRender
{
public: //静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="_device"></param>
	/// <param name="_cmdList"></param>
	static void StaticInitialize(ID3D12Device* _device,ID3D12GraphicsCommandList* _cmdList);
	
public: //メンバ関数

	BaseRender(const std::string& _texName);
	virtual ~BaseRender();
	
	/// <summary>
	/// シーン描画前処理
	/// </summary>
	virtual void PreDrawScene();

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	virtual void PostDrawScene();

	TextureManager* GetTexture() { return texture.get(); }

private:

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepthBuffer();

private:

	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//初期化色
	static const float clearColor[4];

	//レンダーターゲット用画像
	std::unique_ptr<TextureManager> texture;

	//DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
};

