#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <array>

class DescriptorHeapManager
{
private:
	//デスクリプタの大きさ
	static const int DescriptorsSize = 512;

	struct Descriptors {
		//デスクリプタヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
		//デスクリプタテーブルの制御
		std::array<bool, DescriptorsSize> TableManager;
	};

	enum class DescriptorsType {
		srv,
		rtv,
		dsv,
		size,
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// 描画前準備
	/// </summary>
	/// <param name="_cmdList">コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 描画前準備
	/// </summary>
	/// <param name="_cmdList">コマンドリスト</param>
	static void RtvPreDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DescriptorHeapManager() {};

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~DescriptorHeapManager();

	/// <summary>
	/// シェーダーリソースビューの作成
	/// </summary>
	/// <param name="_texBuffer">テクスチャバッファ</param>
	/// <param name="_srvDesc">シェーダーリソースビュー設定</param>
	void CreateSRV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer, D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc);

	/// <summary>
	/// シェーダーリソースビューの作成
	/// </summary>
	/// <param name="_texBuffer">テクスチャバッファ</param>
	void CreateRTV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer);

	/// <summary>
	/// シェーダーリソースビューの作成
	/// </summary>
	/// <param name="_texBuffer">テクスチャバッファ</param>
	void CreateDSV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer);

	static ID3D12DescriptorHeap* GetDescriptorHeap() {
		return descHeap[int(DescriptorsType::srv)].descHeap.Get();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap[int(DescriptorsType::rtv)].descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberRTV,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap[int(DescriptorsType::dsv)].descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberDSV,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV));
	}

private:

	//デバイス
	static ID3D12Device* device;
	//デスクリプタヒープ（画像）
	static std::array<Descriptors, int(DescriptorsType::size)> descHeap;

public:

	int heapNumberSRV = -1;
	int heapNumberRTV = -1;
	int heapNumberDSV = -1;
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpu;
};