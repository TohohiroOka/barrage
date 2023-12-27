#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Texture/TextureManager.h"

class DepthRender
{
public: //静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="_device"></param>
	/// <param name="_cmdList"></param>
	static void StaticInitialize(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList);

public: //メンバ関数
	DepthRender(const std::string& _texName);
	~DepthRender() {};

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	void PostDrawScene();

	TextureManager* GetTexture() { return texture.get(); }

private:

	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//初期化色
	static const float clearColor[4];

	//レンダーターゲット用画像
	std::unique_ptr<TextureManager> texture;

};

