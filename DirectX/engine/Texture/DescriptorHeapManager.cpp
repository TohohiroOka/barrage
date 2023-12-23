#include "DescriptorHeapManager.h"

ID3D12Device* DescriptorHeapManager::device = nullptr;
DescriptorHeapManager::DescriptorsSRV DescriptorHeapManager::descHeapSRV;
DescriptorHeapManager::DescriptorsRTV DescriptorHeapManager::descHeapRTV;
DescriptorHeapManager::DescriptorsDSV DescriptorHeapManager::descHeapDSV;

void DescriptorHeapManager::StaticInitialize(ID3D12Device* _device)
{
	HRESULT result = S_FALSE;

	// nullptrチェック
	assert(!DescriptorHeapManager::device);
	assert(_device);
	DescriptorHeapManager::device = _device;

	//デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = DescriptorsSRVSize;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV.descHeap));
	if (FAILED(result)) { assert(0); }

	//デスクリプタヒープの生成rtv
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHescDesc{};
	rtvDescHescDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHescDesc.NumDescriptors = DescriptorsRTVSize;
	result = device->CreateDescriptorHeap(&rtvDescHescDesc, IID_PPV_ARGS(&descHeapRTV.descHeap));
	if (FAILED(result)) { assert(0); }

	//デスクリプタヒープの生成dsv
	D3D12_DESCRIPTOR_HEAP_DESC dsvDescHescDesc{};
	dsvDescHescDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvDescHescDesc.NumDescriptors = DescriptorsDSVSize;
	result = device->CreateDescriptorHeap(&dsvDescHescDesc, IID_PPV_ARGS(&descHeapDSV.descHeap));
	if (FAILED(result)) { assert(0); }

}

void DescriptorHeapManager::PreDraw(ID3D12GraphicsCommandList* _cmdList)
{
	assert(descHeapSRV.descHeap);
	//デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.descHeap.Get() };
	_cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void DescriptorHeapManager::Finalize()
{
	descHeapSRV.descHeap.Reset();
	descHeapRTV.descHeap.Reset();
	descHeapDSV.descHeap.Reset();
}

DescriptorHeapManager::~DescriptorHeapManager()
{
	descHeapSRV.TableManager[heapNumberSRV] = false;
	if (heapNumberRTV != -1) {
		descHeapRTV.TableManager[heapNumberRTV] = false;
	}
	if (heapNumberDSV != -1) {
		descHeapDSV.TableManager[heapNumberDSV] = false;
	}
}

void DescriptorHeapManager::CreateSRV(
	Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer, D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc)
{
	heapNumberSRV++;
	for (; heapNumberSRV < DescriptorsSRVSize; heapNumberSRV++)
	{
		if (!descHeapSRV.TableManager[heapNumberSRV])
		{
			descHeapSRV.TableManager[heapNumberSRV] = true;
			break;
		}
	}

	cpu = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapSRV.descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberSRV,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	gpu = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV.descHeap->GetGPUDescriptorHandleForHeapStart(), heapNumberSRV,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	//ヒープのtexnumber番目にシェーダーリソースビューを作成
	device->CreateShaderResourceView(
		_texBuffer.Get(),
		&_srvDesc,
		cpu);
}


void DescriptorHeapManager::CreateRTV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer)
{
	heapNumberRTV++;
	for (; heapNumberRTV < DescriptorsRTVSize; heapNumberRTV++)
	{
		if (!descHeapRTV.TableManager[heapNumberRTV])
		{
			descHeapRTV.TableManager[heapNumberRTV] = true;
			break;
		}
	}

	//ヒープのtexnumber番目にレンダーターゲットビューを作成
	device->CreateRenderTargetView(
		_texBuffer.Get(),
		nullptr,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV.descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberRTV,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
}

void DescriptorHeapManager::CreateDSV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer)
{
	heapNumberDSV++;
	for (; heapNumberDSV < DescriptorsDSVSize; heapNumberDSV++)
	{
		if (!descHeapDSV.TableManager[heapNumberDSV])
		{
			descHeapDSV.TableManager[heapNumberDSV] = true;
			break;
		}
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//ヒープのn番目にデプスステンシルビューを作成
	device->CreateDepthStencilView(
		_texBuffer.Get(),
		&dsvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapDSV.descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumberDSV,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV)));
}
