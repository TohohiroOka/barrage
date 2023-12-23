#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "Pipeline/GraphicsPipelineManager.h"
#include "Texture.h"
#include "EngineUseTexture.h"

class TextureManager
{
private:

	struct INFORMATION
	{
		bool isDelete = false; //シーン遷移で削除を行うか
		std::unique_ptr<Texture> instance = nullptr;
	};

public:

	TextureManager(const std::string& _name);
	~TextureManager();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="_keepName">保存名</param>
	/// <param name="_filename">画像ファイル名</param>
	/// <param name="_isDelete">シーン遷移で削除を行うか</param>
	static void LoadTexture(const std::string& _keepName, const std::string& _filename, bool _isDelete = true);

	/// <summary>
	/// レンダーターゲット用画像生成
	/// </summary>
	/// <param name="_fileName"></param>
	static void CreateRenderTexture(const std::string& _fileName);

	/// <summary>
	/// 深度値用画像作成
	/// </summary>
	/// <param name="_fileName"></param>
	/// <param name="_texSize">画像サイズ</param>
	static void CreateDepthTexture(const std::string& _fileName, const std::array<UINT, 2>& _texSize = {});

	/// <summary>
	/// シーンごとの解放処理
	/// </summary>
	static void SceneFinalize();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// その名前の画像が存在するか
	/// </summary>
	/// <param name="_name">画像名</param>
	/// <returns></returns>
	static bool GetIsTextureName(const std::string& _name) {
		return texture.count(_name);
	}

	std::array<UINT, 2>& GetSize() {
		return texture[name].instance->texSize;
	}

	ID3D12Resource* GetTexBuffer() {
		return texture[name].instance->texBuffer.Get();
	}

	DescriptorHeapManager* GetDescriptor() {
		return texture[name].instance->descriptor.get();
	}

	/// <summary>
	/// 画像の大きさを出力
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT2 GetTexSize() {
		//指定番号の画像が読み込み済みなら
		if (texture[name].instance->texBuffer.Get()) {
			return { float(texture[name].instance->texSize[0]),float(texture[name].instance->texSize[1]) };
		} else {
			return { 0.0f,0.0f };
		}
	}

	void SetTexture(const std::string& _name) { name = _name; }
	std::string& GetTexture() { return name; }

private:
	//テクスチャ情報
	static std::unordered_map<std::string, INFORMATION> texture;

	std::string name;
};

