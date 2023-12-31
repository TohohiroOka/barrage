#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <map>

class ShaderManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	ShaderManager() {};
	~ShaderManager() {};

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<ShaderManager> Create();

	/// <summary>
	/// シェーダーの読み込み
	/// </summary>
	void LoadShader();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

public:

	//シェーダー
	std::map<std::string, ComPtr<ID3DBlob>> shaderObject;
	//コンピュートシェーダーのオブジェクト
	std::map<std::string, ComPtr<ID3DBlob>> shaderObjectCS;
};